#include "IndexAllocator.h"

bool operator==(GenerationalIndex& indexA, GenerationalIndex& indexB) {
    return indexA.index == indexB.index && indexA.generation == indexB.generation;
}

GenerationalIndex IndexAllocator::AllocateIndex() 
{
    size_t index;
    uint32_t generation = 0;

    // If no free entity exist, add a new entity.
    if (freeEntries.empty()) 
    {
        index = entries.size();
        entries.push_back({true, generation});
    } 
    else 
    {
        // If free entries exist - re-use a freed index
        index = freeEntries[0];
        generation = entries[index].generation;
        freeEntries.erase(freeEntries.begin());
        entries[index] = {true, generation++};
    }
    currentEntities++;

    return {index, generation};
}

bool IndexAllocator::IsLive(GenerationalIndex index) 
{
    return entries[index.index].live;
}

void IndexAllocator::Deallocate(GenerationalIndex index) 
{
    // Set the index's live-ness to false.
    freeEntries.push_back(index.index);
    entries[index.index].live = false;
}

IndexEntry& IndexAllocator::operator[] (GenerationalIndex index) 
{
    return entries[index.index];
}
