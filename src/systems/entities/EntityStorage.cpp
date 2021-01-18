#include "EntityStorage.h"
#include <cstdint>
#include <algorithm>

// Static member initialisations of basic fields
IndexAllocator EntityStorage::allocator = IndexAllocator();
std::vector<Entity*> EntityStorage::entities = std::vector<Entity*>();

// Static member initialisations of vectors for holding filtered entities
std::vector<Entity*> EntityStorage::packedEntities = std::vector<Entity*>();
std::vector<Entity*> EntityStorage::packedTools = std::vector<Entity*>();
std::vector<Entity*> EntityStorage::allPackedEntities = std::vector<Entity*>();

// Static member initialisations of vectors for transformations
std::vector<Entity*> EntityStorage::freedEntities = std::vector<Entity*>();
ENTITY_LIST EntityStorage::registeredEntities = std::vector<std::pair<Entity*, bool>>();

void EntityStorage::Register(Entity* entity, bool isTool)
{
    // If the pointer is null - stop the function
    if (!entity) return;

    // Generate an index and add it to the entity
    GenerationalIndex index = allocator.AllocateIndex();
    entity->SetIndex(index);

    // Queue the entity for initialisation
    registeredEntities.emplace_back(entity, isTool);
}

// Adds an entity to the Entity Storage
void EntityStorage::AddEntity(Entity* entity, bool isTool)
{
    // If the entity's given index already exists
    if (entity->GetIndex().index < entities.size())
    {
        // override the existing entry
        entities[entity->GetIndex().index] = entity;
    }
    else
    {
        // Add the new entity to the end of the list
        entities.push_back(entity);
    }

    // Add the entity to the end of its appropriate packed entities
    if (isTool) packedTools.push_back(entity);
    else packedEntities.push_back(entity);

    // Add the entity to the end of all packed entities
    allPackedEntities.emplace_back(entity);
}

void EntityStorage::RegisterEntities() {
    // Iterate over our queued entities and initialise them
    for (auto& entity : registeredEntities) {
        // Add the entity to storage
        AddEntity(entity.first, entity.second);
    }
    // Finally, remove all entities from the queue
    registeredEntities.clear();
}

void EntityStorage::Remove(Entity* entity) 
{
    // Check if the entity's index is in bounds
    if (entity->GetIndex().index >= entities.size()) return;

    // De-allocate the entity's index
    allocator.Deallocate(entity->GetIndex());

    // Get the packed index
    int32_t index = GetEntityIndex(entity, packedEntities);
    int32_t generalIndex = GetEntityIndex(entity, allPackedEntities);

    // Check if we found the appropriate index in our tool or entity storage
    if (index != -1)
    {
        // Erase the packed index entry from our packed entity storage
        packedEntities.erase(packedEntities.begin() + index);
    }

    // Check if we found an index in our general storage of all entities
    if (generalIndex != -1)
    {
        // Erase the packed index entry from our container that stores all entities
        allPackedEntities.erase(allPackedEntities.begin() + generalIndex);
    }

    // Finally, delete the entity from the heap
    size_t entityIndex = entity->GetIndex().index;
    delete entities[entityIndex];
}

Entity* EntityStorage::operator[](GenerationalIndex index) 
{
    // Make sure the provided index is actually in use
    if (allocator.IsLive(index)) 
    {
        // Return the entity
        return entities[index.index];
    } 
    else return nullptr;
}

void EntityStorage::QueueFree(Entity* entity) 
{
    // Ensure that we have no duplicates in the freedEntities vector
    int32_t index = GetEntityIndex(entity, freedEntities);
    if (index == -1) 
    {
        // Push the entity back into our queue
        freedEntities.push_back(entity);
    }
}

uint32_t EntityStorage::GetEntityIndex(Entity* entity, std::vector<Entity*>& entityStorage) 
{
    // Try find the entity, and return the index of the entity or -1 if not found
    auto it = std::find(entityStorage.begin(), entityStorage.end(), entity);
    return (it != entityStorage.end()) ? std::distance(entityStorage.begin(), it) : -1;
}

void EntityStorage::FreeEntities() 
{
    // Iterate over all entities that need to be freed
    for (auto& entity : freedEntities)
    {
        // Remove them and free their memory
        Remove(entity);
    }
    // Clear the storage.
    freedEntities.clear();
}