//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "IndexAllocator.h"

namespace Siege
{
bool GenerationalIndex::operator==(GenerationalIndex& rhs) const
{
    return this->index == rhs.index && this->generation == rhs.generation;
}

String GenerationalIndex::ToString() const
{
    return String("%d:%d").Formatted(generation, index);
}

IndexEntry& IndexAllocator::operator[](GenerationalIndex index)
{
    return entries[index.index];
}

GenerationalIndex IndexAllocator::AllocateIndex()
{
    size_t index;
    uint32_t generation = 0;

    // If no free entity exist, add a new entity
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
        entries[index] = {true, ++generation};
    }

    return {index, generation};
}

bool IndexAllocator::IsLive(GenerationalIndex index)
{
    IndexEntry entry = entries[index.index];
    return entry.live && entry.generation == index.generation;
}

void IndexAllocator::Deallocate(GenerationalIndex index)
{
    // Set the index's live-ness to false.
    freeEntries.push_back(index.index);
    entries[index.index].live = false;
}

void IndexAllocator::Reset()
{
    entries.clear();
    freeEntries.clear();
}
} // namespace Siege