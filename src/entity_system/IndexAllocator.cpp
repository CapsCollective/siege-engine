#include "IndexAllocator.h"
#include <iostream>

IndexAllocator::IndexAllocator() {
    entries = std::vector<IndexEntry>();
    freeEntries = std::vector<size_t>();
    currentEntities = 0;
}

GenerationalIndex IndexAllocator::AllocateIndex() {
    size_t index = 0;
    uint32_t generation = 0;

    // If no free entried exist, add a new entity.
    if (freeEntries.empty()) {
        index = entries.size();
        entries.push_back({true, generation});
    } else {
        // If free entries exist - re-use a freed index
        index = freeEntries.back();
        generation = entries[index].generation;
        freeEntries.pop_back();
        entries[index] = {true, generation++};
    }
    // TODO: Remove once entity system is complete and tested
    std::cout << "Created new index at position: " << index << " with generation: " << generation << std::endl;
    currentEntities++;

    return {index, generation};
}

bool IndexAllocator::IsLive(GenerationalIndex index) {
    return entries[index.index].live;
}

void IndexAllocator::Deallocate(GenerationalIndex index) {
    // Set the index's liveness to false.
    freeEntries.push_back(index.index);
    entries[index.index].live = false;
    // TODO: Remove once entity system is complete and tested
    std::cout << "Removing index at position: " << index.index << " and generation: " << index.generation << std::endl;
}

IndexEntry& IndexAllocator::operator[] (GenerationalIndex index) {
    return entries[index.index];
}

IndexAllocator::~IndexAllocator() {

}
