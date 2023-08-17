//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "EntitySystem.h"

#include <utils/Logging.h>

#include <algorithm>
#include <cassert>
#include <cstdint>

#include "./Entity.h"

namespace Siege
{
int32_t GetEntityIndex(Entity* entity, const std::vector<Entity*>& storage)
{
    if (storage.empty()) return -1;

    size_t index = 0;
    int targetZIndex = entity->GetZIndex();
    bool found = storage[0] == entity;
    int32_t foundIndex = 0;

    // Return the index of a given element within a given entity vector
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

bool EntitySystem::IsLive(Entity* entity)
{
    EntitySystem* system = FindInGlobalRegister(entity);
    if (system) return system->IsLive(entity->GetIndex());
    CC_LOG_WARNING("Could not find storage for provided entity");
    return false;
}

void EntitySystem::QueueFree(Entity* entity)
{
    EntitySystem* system = FindInGlobalRegister(entity);
    if (system)
    {
        CC_LOG_INFO("Freeing {} at ({})", entity->GetName(), entity->GetIndex().ToString());
        system->AddToFreeQueue(entity);
    }
    else
    {
        CC_LOG_WARNING("Could not find storage for {} at {}",
                       entity->GetName(),
                       entity->GetIndex().ToString());
    }
}

void EntitySystem::Resort(Entity* entity, int oldZIdx)
{
    EntitySystem* system = FindInGlobalRegister(entity);
    if (system) system->SortPartial(entity, oldZIdx);
    else
    {
        CC_LOG_WARNING("Could not find storage for {} at {}",
                       entity->GetName(),
                       entity->GetIndex().ToString());
    }
}

std::map<const Entity*, EntitySystem*> EntitySystem::globalEntityRegister;

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

const std::vector<Entity*>& EntitySystem::GetEntities()
{
    return packedEntities;
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
        CC_LOG_INFO("Registered {} at ({})", entity->GetName(), entity->GetIndex().ToString());

        packedEntities.push_back(entity);
        AddToGlobalRegister(entity, this);
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
    // Deregistration should always be allowed at this point
    assert(allowDeregistration);

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

void EntitySystem::AddToFreeQueue(Entity* entity)
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
    RemoveFromGlobalRegister(this);

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
    for (auto& entity : freedEntities)
    {
        RemoveFromGlobalRegister(entity);
        Remove(entity, packedEntities);
    }
    // Clear the storage.
    freedEntities.clear();
}

bool EntitySystem::IsLive(const GenerationalIndex& index)
{
    return allocator.IsLive(index);
}

void EntitySystem::SetAllowDeregistration(bool canDeregister)
{
    allowDeregistration = canDeregister;
}

void EntitySystem::AddToGlobalRegister(Entity* entity, EntitySystem* system)
{
    auto it = globalEntityRegister.find(entity);
    if (it == globalEntityRegister.end())
    {
        globalEntityRegister[entity] = system;
    }
}

void EntitySystem::RemoveFromGlobalRegister(Entity* entity)
{
    auto it = globalEntityRegister.find(entity);
    if (it != globalEntityRegister.end())
    {
        globalEntityRegister.erase(it);
    }
}

void EntitySystem::RemoveFromGlobalRegister(EntitySystem* system)
{
    for (auto it = globalEntityRegister.begin(); it != globalEntityRegister.end();)
    {
        if (it->second == system) it = globalEntityRegister.erase(it);
        else ++it;
    }
}

EntitySystem* EntitySystem::FindInGlobalRegister(Entity* entity)
{
    auto it = globalEntityRegister.find(entity);
    return it != globalEntityRegister.end() ? it->second : nullptr;
}

void EntitySystem::ResetGlobalRegister()
{
    globalEntityRegister.clear();
}
} // namespace Siege