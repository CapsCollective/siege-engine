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

#include <cstddef>
#include <cstdint>

#include "../Macros.h"

namespace Siege
{
template<typename T>
class TlsfAllocator
{
public:

    /**
     * Flags for determining the state of a memory block. Every header has a flag where the
     * first three bits are dedicated to keeping track of the block's state
     */
    enum HeaderFlags
    {
        FULL = 0,
        FREE = 1,
        PREV_IS_FREE = 2
    };

    /**
     * linked list nodes for representing free memory blocks
     */
    struct FreeBlockNode
    {
        FreeBlockNode* next {nullptr};
        FreeBlockNode* prev {nullptr};
    };

    /**
     * A struct representing the header for each memory block in the allocator. This struct
     * stores the size of the entire block alongside the state of the block
     */
    struct BlockHeader
    {
        T sizeAndFlags {0};
    };

    /**
     * a struct which stores the size of the the entire memory block. Used to navigate the
     * allocator and find the block's header
     */
    struct BlockFooter
    {
        T totalBlockSize {0};
    };

    // S'tructors

    /**
     * Empty constructor Initialises the constructor and size to 0 (null)
     */
    TlsfAllocator();

    /**
     * Size initialiser. Accepts a number representing the number of bytes to allocate to
     * the allocator's memory buffer
     * @param size the number of bytes to allocate, cannot be less than 16
     */
    TlsfAllocator(const T size);

    /**
     * Destructor, deallocates all memory and sets all values to zero
     */
    ~TlsfAllocator();

    // Deleted copy and move constructors

    TlsfAllocator(const TlsfAllocator& other) = delete;
    TlsfAllocator(const TlsfAllocator&& other) = delete;

    // Other functions

    /**
     * Checks if a block's preceding neighbour is free (the block that comes before it)
     * @param header the current header
     * @return true if the previous block is free, otherwise false
     */
    bool PrevBlockIsFree(BlockHeader* header) const;

    /**
     * Checks if a block is free for allocation
     * @param header the header to check for free space
     * @return true if the free flag is set, otherwise false
     */
    bool IsFree(BlockHeader* header) const;

    /**
     * Checks if an entry exists in the FreeList for a specific combination of first and
     * second level indices
     * @param firstLevel the index to search the first level bitmask with
     * @param secondLevel the index to search the second level bitmask with
     * @return true if a free block exists, false otherwise
     */
    bool IsFree(T firstLevel, T secondLevel) const;

    /**
     * Checks if the allocator is full
     * @return true if there are no bytes left in the allocator, otherwise false
     */
    bool IsFull() const;

    // Buffer manipulation Functions

    /**
     * Retrieves a free block from a header if possible
     * @param header the header from which to get a FreeBlock
     * @return a FreeBlock if applicable, otherwise a nullptr
     */
    FreeBlockNode* GetFreeBlock(BlockHeader* header) const;

    /**
     * Retrieves a free block using first and second level indices
     * @param firstLevel the first level index to be calculated
     * @param secondLevel the second level index to be calculated
     * @return the FreeBlock at the specified first and second level indices
     */
    FreeBlockNode* GetFreeBlock(const T firstLevel, const T secondLevel) const;

    /**
     * Retrieves the raw allocated data stored within the memory block
     * @param header the header to extrapolate the data from
     * @return a pointer to the data stored by the memory block
     */
    uint8_t* GetBlockData(BlockHeader* header) const;

    /**
     * Gets the footer for a memory block
     * @param header the header of the memory block
     * @return the footer associated with the memory block or nullptr if out of bounds
     */
    BlockFooter* GetFooter(BlockHeader* header) const;

    /**
     * Returns the footer of the previous memory block
     * @param header the current header
     * @return the previous footer or nullptr if it doesnt exist
     */
    BlockFooter* GetPrevFooter(BlockHeader* header) const;

    /**
     * Returns the header associated with a FreeBlock
     * @param node the FreeBlock to search
     * @return the BlockHeader associated with the free block
     */
    BlockHeader* GetHeader(FreeBlockNode* node) const;

    /**
     * Returns the header of a data pointer. Assumes that the data is pointing to the start
     * of the data pointer
     * @param ptr the raw data pointer
     * @return a BlockHeader if one exists
     */
    BlockHeader* GetHeader(uint8_t* ptr) const;

    /**
     * Returns the header of the previous data block if within the allocator's range
     * @param header the current header
     * @return the header of the previous block or nullptr if none exist
     */
    BlockHeader* GetPrevHeader(BlockHeader* header) const;

    /**
     * Returns the header of the next memory block if within the allocator's range
     * @param header the current header
     * @return the header of the next memory block or nullptr if out of bounds
     */
    BlockHeader* GetNextHeader(BlockHeader* header) const;

    // Allocate/Deallocate

    /**
     * Allocates a memory block and returns a pointer to the allocator's buffer
     * @param size the size in bytes to allocate. The minimum allocatable chunk of memory is 16
     * bytes
     * @return a pointer to the memory block
     */
    void* Allocate(const T& size);

    /**
     * Deallocates a memory block and returns the memory to the allocator's pool
     * @param ptr the pointer to deallocate
     */
    void Deallocate(void* ptr);

    // Getters

    /**
     * Returns the size of a memory block from its header
     * @param header the header of the memory block
     * @return the fully padded size of the memory block
     */
    const T GetHeaderSize(BlockHeader* header) const;

    /**
     * Returns the maximum capacity of the allocator
     * @return the allocator's capacity in bytes
     */
    const T Capacity() const;

    /**
     * Returns the number of unpadded bytes remaining
     * @return the number of bytes remaining in the allocator
     */
    const T BytesRemaining() const;

    /**
     * Returns the total bytes remaining with padding
     * @return the total bytes remaining with padding
     */
    const T TotalBytesRemaining() const
    {
        return totalBytesRemaining;
    }

    /**
     * Returns the total size of allocated memory by the allocator with padding
     * @return the total size of allocated memory by the allocator with padding
     */
    const T TotalSize() const
    {
        return totalSize;
    }

    /**
     * Returns the data buffer held by the allocator
     * @return the data buffer held by the allocator
     */
    const uint8_t* Data() const
    {
        return data;
    }

    /**
     * Returns the bitmask representing the first level of size classes
     * @return the bitmask representing the first level of size classes
     */
    const T FirstLevelBitmask() const
    {
        return firstLevelBitmask;
    }

    /**
     * Returns the second level bitmask stored by the allocator
     * @param firstLevel the first level index to search over
     * @return the value of the second level bitmask
     */
    const uint16_t& SecondLevelBitmask(const T firstLevel)
    {
        return secondLevelBitmasks[firstLevel];
    }

private:

    /**
     * Calculates the FreeList, first level, and second level indices for a given size
     * @param size the size class to search for in the allocator
     * @param firstLevel the first level index to be calculated
     * @param secondLevel the second level index to be calculated
     * @return the FreeList index
     */
    T CalculateFreeBlockIndices(T size, OUT T& firstLevel, OUT T& secondLevel);

    /**
     * Creates and initialises a header at a specified memory location
     * @param ptr the memory location, represented as a pointer to a set of bytes
     * @param size the size to allocate to the memory block. Size must have metadata sizes factored
     * into its calculation
     * @param flags The state flags for header (see HeaderFlags)
     */
    void CreateHeader(uint8_t* ptr, const T size, HeaderFlags flags);

    /**
     * Creates a footer at a specified memory location. Footers store the size of the entire
     * memory block (including metadata padding)
     * @param ptr the memory location to allocate the footer to (represented as a byte pointer)
     * @param size the size to assign the footer (should be the full block size)
     */
    void CreateFooter(uint8_t* ptr, const T size);

    /**
     * Creates a FreeBlockNode at a specified memory location and positions it within the
     * FreeList
     * @param ptr the memory location to initialise the block to
     * @param prev the FreeBlock that precedes this block
     * @param next the FreeBlock that follows this block
     * @return a pointer to the new FreeBlock
     */
    FreeBlockNode* CreateFreeBlock(uint8_t* ptr, FreeBlockNode* prev, FreeBlockNode* next);

    /**
     * Finds a free block for a specified size. If none exist, will attempt to find the next
     * available block
     * @param size the minimum size of the free block
     * @return a pointer to a FreeBlockNode if one exists, otherwise nullptr
     */
    FreeBlockNode* FindFreeBlock(const T& size);

    /**
     * Returns the next available free block (if any exist)
     * @param firstLevel the index to search the first level bitmask with
     * @param secondLevel the index to search the second level bitmask with
     * @return the index to the FreeList where the block exists
     */
    const T GetNextFreeSlotIndex(T& firstLevel, T& secondLevel);

    /**
     * Attempts to split a block in two, prioritising splitting the block into the requested size
     * first and allocating the remainder to a second block. If an attempted split results in too
     * few bytes remaining, the block will allocate the full free block
     * @param node the node to try and split
     * @param allocatedSize the size to allocate to the first split block
     * @return a BlockHeader representing the split block
     */
    BlockHeader* TrySplitBlock(FreeBlockNode* node, T& allocatedSize);

    /**
     * Removes a free block from the FreeList, unlinking it from its class linked list
     * @param node the node to remove
     * @return true if successfully removed, otherwise false
     */
    bool RemoveFreeBlock(FreeBlockNode* node);

    /**
     * Adds a new free block to the allocator
     * @param size the size in bytes of the new FreeBlock
     * @param currentNode the header of the FreeNode you wish to add
     */
    void AddNewBlock(const T size, BlockHeader* currentNode);

    /**
     * Attempts to combine a FreeBlock with its neighbours and updates the size of the header if
     * memory blocks were successfully combined
     * @param header the header to attempt to coalesce
     * @param size the size in bytes of the combined new memory block
     * @return the BlockHeader of the combined memory block (if combined)
     */
    BlockHeader* TryCoalesce(BlockHeader* header, OUT T& size);

    /**
     * Tests if a pointer is valid within the allocator
     * @param ptr the pointer to test
     * @return true if the pointer is valid, otherwise false
     */
    bool IsValid(uint8_t* ptr) const;

    static uint8_t MIN_SIZE_INDEX;

    T totalSize {0};
    T totalBytesRemaining {0};

    T capacity {0};
    T bytesRemaining {0};

    uint8_t* data {nullptr};
    FreeBlockNode** freeList {nullptr};

    // bitmasks
    T firstLevelBitmask {0};
    uint16_t* secondLevelBitmasks {nullptr};
};

/**
 * A TLSF allocator capable of storing up to 8KB of memory. Each allocation has a overhead of
 * 4 bytes and can only allocate a minimum of 16 bytes
 */
typedef TlsfAllocator<uint16_t> SmallTlsfAllocator;

/**
 * A TLSF allocator capable of storing up to 512MB of memory. Each allocation has a overhead of
 * 8 bytes and can only allocate a minimum of 16 bytes
 */
typedef TlsfAllocator<uint32_t> MediumTlsfAllocator;

/**
 * A TLSF allocator capable of storing up to 8GB of memory. Each allocation has a overhead of
 * 16 bytes and can only allocate a minimum of 16 bytes
 */
typedef TlsfAllocator<uint64_t> LargeTlsfAllocator;
} // namespace Siege

#endif // SIEGE_ENGINE_TLSF_H
