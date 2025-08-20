//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "TlsfAllocator.h"
#include "Logging.h"

#include <memory>

#define TO_BYTES(val) reinterpret_cast<uint8_t*>(val)
#define TO_HEADER(val) reinterpret_cast<BlockHeader*>(val)
#define TO_FOOTER(val) reinterpret_cast<BlockFooter*>(val)
#define TO_FREE_BLOCK(val) reinterpret_cast<FreeBlockNode*>(val)

#define HEADER_SIZE sizeof(BlockHeader)
#define FOOTER_SIZE sizeof(BlockFooter)
#define FREE_BLOCK_SIZE sizeof(FreeBlockNode)
#define PAD_SIZE(size) size + HEADER_SIZE + FOOTER_SIZE

#define FL_MIN_INDEX 4
#define MAX_SL_BUCKETS 16
#define FL(size) 63 - __builtin_clzll(size)
#define SL(size, fl) (size >> fl) & ((1 << FL_MIN_INDEX) - 1);
#define FLAG_OFFSET 3
#define MIN_ALLOCATION 16
#define INVALID_INDEX UINT64_MAX

namespace Siege
{

TlsfAllocator::TlsfAllocator() {}

TlsfAllocator::TlsfAllocator(const uint64_t size)
    : capacity {size}, bytesRemaining {size}
{
    if (size == 0 || size < MIN_ALLOCATION || size >= UINT64_MAX)
    {
        capacity = 0;
        bytesRemaining = 0;
        return;
    }

    uint64_t maxBuckets = (FL(size) - FL_MIN_INDEX) + 1;

    uint64_t totalSize = PAD_SIZE(size);
    uint64_t slBucketSize = sizeof(uint16_t) * maxBuckets;
    uint64_t freeListSize = maxBuckets * MAX_SL_BUCKETS * sizeof(FreeBlockNode*);

    uint64_t allocSize = totalSize + slBucketSize + freeListSize;

    data = TO_BYTES(calloc(1, allocSize));

    CC_ASSERT(data, "Allocation returned null. This should not happen and implies an implementation failure.")

    freeList = (FreeBlockNode**)(data + totalSize);
    slBitmasks = (uint16_t*)(data + totalSize + freeListSize);

    BlockHeader* firstHeader = CreateHeader(data, totalSize, FREE);

    AddNewBlock(totalSize, firstHeader);
}

TlsfAllocator::BlockHeader* TlsfAllocator::CreateHeader(uint8_t* ptr, const uint64_t size,
                                                        HeaderFlags flags)
{
    if (!ptr) return nullptr;
    BlockHeader* header = TO_HEADER(ptr);
    header->sizeAndFlags = (size << FLAG_OFFSET) | flags;
    return header;
}

void* TlsfAllocator::Allocate(const uint64_t& size)
{
    size_t requiredSize = sizeof(BlockHeader) + size + sizeof(BlockFooter);
    if (!data || capacity == 0 ||size == 0 || requiredSize < size || requiredSize > bytesRemaining
        || requiredSize < MIN_ALLOCATION) return nullptr;

    FreeBlockNode* block = FindFreeBlock(requiredSize);

    if (!block) return nullptr;

    BlockHeader* header = TrySplitBlock(block, requiredSize);
    header = CreateHeader(TO_BYTES(header), requiredSize, FULL);
    BlockFooter* footer = CreateFooter(TO_BYTES(GetFooter(header)) , requiredSize);

    uint8_t* ptr = GetBlockData(header);
    bytesRemaining -= requiredSize;
    return ptr;
}

TlsfAllocator::BlockHeader* TlsfAllocator::TrySplitBlock(TlsfAllocator::FreeBlockNode* node, uint64_t allocatedSize)
{
    if (!node) return nullptr;

    uint64_t oldSize = GetHeaderSize(GetHeader(node));

    BlockHeader* header = GetHeader(node);

    if (!RemoveFreeBlock(node)) return nullptr;

    if (oldSize <= allocatedSize || ((oldSize - allocatedSize) <
                                     (HEADER_SIZE + FREE_BLOCK_SIZE + FOOTER_SIZE))) return header;

    AddNewBlock(oldSize - allocatedSize, TO_HEADER(TO_BYTES(header) + allocatedSize));

    return header;
}

void TlsfAllocator::AddNewBlock(const uint64_t size, BlockHeader* header)
{
    uint64_t fl = 0,sl = 0, index;
    index = CalculateFreeBlockIndices(size, fl, sl);

    BlockHeader* newHeader = CreateHeader(TO_BYTES(header), size, FREE);
    FreeBlockNode* node = CreateFreeBlock(TO_BYTES(GetFreeBlock(newHeader)), nullptr, freeList[index]);
    BlockFooter* footer = CreateFooter(TO_BYTES(GetFooter(newHeader)), size);

    if (node->next) node->next->prev = node;

    freeList[index] = node;
    flBitmask |= (1ULL << fl);
    slBitmasks[fl] |= (1 << sl);
}

bool TlsfAllocator::RemoveFreeBlock(TlsfAllocator::FreeBlockNode* node)
{
    BlockHeader* header = GetHeader(node);

    if (!header) return false;

    uint64_t oldSize = GetHeaderSize(header);

    uint64_t fl, sl, index;

    index = CalculateFreeBlockIndices(oldSize, fl, sl);

    if (!node->prev) freeList[index] = node->next;
    else node->prev->next = node->next;

    if (node->next) node->next->prev = node->prev;

    if (!freeList[index]) slBitmasks[fl] &= ~(1 << sl);
    if (!slBitmasks[fl]) flBitmask &= ~(1ULL << fl);

    node->next = nullptr;
    node->prev = nullptr;

    return true;
}

TlsfAllocator::FreeBlockNode* TlsfAllocator::FindFreeBlock(const uint64_t& size)
{
    uint64_t fl, sl, index;
    index = CalculateFreeBlockIndices(size, fl, sl);

    if (!IsFree(fl, sl)) index = GetNextFreeSlotIndex(fl, sl);
    if (index == INVALID_INDEX) return nullptr;

    return freeList[index];
}

const uint64_t TlsfAllocator::GetNextFreeSlotIndex(uint64_t& fl, uint64_t& sl)
{
    sl = __builtin_ctz(slBitmasks[fl] & ~(((1 << (sl + 1)) - 1)));

    if (sl == 32) sl = 0;
    if (sl) return fl * MAX_SL_BUCKETS + sl;

    fl = flBitmask & ~(((1ULL << (sl + 1)) - 1));

    if (!fl) return INVALID_INDEX;

    fl = __builtin_ctzll(fl);
    CC_ASSERT(slBitmasks[fl] > 0,
              "SlBitmasks is returning 0. This should not be happening and indicates an implementation error.")

    sl = __builtin_ctz(slBitmasks[fl]);

    return fl * MAX_SL_BUCKETS + sl;
}

uint64_t TlsfAllocator::CalculateFreeBlockIndices(uint64_t size, OUT uint64_t & fl, OUT uint64_t & sl)
{
    uint64_t rawFl = FL(size);
    fl = rawFl - FL_MIN_INDEX;
    sl = SL(size, fl);
    return fl * MAX_SL_BUCKETS + sl;
}

TlsfAllocator::BlockFooter* TlsfAllocator::CreateFooter(uint8_t* ptr, const uint64_t size)
{
    if (!ptr) return nullptr;
    BlockFooter* footer = TO_FOOTER(ptr);
    footer->totalBlockSize = size;
    return footer;
}

TlsfAllocator::BlockHeader* TlsfAllocator::GetHeader(TlsfAllocator::FreeBlockNode* node)
{
    if (!node) return nullptr;
    return TO_HEADER(TO_BYTES(node) - HEADER_SIZE);
}

TlsfAllocator::BlockHeader* TlsfAllocator::GetPrevHeader(TlsfAllocator::BlockHeader* header)
{
    if (!header || TO_BYTES(header) == data) return nullptr;
    return TO_HEADER(TO_BYTES(header) - GetPrevFooter(header)->totalBlockSize);
}

TlsfAllocator::BlockHeader* TlsfAllocator::GetNextHeader(TlsfAllocator::BlockHeader* header)
{
    if (!header) return nullptr;
    return TO_HEADER(TO_BYTES(header) + GetHeaderSize(header));
}

TlsfAllocator::FreeBlockNode* TlsfAllocator::CreateFreeBlock(uint8_t* ptr,
                                                             TlsfAllocator::FreeBlockNode* prev,
                                                             TlsfAllocator::FreeBlockNode* next)
{
    if (!ptr) return nullptr;
    FreeBlockNode* node = TO_FREE_BLOCK(ptr);
    node->prev = prev;
    node->next = next;
    return node;
}

TlsfAllocator::FreeBlockNode* TlsfAllocator::GetFreeBlock(BlockHeader* header)
{
    if (!IsFree(header)) return nullptr;
    return TO_FREE_BLOCK(TO_BYTES(header) + HEADER_SIZE);
}

TlsfAllocator::FreeBlockNode* TlsfAllocator::GetFreeBlock(const uint64_t fl, const uint64_t sl)
{
    return freeList[fl * MAX_SL_BUCKETS + sl];
}

TlsfAllocator::BlockFooter* TlsfAllocator::GetFooter(TlsfAllocator::BlockHeader* header)
{
    uint64_t size = GetHeaderSize(header);
    return TO_FOOTER(TO_BYTES(header) + (size - FOOTER_SIZE));
}

TlsfAllocator::BlockFooter* TlsfAllocator::GetPrevFooter(TlsfAllocator::BlockHeader* header)
{
    uint8_t* raw = TO_BYTES(header);
    if (raw == data) return nullptr;
    return TO_FOOTER(raw - FOOTER_SIZE);
}

uint8_t* TlsfAllocator::GetBlockData(TlsfAllocator::BlockHeader* header)
{
    return TO_BYTES(header) + HEADER_SIZE;
}

const uint64_t TlsfAllocator::GetHeaderSize(BlockHeader* header)
{
    return header->sizeAndFlags >> FLAG_OFFSET;
}

bool TlsfAllocator::IsFree(BlockHeader* header)
{
    return header->sizeAndFlags & FREE;
}

bool TlsfAllocator::IsFree(uint64_t fl, uint64_t sl)
{
    return slBitmasks[fl] & (1 << sl);
}

bool TlsfAllocator::PrevBlockIsFree(BlockHeader* header)
{
    uint8_t* raw = TO_BYTES(header);
    if (raw == data) return false;
    return header && IsFree(TO_HEADER(raw - GetPrevFooter(header)->totalBlockSize));
}

} // namespace Siege