//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_INDEXALLOCATOR_H
#define SIEGE_ENGINE_INDEXALLOCATOR_H

#include <utils/String.h>

#include <cstdint>
#include <vector>

namespace Siege
{
/**
 * A struct for allocator index entry info
 */
struct IndexEntry
{
    /**
     * The liveness of the entry
     */
    bool live;

    /**
     * The generation which the entry represents
     */
    uint32_t generation;
};

/**
 * A struct for handling generational indices
 */
struct GenerationalIndex
{
    /**
     * The represented index
     */
    size_t index;

    /**
     * The generation of the index
     */
    uint32_t generation;

    // Operator overloads

    /**
     * Equality operator overload for checking equality with another
     * @param rhs - the GenerationalIndex to compare with
     * @return true if the indices are equal, false otherwise
     */
    bool operator==(GenerationalIndex& rhs) const;

    // Public methods

    /**
     * Gets a simple String representation of the GenerationalIndex
     * @return a formatted String
     */
    String ToString() const;
};

class IndexAllocator
{
public:

    // 'Structors

    /**
     * Zero-param constructor for initialisation
     */
    IndexAllocator() = default;

    // Operator overloads

    /**
     * Indexing operator overload for obtaining reference to an IndexEntry by GenerationalIndex
     * @warning attempting to access an entry outside the storage's range has undefined results
     * @param index - the GenerationalIndex to index by
     * @return a reference to an IndexEntry
     */
    IndexEntry& operator[](GenerationalIndex index);

    // Public methods

    /**
     * Allocates a new GenerationalIndex from the storage
     * @return a GenerationalIndex to the newly allocated entry
     */
    GenerationalIndex AllocateIndex();

    /**
     * Checks whether a given GenerationalIndex is live or not
     * @param index - the GenerationalIndex to check
     * @return true if the GenerationalIndex is live, false otherwise
     */
    bool IsLive(GenerationalIndex index);

    /**
     * Frees the index of a given GenerationalIndex, killing it
     * @param index - the GenerationalIndex to deallocate
     */
    void Deallocate(GenerationalIndex index);

    /**
     * Empties and resets the allocator storages
     */
    void Reset();

private:

    // Private fields

    /**
     * The currently allocated IndexEntries
     */
    std::vector<IndexEntry> entries;

    /**
     * The indices of free IndexEntries
     */
    std::vector<size_t> freeEntries;
};

} // namespace Siege

#endif // SIEGE_ENGINE_INDEXALLOCATOR_H
