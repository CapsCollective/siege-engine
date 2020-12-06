#include "IndexAllocator.h"

IndexAllocator::IndexAllocator(size_t size) {
    entries = std::vector<IndexEntry>(size);
    freeEntries = std::vector<size_t>(size);
    currentEntities = 0;
}

GenerationalIndex IndexAllocator::allocateIndex() {
    GenerationalIndex generationalIndex;

    if (freeEntries.empty()) {
        size_t index = entries.size();
        entries[index] = {true, 0};

        // TODO: create generational index and return it.

    } else {
        size_t index = freeEntries.back();
        freeEntries.pop_back();
        
        entries[index].live = true;
        entries[index].generation++;

        generationalIndex = {index, entries[index].generation};

        currentEntities++;
    }

    return generationalIndex;
}
