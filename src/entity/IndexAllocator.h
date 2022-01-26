#ifndef A_DARK_DISCOMFORT_INDEXALLOCATOR_H
#define A_DARK_DISCOMFORT_INDEXALLOCATOR_H

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <vector>

#include "../utils/String.h"

// Utility Structs
// TODO document this class

struct IndexEntry
{
    bool live = false;
    uint32_t generation = 0;
};

struct GenerationalIndex
{
    size_t index = 0;
    uint32_t generation = 0;

    String ToString() const
    {
        std::stringstream ss;
        ss << generation << ":" << index;
        return ss.str().c_str();
    }
};

bool operator==(GenerationalIndex&, GenerationalIndex&);

class IndexAllocator
{
public:

    // 'Structors

    IndexAllocator() = default;

    ~IndexAllocator() = default;

    // Public methods

    GenerationalIndex AllocateIndex();

    bool IsLive(GenerationalIndex);

    void Deallocate(GenerationalIndex);

    IndexEntry& operator[](GenerationalIndex);

private:

    // Private fields

    std::vector<IndexEntry> entries;

    std::vector<size_t> freeEntries;

    uint32_t currentEntities = 0;
};

#endif // A_DARK_DISCOMFORT_INDEXALLOCATOR_H
