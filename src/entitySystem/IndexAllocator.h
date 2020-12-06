#ifndef A_DARK_DISCOMFORT_INDEXALLOCATOR_H
#define A_DARK_DISCOMFORT_INDEXALLOCATOR_H

#include <vector>

struct IndexEntry {
    bool live = false;
    uint32_t generation = 0;
};

struct GenerationalIndex {
    size_t index = 0;
    uint32_t generation = 0;
};

class IndexAllocator {

public:
    IndexAllocator(size_t = 256);
    GenerationalIndex allocateIndex();
private:
    std::vector<IndexEntry> entries;
    std::vector<size_t> freeEntries;
    uint32_t currentEntities;
};


#endif //A_DARK_DISCOMFORT_INDEXALLOCATOR_H
