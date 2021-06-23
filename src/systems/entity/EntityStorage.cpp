#include "EntityStorage.h"
#include "Tool.h"
#include <cstdint>
#include <algorithm>

// Static member initialisations of basic fields
IndexAllocator EntityStorage::allocator = IndexAllocator();
std::vector<Entity*> EntityStorage::entities = std::vector<Entity*>();

// Static member initialisations of vectors for holding filtered entities
std::vector<Entity*> EntityStorage::packedEntities = std::vector<Entity*>();
std::vector<Entity*> EntityStorage::packedTools = std::vector<Entity*>(); 

// Static member initialisations of vectors for transformations
std::vector<Entity*> EntityStorage::freedEntities = std::vector<Entity*>();
std::vector<std::pair<Entity*, bool>> EntityStorage::registeredEntities = std::vector<std::pair<Entity*, bool>>();

void EntityStorage::Register(Entity* entity, bool isTool)
{
    // If the pointer is null, stop the function
    if (!entity) return;

    // Generate an index and add it to the entity
    entity->SetIndex(allocator.AllocateIndex());

    // Queue the entity for initialisation
    registeredEntities.emplace_back(std::make_pair(entity, isTool));
}

void EntityStorage::Register(const std::vector<Entity*>& newEntities, bool isTool) 
{
    if (newEntities.empty()) return;

    size_t newEntityCount = newEntities.size();

    std::vector<Entity*>& storage = isTool ? packedTools : packedEntities;

    // Reserve vector space to avoid unnecessary resizing
    entities.reserve(entities.size() + newEntityCount);
    storage.reserve(storage.size() + newEntityCount);

    for (auto& entity : newEntities) {
        entity->SetIndex(allocator.AllocateIndex());
        registeredEntities.emplace_back(std::make_pair(entity, isTool));
    }
}

void EntityStorage::AddEntity(Entity* entity)
{
    // Add entity straight to the entity storage
    AddToEntities(entity);

    // Add the entity to packed entities
    packedEntities.push_back(entity);
}

void EntityStorage::AddTool(Entity* entity)
{
     // Add tool straight to the entity storage
    AddToEntities(entity);

    // Add the entity to the end of the packed tools vector
    packedTools.push_back(entity);
}

void EntityStorage::AddToEntities(Entity* entity) {
    // If the entity's given index already exists then override the existing entry
    if (entity->GetIndex().index < entities.size()) entities[entity->GetIndex().index] = entity;
    else entities.push_back(entity);
}

void EntityStorage::RegisterEntities()
{
    // Iterate over queued entities and initialise them
    for (auto& entity : registeredEntities)
    {
        // Add the entity to appropriate storage
        if (entity.second) AddTool(entity.first); 
        else AddEntity(entity.first);
    }
    // Remove all entities from the queue
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

    // Erase the packed index entry from packed entity storage if found
    if (index != -1) packedEntities.erase(packedEntities.begin() + index);

    // Delete the entity from the heap
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
        // Push the entity back into the queue
        freedEntities.push_back(entity);
    }
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

uint32_t EntityStorage::GetEntityIndex(Entity *entity, std::vector<Entity *> &storage) {
    // Try find the entity, and return the index of the entity or -1 if not found
    auto it = std::find(storage.begin(), storage.end(), entity);
    return (it != storage.end()) ? std::distance(storage.begin(), it) : -1;
}
