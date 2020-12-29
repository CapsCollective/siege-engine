#include "EntityStorage.h"
#include <cstdint>
#include <algorithm>

// Static member initialisations

std::vector<Entity*> EntityStorage::entities = std::vector<Entity*>();

IndexAllocator EntityStorage::allocator = IndexAllocator();

std::vector<Entity*> EntityStorage::packedEntities = std::vector<Entity*>();

std::vector<Entity*> EntityStorage::freedEntities = std::vector<Entity*>();

std::vector<Entity*> EntityStorage::registeredEntities = std::vector<Entity*>();

// Functions

void EntityStorage::Register(Entity* entity)
{
    // If the pointer is null - stop the function.
    if (!entity) return;

    // Generate an index and add it to the entity
    GenerationalIndex index = allocator.AllocateIndex();
    entity->SetIndex(index);

    // Queue the entity for initialisation.
    registeredEntities.push_back(entity);
}

// Adds an entity to the Entity Storage.
void EntityStorage::AddEntity(Entity* entity)
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

    // Add the entity to the end of our packed entities.
    packedEntities.push_back(entity);
}

void EntityStorage::RegisterEntities() {
    // Iterate over our queued entities and initialise them
    for (auto& entity : registeredEntities) {
        // Add the entity to storage
        AddEntity(entity);
    }
    // Finally, remove all entities from the queue.
    registeredEntities.clear();
}

void EntityStorage::Remove(Entity* entity) 
{
    // Check if the entity's index is in bounds
    if (entity->GetIndex().index < entities.size()) 
    {
        // De-allocate the entity's index
        allocator.Deallocate(entity->GetIndex());

        // Get the packed index
        int32_t index = GetEntityIndex(entity, packedEntities);

        if (index != -1) 
        {
            // Erase the packed index entry from our packed entity storage
            packedEntities.erase(packedEntities.begin() + index);
        }
        // Finally, delete the entity from the heap
        size_t entityIndex = entity->GetIndex().index;
        delete entities[entityIndex];
    }
}

Entity* EntityStorage::operator[](GenerationalIndex index) 
{
    // Make sure the provided index is actually in use
    if (allocator.IsLive(index)) 
    {
        // Return the entity
        return entities[index.index];
    } 
    else 
    {
        // Return a nullpointer
        return nullptr;
    }
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
    // Iterate over the given vector and check if the element exists, if it does, return the index
    auto it = std::find(entityStorage.begin(), entityStorage.end(), entity);
    int32_t index = -1;
    if (it != entityStorage.end()) 
    {
        // Get the index of the entity (if found)
        index = std::distance(entityStorage.begin(), it);
    }
    // Return the index
    return index;
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