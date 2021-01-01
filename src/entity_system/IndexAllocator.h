#ifndef A_DARK_DISCOMFORT_INDEXALLOCATOR_H
#define A_DARK_DISCOMFORT_INDEXALLOCATOR_H

#include <vector>
#include <cstdint>

// Utility Structs

struct IndexEntry 
{
    bool live = false;
    uint32_t generation = 0;
};

struct GenerationalIndex 
{
    size_t index = 0;
    // TODO generation is unused - why?
    uint32_t generation = 0;
};

class IndexAllocator 
{
public:

    // Constructors

    IndexAllocator() :
    entries(std::vector<IndexEntry>()),
    freeEntries(std::vector<size_t>()),
    currentEntities(0)
    {};

    ~IndexAllocator() = default;

    // Public Utility Methods

    GenerationalIndex AllocateIndex();

    bool IsLive(GenerationalIndex);

    void Deallocate(GenerationalIndex);

    IndexEntry& operator[] (GenerationalIndex);

private:

    // Private fields

    std::vector<IndexEntry> entries;

    std::vector<size_t> freeEntries;

    uint32_t currentEntities;
};


#endif //A_DARK_DISCOMFORT_INDEXALLOCATOR_H
