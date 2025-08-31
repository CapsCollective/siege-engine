//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_TLSF_H
#define SIEGE_ENGINE_TLSF_H

#include <cstdint>

#include "../Macros.h"

#define FREE(ptr) Deallocate((void**) &ptr)

namespace Siege
{
template<typename T>
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

    struct BlockHeader
    {
        T sizeAndFlags {0};
    };

    struct BlockFooter
    {
        T totalBlockSize {0};
    };

    // S'tructors

    TlsfAllocator();
    TlsfAllocator(const T size);

    // Deleted copy and move constructors

    TlsfAllocator(const TlsfAllocator& other) = delete;
    TlsfAllocator(const TlsfAllocator&& other) = delete;

    // Other functions

    void CreateHeader(uint8_t* ptr, const T size, HeaderFlags flags);
    void CreateFooter(uint8_t* ptr, const T size);
    FreeBlockNode* CreateFreeBlock(uint8_t* ptr, FreeBlockNode* prev, FreeBlockNode* next);
    FreeBlockNode* FindFreeBlock(const T& size);

    const T GetNextFreeSlotIndex(T& fl, T& sl);
    bool IsFree(BlockHeader* header);
    bool IsFree(T fl, T sl);
    bool IsFull();
    bool PrevBlockIsFree(BlockHeader* header);
    T CalculateFreeBlockIndices(T size, OUT T& fl, OUT T& sl);

    // Buffer manipulation Functions

    FreeBlockNode* GetFreeBlock(BlockHeader* header);
    FreeBlockNode* GetFreeBlock(const T fl, const T sl);
    uint8_t* GetBlockData(BlockHeader* header);
    BlockFooter* GetFooter(BlockHeader* header);
    BlockFooter* GetPrevFooter(BlockHeader* header);
    BlockHeader* GetHeader(FreeBlockNode* node);
    BlockHeader* GetHeader(uint8_t* ptr);
    BlockHeader* GetPrevHeader(BlockHeader* header);
    BlockHeader* GetNextHeader(BlockHeader* header);

    // Allocate/Deallocate

    void* Allocate(const T& size);
    void Deallocate(void** ptr);

    BlockHeader* TrySplitBlock(FreeBlockNode* node, T& allocatedSize);
    bool RemoveFreeBlock(FreeBlockNode* node);
    void AddNewBlock(const T size, BlockHeader* currentNode);
    BlockHeader* TryCoalesce(BlockHeader* header, OUT T& size);

    // Getters

    bool IsValid(uint8_t* ptr);
    const T GetHeaderSize(BlockHeader* header);
    const T Capacity();
    const T BytesRemaining();
    const T TotalBytesRemaining()
    {
        return totalBytesRemaining;
    }
    const T TotalSize()
    {
        return totalSize;
    }
    const uint8_t* Data()
    {
        return data;
    }
    const T FlBitmask()
    {
        return flBitmask;
    }
    const uint16_t& SlBitmask(const T fl)
    {
        return slBitmasks[fl];
    }

private:

    static uint8_t MIN_SIZE_INDEX;

    T totalSize {0};
    T totalBytesRemaining {0};

    T capacity {0};
    T bytesRemaining {0};

    uint8_t* data {nullptr};
    FreeBlockNode** freeList {nullptr};

    // bitmasks
    T flBitmask {0};
    uint16_t* slBitmasks {nullptr};
};

// Maximum buffer possible here is around 8KB.
typedef TlsfAllocator<uint16_t> SmallTlsfAllocator;
// Maximum buffer possible here is around 512MB.
typedef TlsfAllocator<uint32_t> MediumTlsfAllocator;
// Maximum buffer possible here is around 2,147,483.6GB.
typedef TlsfAllocator<uint64_t> LargeTlsfAllocator;
} // namespace Siege

#endif // SIEGE_ENGINE_TLSF_H
