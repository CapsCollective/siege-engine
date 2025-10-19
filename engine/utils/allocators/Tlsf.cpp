//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Tlsf.h"

#include <memory>

#include "../Logging.h"

#define TO_BYTES(val) reinterpret_cast<uint8_t*>(val)
#define TO_HEADER(val) reinterpret_cast<BlockHeader*>(val)
#define TO_FOOTER(val) reinterpret_cast<BlockFooter*>(val)
#define TO_FREE_BLOCK(val) reinterpret_cast<FreeBlockNode*>(val)

#define HEADER_SIZE sizeof(BlockHeader)
#define FOOTER_SIZE sizeof(BlockFooter)
#define FREE_BLOCK_SIZE sizeof(FreeBlockNode)
#define METADATA_OVERHEAD HEADER_SIZE + FOOTER_SIZE
#define PAD_SIZE(size) size + METADATA_OVERHEAD

#define MAX_SL_BUCKETS 16
#define FL(size) 63 - __builtin_clzll(size)
#define SL(size, firstLevel) (size >> firstLevel) & ((1 << MIN_SIZE_INDEX) - 1);
#define FLAG_OFFSET 3
#define INVALID_INDEX(T) std::numeric_limits<T>::max()
#define MIN_ALLOCATION_SIZE 16

namespace Siege
{
template<typename T>
uint8_t TlsfAllocator<T>::MIN_SIZE_INDEX = 4;

template<typename T>
TlsfAllocator<T>::TlsfAllocator()
{}

template<typename T>
TlsfAllocator<T>::TlsfAllocator(const T size)
{
    T paddedSize = PAD_SIZE(size);

    if (size == 0 || size < MIN_ALLOCATION_SIZE)
    {
        CC_LOG_ERROR("CONSTRUCTOR: Size must be greater than or equal to {}", MIN_ALLOCATION_SIZE);
        return;
    }

    if (size > (INVALID_INDEX(T) - METADATA_OVERHEAD) || paddedSize < size)
    {
        T max_size = INVALID_INDEX(T) - METADATA_OVERHEAD;
        CC_LOG_ERROR("CONSTRUCTOR: size: {} is above allowed allocator limit of {}",
                     size,
                     max_size);
        return;
    }

    T maxBuckets = (FL(size) - MIN_SIZE_INDEX) + 1;

    T slBucketSize = sizeof(uint16_t) * maxBuckets;
    T freeListSize = maxBuckets * MAX_SL_BUCKETS * sizeof(FreeBlockNode*);

    T allocSize = paddedSize + slBucketSize + freeListSize;

    totalSize = paddedSize;
    totalBytesRemaining = paddedSize;

    capacity = size;
    bytesRemaining = size;

    data = TO_BYTES(calloc(1, allocSize));

    CC_ASSERT(
        data,
        "Allocation returned null. This should not happen and implies an implementation failure.")

    freeList = (FreeBlockNode**) (data + paddedSize);
    secondLevelBitmasks = (uint16_t*) (data + paddedSize + freeListSize);

    CreateHeader(data, paddedSize, FREE);

    AddNewBlock(paddedSize, TO_HEADER(data));
}

template<typename T>
TlsfAllocator<T>::~TlsfAllocator()
{
    if (data) free(data);

    totalBytesRemaining = 0;
    capacity = 0;
    bytesRemaining = 0;
    totalSize = 0;

    firstLevelBitmask = 0;

    data = nullptr;
    freeList = nullptr;
    secondLevelBitmasks = nullptr;
}

template<typename T>
void TlsfAllocator<T>::CreateHeader(uint8_t* ptr, const T size, HeaderFlags flags)
{
    if (!ptr) return;
    BlockHeader* header = TO_HEADER(ptr);
    header->sizeAndFlags = (size << FLAG_OFFSET) | flags;
}

template<typename T>
void* TlsfAllocator<T>::Allocate(const T& size)
{
    T requiredSize = sizeof(BlockHeader) + size + sizeof(BlockFooter);

    if (!data)
    {
        CC_LOG_ERROR(
            "ALLOCATE: Allocator's buffer is a nullptr. This should not happen and indicates a bug")
        return nullptr;
    }

    if (capacity == 0 | size == 0)
    {
        CC_LOG_ERROR("ALLOCATE: Cannot allocate memory from an allocator with a capacity of 0")
        return nullptr;
    }

    if (requiredSize < size)
    {
        CC_LOG_ERROR("ALLOCATE: Requested size is too large and has caused an overflow")
        return nullptr;
    }

    if (requiredSize > totalBytesRemaining)
    {
        CC_LOG_ERROR("ALLOCATE: Not enough free space available for an allocation of size: {}",
                     size)
        return nullptr;
    }

    if (size < MIN_ALLOCATION_SIZE)
    {
        CC_LOG_ERROR(
            "ALLOCATE: Requested size is too small. Allocations must not be smaller than {} bytes",
            MIN_ALLOCATION_SIZE)
        return nullptr;
    }
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

template<typename T>
void TlsfAllocator<T>::Deallocate(void** ptr)
{
    uint8_t* raw = (uint8_t*) *ptr;
    if (!raw)
    {
        CC_LOG_ERROR("DEALLOCATE: Cannot deallocate nullptr. This may indicate a larger bug")
        return;
    }
    if (raw < data || raw >= (data + capacity))
    {
        CC_LOG_ERROR("DEALLOCATE: Provided pointer is out of the allocator's pointer range")
        return;
    }

    BlockHeader* header = GetHeader(raw);

    if (IsFree(header)) return;

    T blockSize = GetHeaderSize(header);

    T totalBlockSize = blockSize;
    header = TryCoalesce(header, totalBlockSize);
    BlockFooter* footer = GetFooter(header);

    header->sizeAndFlags = (totalBlockSize << 3) | (header->sizeAndFlags & PREV_IS_FREE) | FREE;
    footer->totalBlockSize = totalBlockSize;

    BlockHeader* nextHeader = GetNextHeader(header);

    if (nextHeader && ((uint8_t*) nextHeader < (data + capacity)))
    {
        nextHeader->sizeAndFlags |= PREV_IS_FREE;
    }

    AddNewBlock(totalBlockSize, header);

    bytesRemaining += blockSize - sizeof(BlockHeader) - sizeof(BlockFooter);
    totalBytesRemaining += blockSize;

    *ptr = nullptr;
}

template<typename T>
typename TlsfAllocator<T>::BlockHeader* TlsfAllocator<T>::TrySplitBlock(
    TlsfAllocator<T>::FreeBlockNode* node,
    T& allocatedSize)
{
    if (!node) return nullptr;

    T oldSize = GetHeaderSize(GetHeader(node));

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

template<typename T>
void TlsfAllocator<T>::AddNewBlock(const T size, BlockHeader* header)
{
    T firstLevel = 0, secondLevel = 0, index;
    index = CalculateFreeBlockIndices(size, firstLevel, secondLevel);

    CreateHeader(TO_BYTES(header), size, FREE);
    FreeBlockNode* node = CreateFreeBlock(TO_BYTES(GetFreeBlock(header)), nullptr, freeList[index]);
    CreateFooter(TO_BYTES(GetFooter(header)), size);

    if (node && node->next) node->next->prev = node;

    freeList[index] = node;
    firstLevelBitmask |= (1ULL << firstLevel);
    secondLevelBitmasks[firstLevel] |= (1 << secondLevel);
}

template<typename T>
typename TlsfAllocator<T>::BlockHeader* TlsfAllocator<T>::TryCoalesce(BlockHeader* header,
                                                                      OUT T& size)
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

template<typename T>
bool TlsfAllocator<T>::RemoveFreeBlock(TlsfAllocator::FreeBlockNode* node)
{
    BlockHeader* header = GetHeader(node);

    if (!header) return false;

    T oldSize = GetHeaderSize(header);

    T firstLevel, secondLevel, index;

    index = CalculateFreeBlockIndices(oldSize, firstLevel, secondLevel);

    if (!node->prev) freeList[index] = node->next;
    else node->prev->next = node->next;

    if (node->next) node->next->prev = node->prev;

    if (!freeList[index]) secondLevelBitmasks[firstLevel] &= ~(1 << secondLevel);
    if (!secondLevelBitmasks[firstLevel]) firstLevelBitmask &= ~(1ULL << firstLevel);

    node->next = nullptr;
    node->prev = nullptr;

    return true;
}

template<typename T>
typename TlsfAllocator<T>::FreeBlockNode* TlsfAllocator<T>::FindFreeBlock(const T& size)
{
    T firstLevel, secondLevel, index;
    index = CalculateFreeBlockIndices(size, firstLevel, secondLevel);

    if (!IsFree(firstLevel, secondLevel)) index = GetNextFreeSlotIndex(firstLevel, secondLevel);
    if (index == INVALID_INDEX(T)) return nullptr;

    return freeList[index];
}

template<typename T>
const T TlsfAllocator<T>::GetNextFreeSlotIndex(T& firstLevel, T& secondLevel)
{
    secondLevel =
        __builtin_ctz(secondLevelBitmasks[firstLevel] & ~(((1 << (secondLevel + 1)) - 1)));

    if (secondLevel == 32) secondLevel = 0;
    if (secondLevel) return firstLevel * MAX_SL_BUCKETS + secondLevel;

    firstLevel = firstLevelBitmask & ~(((1ULL << (firstLevel + 1)) - 1));

    if (!firstLevel) return INVALID_INDEX(T);

    firstLevel = __builtin_ctzll(firstLevel);
    CC_ASSERT(secondLevelBitmasks[firstLevel] > 0,
              "SlBitmasks is returning 0. This should not be happening and indicates an "
              "implementation error.")

    secondLevel = __builtin_ctz(secondLevelBitmasks[firstLevel]);

    return firstLevel * MAX_SL_BUCKETS + secondLevel;
}

template<typename T>
T TlsfAllocator<T>::CalculateFreeBlockIndices(T size, OUT T& firstLevel, OUT T& secondLevel)
{
    T rawFl = FL(size);
    firstLevel = rawFl - MIN_SIZE_INDEX;
    secondLevel = SL(size, firstLevel);
    return firstLevel * MAX_SL_BUCKETS + secondLevel;
}

template<typename T>
void TlsfAllocator<T>::CreateFooter(uint8_t* ptr, const T size)
{
    if (!ptr) return;
    BlockFooter* footer = TO_FOOTER(ptr);
    footer->totalBlockSize = size;
}

template<typename T>
typename TlsfAllocator<T>::BlockHeader* TlsfAllocator<T>::GetHeader(
    TlsfAllocator::FreeBlockNode* node) const
{
    if (!node) return nullptr;
    uint8_t* rawHeader = TO_BYTES(node) - HEADER_SIZE;
    if (!IsValid(rawHeader)) return nullptr;
    return TO_HEADER(rawHeader);
}

template<typename T>
typename TlsfAllocator<T>::BlockHeader* TlsfAllocator<T>::GetHeader(uint8_t* ptr) const
{
    if (!ptr) return nullptr;
    uint8_t* rawHeader = ptr - HEADER_SIZE;
    if (!IsValid(rawHeader)) return nullptr;
    return TO_HEADER(rawHeader);
}

template<typename T>
typename TlsfAllocator<T>::BlockHeader* TlsfAllocator<T>::GetPrevHeader(
    TlsfAllocator::BlockHeader* header) const
{
    if (!header) return nullptr;
    uint8_t* rawPrevFooter = TO_BYTES(GetPrevFooter(header));
    if (!IsValid(rawPrevFooter)) return nullptr;
    uint8_t* prevHeader = (rawPrevFooter - TO_FOOTER(rawPrevFooter)->totalBlockSize) + FOOTER_SIZE;
    if (!IsValid(prevHeader)) return nullptr;
    return TO_HEADER(prevHeader);
}

template<typename T>
typename TlsfAllocator<T>::BlockHeader* TlsfAllocator<T>::GetNextHeader(
    TlsfAllocator::BlockHeader* header) const
{
    if (!header) return nullptr;
    uint8_t* rawHeader = TO_BYTES(header);
    uint8_t* next = rawHeader + GetHeaderSize(header);
    if (!IsValid(next)) return nullptr;
    return TO_HEADER(next);
}

template<typename T>
typename TlsfAllocator<T>::FreeBlockNode* TlsfAllocator<T>::CreateFreeBlock(
    uint8_t* ptr,
    TlsfAllocator::FreeBlockNode* prev,
    TlsfAllocator::FreeBlockNode* next)
{
    if (!IsValid(ptr)) return nullptr;

    FreeBlockNode* node = TO_FREE_BLOCK(ptr);
    node->prev = prev;
    node->next = next;
    return node;
}

template<typename T>
typename TlsfAllocator<T>::FreeBlockNode* TlsfAllocator<T>::GetFreeBlock(BlockHeader* header) const
{
    if (!header || !IsFree(header)) return nullptr;
    uint8_t* rawBlock = TO_BYTES(header) + HEADER_SIZE;
    if (!IsValid(rawBlock)) return nullptr;
    return TO_FREE_BLOCK(rawBlock);
}

template<typename T>
typename TlsfAllocator<T>::FreeBlockNode* TlsfAllocator<T>::GetFreeBlock(const T firstLevel,
                                                                         const T secondLevel) const
{
    return freeList[firstLevel * MAX_SL_BUCKETS + secondLevel];
}

template<typename T>
typename TlsfAllocator<T>::BlockFooter* TlsfAllocator<T>::GetFooter(
    TlsfAllocator::BlockHeader* header) const
{
    if (!header) return nullptr;
    T size = GetHeaderSize(header);
    uint8_t* rawFooter = TO_BYTES(header) + (size - FOOTER_SIZE);
    if (!rawFooter || !IsValid(rawFooter)) return nullptr;
    return TO_FOOTER(rawFooter);
}

template<typename T>
typename TlsfAllocator<T>::BlockFooter* TlsfAllocator<T>::GetPrevFooter(
    TlsfAllocator::BlockHeader* header) const
{
    if (!header) return nullptr;
    uint8_t* rawFooter = TO_BYTES(header) - FOOTER_SIZE;
    if (!IsValid(rawFooter)) return nullptr;
    return TO_FOOTER(rawFooter);
}

template<typename T>
uint8_t* TlsfAllocator<T>::GetBlockData(TlsfAllocator::BlockHeader* header) const
{
    return TO_BYTES(header) + HEADER_SIZE;
}

template<typename T>
const T TlsfAllocator<T>::GetHeaderSize(BlockHeader* header) const
{
    return header->sizeAndFlags >> FLAG_OFFSET;
}

template<typename T>
bool TlsfAllocator<T>::IsFree(BlockHeader* header) const
{
    return header->sizeAndFlags & FREE;
}

template<typename T>
bool TlsfAllocator<T>::IsFree(T firstLevel, T secondLevel) const
{
    return secondLevelBitmasks[firstLevel] & (1 << secondLevel);
}

template<typename T>
bool TlsfAllocator<T>::IsFull() const
{
    return totalBytesRemaining == 0;
}

template<typename T>
bool TlsfAllocator<T>::PrevBlockIsFree(BlockHeader* header) const
{
    uint8_t* raw = TO_BYTES(header);
    if (raw == data) return false;

    BlockFooter* prevFooter = GetPrevFooter(header);
    if (!prevFooter) return false;

    uint8_t* rawPrevHeader = raw - (prevFooter->totalBlockSize - FOOTER_SIZE);
    if (!rawPrevHeader || !IsValid(rawPrevHeader)) return false;

    return IsFree(TO_HEADER(rawPrevHeader));
}

template<typename T>
bool TlsfAllocator<T>::IsValid(uint8_t* ptr) const
{
    return ptr && ptr >= data && ptr < (data + (capacity + HEADER_SIZE + FOOTER_SIZE));
}

template<typename T>
const T TlsfAllocator<T>::Capacity() const
{
    return capacity;
}

template<typename T>
const T TlsfAllocator<T>::BytesRemaining() const
{
    return bytesRemaining;
}

template class TlsfAllocator<uint16_t>;
template class TlsfAllocator<uint32_t>;
template class TlsfAllocator<uint64_t>;
} // namespace Siege
