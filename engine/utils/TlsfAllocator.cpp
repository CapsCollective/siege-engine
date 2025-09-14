//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "TlsfAllocator.h"

#include <memory>

#include "Logging.h"

#define TO_BYTES(val) reinterpret_cast<uint8_t*>(val)
#define TO_HEADER(val) reinterpret_cast<BlockHeader*>(val)
#define TO_FOOTER(val) reinterpret_cast<BlockFooter*>(val)
#define TO_FREE_BLOCK(val) reinterpret_cast<FreeBlockNode*>(val)

#define HEADER_SIZE sizeof(BlockHeader)
#define FOOTER_SIZE sizeof(BlockFooter)
#define FREE_BLOCK_SIZE sizeof(FreeBlockNode)
#define METADATA_OVERHEAD HEADER_SIZE + FOOTER_SIZE
#define PAD_SIZE(size) size + METADATA_OVERHEAD

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
{
    uint64_t paddedSize = PAD_SIZE(size);
    if (size == 0 || size < MIN_ALLOCATION || size > (UINT64_MAX - METADATA_OVERHEAD) ||
        paddedSize < size)
        return;

    uint64_t maxBuckets = (FL(size) - FL_MIN_INDEX) + 1;

    uint64_t slBucketSize = sizeof(uint16_t) * maxBuckets;
    uint64_t freeListSize = maxBuckets * MAX_SL_BUCKETS * sizeof(FreeBlockNode*);

    uint64_t allocSize = paddedSize + slBucketSize + freeListSize;

    totalSize = paddedSize;
    totalBytesRemaining = paddedSize;

    capacity = size;
    bytesRemaining = size;

    data = TO_BYTES(calloc(1, allocSize));

    CC_ASSERT(
        data,
        "Allocation returned null. This should not happen and implies an implementation failure.")

    freeList = (FreeBlockNode**) (data + paddedSize);
    slBitmasks = (uint16_t*) (data + paddedSize + freeListSize);

    CreateHeader(data, paddedSize, FREE);

    AddNewBlock(paddedSize, TO_HEADER(data));
}

void TlsfAllocator::CreateHeader(uint8_t* ptr, const uint64_t size, HeaderFlags flags)
{
    if (!ptr) return;
    BlockHeader* header = TO_HEADER(ptr);
    header->sizeAndFlags = (size << FLAG_OFFSET) | flags;
}

void* TlsfAllocator::Allocate(const uint64_t& size)
{
    uint64_t requiredSize = sizeof(BlockHeader) + size + sizeof(BlockFooter);
    if (!data || capacity == 0 || size == 0 || requiredSize < size ||
        requiredSize > totalBytesRemaining || size < MIN_ALLOCATION)
        return nullptr;

    FreeBlockNode* block = FindFreeBlock(requiredSize);

    if (!block) return nullptr;

    BlockHeader* header = TrySplitBlock(block, requiredSize);

    header->sizeAndFlags = (requiredSize << FLAG_OFFSET) | FULL;
    CreateFooter(TO_BYTES(GetFooter(header)), requiredSize);

    uint8_t* ptr = GetBlockData(header);
    bytesRemaining -= size;
    totalBytesRemaining -= requiredSize;
    return ptr;
}

TlsfAllocator::BlockHeader* TlsfAllocator::TrySplitBlock(TlsfAllocator::FreeBlockNode* node,
                                                         uint64_t& allocatedSize)
{
    if (!node) return nullptr;

    uint64_t oldSize = GetHeaderSize(GetHeader(node));

    BlockHeader* header = GetHeader(node);

    if (!RemoveFreeBlock(node)) return nullptr;

    if (oldSize <= allocatedSize ||
        ((oldSize - allocatedSize) < (HEADER_SIZE + FREE_BLOCK_SIZE + FOOTER_SIZE)))
    {
        allocatedSize = oldSize;
        return header;
    }

    BlockHeader* newFreeBlock = TO_HEADER(TO_BYTES(header) + allocatedSize);

    AddNewBlock(oldSize - allocatedSize, newFreeBlock);

    return header;
}

void TlsfAllocator::AddNewBlock(const uint64_t size, BlockHeader* header)
{
    uint64_t fl = 0, sl = 0, index;
    index = CalculateFreeBlockIndices(size, fl, sl);

    CreateHeader(TO_BYTES(header), size, FREE);
    FreeBlockNode* node = CreateFreeBlock(TO_BYTES(GetFreeBlock(header)), nullptr, freeList[index]);
    CreateFooter(TO_BYTES(GetFooter(header)), size);

    if (node && node->next) node->next->prev = node;

    freeList[index] = node;
    flBitmask |= (1ULL << fl);
    slBitmasks[fl] |= (1 << sl);
}

TlsfAllocator::BlockHeader* TlsfAllocator::TryCoalesce(BlockHeader* header, OUT uint64_t& size)
{
    BlockHeader* start = header;
    BlockHeader* prev = GetPrevHeader(start);
    BlockHeader* next = GetNextHeader(header);

    if (prev && IsFree(prev))
    {
        start = prev;
        size += GetHeaderSize(prev);
        RemoveFreeBlock(GetFreeBlock(prev));
    }

    if (next && IsFree(next))
    {
        size += GetHeaderSize(next);
        RemoveFreeBlock(GetFreeBlock(next));
    }

    return start;
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
    // 1000 0000 0000 1000
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

    fl = flBitmask & ~(((1ULL << (fl + 1)) - 1));

    if (!fl) return INVALID_INDEX;

    fl = __builtin_ctzll(fl);
    CC_ASSERT(slBitmasks[fl] > 0,
              "SlBitmasks is returning 0. This should not be happening and indicates an "
              "implementation error.")

    sl = __builtin_ctz(slBitmasks[fl]);

    return fl * MAX_SL_BUCKETS + sl;
}

uint64_t TlsfAllocator::CalculateFreeBlockIndices(uint64_t size, OUT uint64_t& fl, OUT uint64_t& sl)
{
    uint64_t rawFl = FL(size);
    fl = rawFl - FL_MIN_INDEX;
    sl = SL(size, fl);
    return fl * MAX_SL_BUCKETS + sl;
}

void TlsfAllocator::CreateFooter(uint8_t* ptr, const uint64_t size)
{
    if (!ptr) return;
    BlockFooter* footer = TO_FOOTER(ptr);
    footer->totalBlockSize = size;
}

TlsfAllocator::BlockHeader* TlsfAllocator::GetHeader(TlsfAllocator::FreeBlockNode* node)
{
    if (!node) return nullptr;
    uint8_t* rawHeader = TO_BYTES(node) - HEADER_SIZE;
    if (!IsValid(rawHeader)) return nullptr;
    return TO_HEADER(rawHeader);
}

TlsfAllocator::BlockHeader* TlsfAllocator::GetHeader(uint8_t* ptr)
{
    if (!ptr) return nullptr;
    uint8_t* rawHeader = ptr - HEADER_SIZE;
    if (!IsValid(rawHeader)) return nullptr;
    return TO_HEADER(rawHeader);
}

TlsfAllocator::BlockHeader* TlsfAllocator::GetPrevHeader(TlsfAllocator::BlockHeader* header)
{
    if (!header) return nullptr;
    uint8_t* rawPrevFooter = TO_BYTES(GetPrevFooter(header));
    if (!IsValid(rawPrevFooter)) return nullptr;
    uint8_t* prevHeader = (rawPrevFooter - TO_FOOTER(rawPrevFooter)->totalBlockSize) + FOOTER_SIZE;
    if (!IsValid(prevHeader)) return nullptr;
    return TO_HEADER(prevHeader);
}

TlsfAllocator::BlockHeader* TlsfAllocator::GetNextHeader(TlsfAllocator::BlockHeader* header)
{
    if (!header) return nullptr;
    uint8_t* rawHeader = TO_BYTES(header);
    uint8_t* next = rawHeader + GetHeaderSize(header);
    if (!IsValid(next)) return nullptr;
    return TO_HEADER(next);
}

TlsfAllocator::FreeBlockNode* TlsfAllocator::CreateFreeBlock(uint8_t* ptr,
                                                             TlsfAllocator::FreeBlockNode* prev,
                                                             TlsfAllocator::FreeBlockNode* next)
{
    if (!IsValid(ptr)) return nullptr;

    FreeBlockNode* node = TO_FREE_BLOCK(ptr);
    node->prev = prev;
    node->next = next;
    return node;
}

TlsfAllocator::FreeBlockNode* TlsfAllocator::GetFreeBlock(BlockHeader* header)
{
    if (!header || !IsFree(header)) return nullptr;
    uint8_t* rawBlock = TO_BYTES(header) + HEADER_SIZE;
    if (!IsValid(rawBlock)) return nullptr;
    return TO_FREE_BLOCK(rawBlock);
}

TlsfAllocator::FreeBlockNode* TlsfAllocator::GetFreeBlock(const uint64_t fl, const uint64_t sl)
{
    return freeList[fl * MAX_SL_BUCKETS + sl];
}

TlsfAllocator::BlockFooter* TlsfAllocator::GetFooter(TlsfAllocator::BlockHeader* header)
{
    if (!header) return nullptr;
    uint64_t size = GetHeaderSize(header);
    uint8_t* rawFooter = TO_BYTES(header) + (size - FOOTER_SIZE);
    if (!rawFooter || !IsValid(rawFooter)) return nullptr;
    return TO_FOOTER(rawFooter);
}

TlsfAllocator::BlockFooter* TlsfAllocator::GetPrevFooter(TlsfAllocator::BlockHeader* header)
{
    if (!header) return nullptr;
    uint8_t* rawFooter = TO_BYTES(header) - FOOTER_SIZE;
    if (!IsValid(rawFooter)) return nullptr;
    return TO_FOOTER(rawFooter);
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

    BlockFooter* prevFooter = GetPrevFooter(header);
    if (!prevFooter) return false;

    uint8_t* rawPrevHeader = raw - (prevFooter->totalBlockSize - FOOTER_SIZE);
    if (!rawPrevHeader || !IsValid(rawPrevHeader)) return false;

    return IsFree(TO_HEADER(rawPrevHeader));
}

bool TlsfAllocator::IsValid(uint8_t* ptr)
{
    return ptr && ptr >= data && ptr < (data + (capacity + HEADER_SIZE + FOOTER_SIZE));
}

const uint64_t TlsfAllocator::Capacity()
{
    return capacity;
}

const uint64_t TlsfAllocator::BytesRemaining()
{
    return bytesRemaining;
}

} // namespace Siege