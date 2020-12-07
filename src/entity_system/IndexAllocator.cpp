#include "IndexAllocator.h"
#include <iostream>

IndexAllocator::IndexAllocator() {
    entries = std::vector<IndexEntry>();
    freeEntries = std::vector<size_t>();
    currentEntities = 0;
}

GenerationalIndex IndexAllocator::allocateIndex() {
    size_t index = 0;
    uint32_t generation = 0;

    if (freeEntries.empty()) {
        index = entries.size();
        entries.push_back({true, generation});
    } else {
        index = freeEntries.back();
        generation = entries[index].generation;
        freeEntries.pop_back();
        entries[index] = {true, generation++};
    }
    std::cout << "Created new index at position: " << index << " with generation: " << generation << std::endl;
    currentEntities++;

    return {index, generation};
}

bool IndexAllocator::isLive(GenerationalIndex index) {
    return entries[index.index].live;
}

void IndexAllocator::deallocate(GenerationalIndex index) {
    freeEntries.push_back(index.index);
    entries[index.index].live = false;
    std::cout << "Removing index at position: " << index.index << " and generation: " << index.generation << std::endl;
}

IndexEntry& IndexAllocator::operator[] (GenerationalIndex index) {
    return entries[index.index];
}
