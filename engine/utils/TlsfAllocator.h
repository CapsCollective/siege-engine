//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_TLSFALLOCATOR_H
#define SIEGE_ENGINE_TLSFALLOCATOR_H

#include <cstdint>
#include "Macros.h"

namespace Siege
{

class TlsfAllocator
{
public:
    enum HeaderFlags
    {
        FULL = 0,
        FREE = 1,
        PREV_IS_FREE = 2
    };

    struct FreeBlockNode
    {
        FreeBlockNode* next {nullptr};
        FreeBlockNode* prev {nullptr};
    };

    struct BlockHeader {
        uint32_t sizeAndFlags {0};
    };

    struct BlockFooter
    {
        uint32_t totalBlockSize {0};
    };

    // S'tructors

    TlsfAllocator();
    TlsfAllocator(const uint64_t size);

    // Deleted copy and move constructors

    TlsfAllocator(const TlsfAllocator& other) = delete;
    TlsfAllocator(const TlsfAllocator&& other) = delete;

    // Other functions

    void CreateHeader(uint8_t* ptr, const uint64_t size, HeaderFlags flags);
    void CreateFooter(uint8_t* ptr, const uint64_t size);
    FreeBlockNode* CreateFreeBlock(uint8_t* ptr, FreeBlockNode* prev, FreeBlockNode* next);
    FreeBlockNode* FindFreeBlock(const uint64_t& size);

    const uint64_t GetNextFreeSlotIndex(uint64_t& fl, uint64_t& sl);
    bool IsFree(BlockHeader* header);
    bool IsFree(uint64_t fl, uint64_t sl);
    bool PrevBlockIsFree(BlockHeader* header);
    uint64_t CalculateFreeBlockIndices(uint64_t size, OUT uint64_t & fl, OUT uint64_t & sl);

    // Buffer manipulation Functions

    FreeBlockNode* GetFreeBlock(BlockHeader* header);
    FreeBlockNode* GetFreeBlock(const uint64_t fl, const uint64_t sl);
    uint8_t* GetBlockData(BlockHeader* header);
    BlockFooter* GetFooter(BlockHeader* header);
    BlockFooter* GetPrevFooter(BlockHeader* header);
    BlockHeader* GetHeader(FreeBlockNode* node);
    BlockHeader* GetHeader(uint8_t* ptr);
    BlockHeader* GetPrevHeader(BlockHeader* header);
    BlockHeader* GetNextHeader(BlockHeader* header);

    // Allocate/Deallocate

    void* Allocate(const uint64_t& size);

    template<typename T>
    void Deallocate(T*& ptr)
    {
        uint8_t* raw = (uint8_t*)ptr;
        if (!raw) return;
        if (raw < data || raw >= (data + capacity)) return;

        BlockHeader* header = GetHeader(raw);

        if (IsFree(header)) return;

        uint64_t blockSize = GetHeaderSize(header);

        uint64_t totalBlockSize = blockSize;
        header = TryCoalesce(header, totalBlockSize);
        BlockFooter* footer = GetFooter(header);

        header->sizeAndFlags = (totalBlockSize << 3) | (header->sizeAndFlags & PREV_IS_FREE) | FREE;
        footer->totalBlockSize = totalBlockSize;

        BlockHeader* nextHeader = GetNextHeader(header);

        if (nextHeader && ((uint8_t*)nextHeader < (data + capacity)))
        {
            nextHeader->sizeAndFlags |= PREV_IS_FREE;
        }

        AddNewBlock(totalBlockSize, header);

        bytesRemaining += blockSize - sizeof(BlockHeader) - sizeof(BlockFooter);
        totalBytesRemaining += blockSize;

        ptr = nullptr;
    }
    BlockHeader* TrySplitBlock(FreeBlockNode* node, uint64_t& allocatedSize);
    bool RemoveFreeBlock(FreeBlockNode* node);
    void AddNewBlock(const uint64_t size, BlockHeader* currentNode);
    BlockHeader* TryCoalesce(BlockHeader* header, OUT uint64_t& size);

    // Getters

    bool IsValid(uint8_t* ptr);
    const uint64_t GetHeaderSize(BlockHeader* header);
    const uint64_t Capacity();
    const uint64_t BytesRemaining();
    const uint64_t TotalBytesRemaining() { return totalBytesRemaining; }
    const uint64_t TotalSize() { return totalSize; }
    const uint8_t* Data() { return data; }
    const uint64_t FlBitmask() { return flBitmask; }
    const uint16_t& SlBitmask(const uint64_t fl) { return slBitmasks[fl]; }
private:
    uint64_t totalSize {0};
    uint64_t totalBytesRemaining {0};

    uint64_t capacity {0};
    uint64_t bytesRemaining {0};

    uint8_t* data {nullptr};
    FreeBlockNode** freeList {nullptr};

    // bitmasks
    uint64_t flBitmask {0};
    uint16_t* slBitmasks {nullptr};
};

} // namespace Siege

#endif // SIEGE_ENGINE_TLSFALLOCATOR_H
