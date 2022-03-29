#include "EntitySystem.h"

#include <algorithm>
#include <cstdint>

#include "./Entity.h"

void EntitySystem::Add(Entity* entity)
{
    // If the pointer is null, stop the function
    if (!entity) return;

    // Generate an index and add it to the entity
    entity->SetIndex(allocator.AllocateIndex());

    // Queue the entity for initialisation
    registeredEntities.emplace_back(entity);
}

void EntitySystem::Add(const std::vector<Entity*>& newEntities)
{
    if (newEntities.empty()) return;

    size_t newEntityCount = newEntities.size();

    // Reserve vector space to avoid unnecessary resizing
    entities.reserve(entities.size() + newEntityCount);
    packedEntities.reserve(packedEntities.size() + newEntityCount);

    for (auto& entity : newEntities)
    {
        entity->SetIndex(allocator.AllocateIndex());
        registeredEntities.emplace_back(entity);
    }
}

void EntitySystem::RegisterEntities()
{
    if (registeredEntities.empty()) return;

    // Iterate over queued entities and initialise them
    for (auto& entity : registeredEntities)
    {
        // If the entity's given index already exists then override the existing entry
        if (entity->GetIndex().index < entities.size()) entities[entity->GetIndex().index] = entity;
        else entities.push_back(entity);

        packedEntities.push_back(entity);
        entity->OnStart();
    }

    // Once all entities are added, sort the packed storage
    SortStorage();

    // Remove all entities from the queue
    registeredEntities.clear();
}

void EntitySystem::SortStorage()
{
    // Sort packedEntities by Z index - lowest to highest
    std::sort(packedEntities.begin(), packedEntities.end(), [](Entity* a, Entity* b) {
        return a->GetZIndex() < b->GetZIndex();
    });
}

void EntitySystem::SortPartial(Entity* entity, int oldZIdx)
{
    // Get the index of the entity in packed storage
    int32_t index = GetEntityIndex(entity, packedEntities);

    if (index != -1)
    {
        // Check if the new z index is greater than the old z index. If it is, start sorting
        // from the entity's index.
        bool isGreater = entity->GetZIndex() > oldZIdx;
        auto begin = isGreater ? packedEntities.begin() + index : packedEntities.begin();
        auto end = isGreater ? packedEntities.end() : packedEntities.begin() + index;

        // Sort EntitySystem from either beginning -> entity index, or entity index -> end
        std::partial_sort(begin, end, packedEntities.end(), [](Entity* a, Entity* b) {
            return a->GetZIndex() < b->GetZIndex();
        });
    }
}

void EntitySystem::Remove(Entity* entity, std::vector<Entity*>& storage)
{
    if (!allowDeregistration) return;

    // Check if the entity's index is in bounds
    if (entity->GetIndex().index >= entities.size()) return;

    // De-allocate the entity's index
    allocator.Deallocate(entity->GetIndex());

    // Get the packed index
    int32_t index = GetEntityIndex(entity, storage);

    // Erase the packed index entry from packed entity storage if found
    if (index != -1) storage.erase(storage.begin() + index);

    // Delete the entity from the heap
    size_t entityIndex = entity->GetIndex().index;
    delete entities[entityIndex];
}

void EntitySystem::QueueFree(Entity* entity)
{
    if (!allowDeregistration) return;

    // Ensure that we have no duplicates in the freedEntities vector
    int32_t index = GetEntityIndex(entity, freedEntities);
    if (index == -1)
    {
        // Push the entity back into the queue
        freedEntities.push_back(entity);
    }
}

void EntitySystem::Reset()
{
    // Delete all entities that were queued for registration
    for (auto& entity : registeredEntities) delete entity;

    // Clear queuing storages
    registeredEntities.clear();
    freedEntities.clear();

    // Clear packedEntities, packedTools, and the allocator
    ClearStorage(packedEntities);
    allocator.Reset();
}

void EntitySystem::ClearStorage(std::vector<Entity*>& storage)
{
    if (!allowDeregistration) return;

    // append storage to the end of freedEntities
    freedEntities.insert(freedEntities.begin(), storage.begin(), storage.end());

    // Remove the entities from storage
    for (auto& e : freedEntities)
    {
        Remove(e, storage);
    }

    // Clear all pointers from freedEntities
    freedEntities.clear();
}

void EntitySystem::FreeEntities()
{
    if (!allowDeregistration) return;

    // Iterate over all entities that need to be freed
    for (auto& entity : freedEntities) Remove(entity, packedEntities);
    // Clear the storage.
    freedEntities.clear();
}

int32_t EntitySystem::GetEntityIndex(Entity* entity, const std::vector<Entity*>& storage)
{
    if (storage.empty()) return -1;

    size_t index = 0;
    int targetZIndex = entity->GetZIndex();
    bool found = storage[0] == entity;
    int32_t foundIndex = 0;

    while (!found && index < storage.size())
    {
        // Get the branches of the current node
        size_t left = (index * 2) + 1;
        size_t right = (index * 2) + 2;

        auto searchBranch = [&entity, &storage, targetZIndex](size_t branchIndex) -> size_t {
            // Check if the index is in bounds, otherwise return 0
            size_t index = (branchIndex < storage.size()) * branchIndex;
            // return the entity's index if it exists, otherwise return 0.
            return (storage[index]->GetZIndex() == targetZIndex) && (storage[index] == entity);
        };

        // Search each branch for the entity. If no entity is found, return 0.
        size_t isOnLeft = searchBranch(left);
        size_t isOnRight = searchBranch(right);

        // Check if either branch found the entity
        size_t branchResults = (isOnLeft * left) + (isOnRight * right);

        // If the entity was found, return the entity's index, otherwise return -1.
        foundIndex = (int) branchResults + ((branchResults == 0) * -1);

        found = (foundIndex != -1);

        index++;
    }
    return foundIndex;
}

bool EntitySystem::IsLive(const GenerationalIndex& index)
{
    return allocator.IsLive(index);
}

void EntitySystem::SetAllowDeregistration(bool canDeregister)
{
    allowDeregistration = canDeregister;
}
