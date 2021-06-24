#include "EntityStorage.h"
#include "./Entity.h"
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

void EntityStorage::Add(Entity* entity, bool isTool)
{
    // If the pointer is null, stop the function
    if (!entity) return;

    // Generate an index and add it to the entity
    entity->SetIndex(allocator.AllocateIndex());

    // Queue the entity for initialisation
    registeredEntities.emplace_back(std::make_pair(entity, isTool));
}

void EntityStorage::Add(const std::vector<Entity*>& newEntities, bool isTool) 
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

void EntityStorage::RegisterEntities()
{
    if (registeredEntities.empty()) return;
    // Iterate over queued entities and initialise them
    for (auto& registrationData : registeredEntities)
    {
        auto& entity = registrationData.first;
        
        // If the entity's given index already exists then override the existing entry
        if (entity->GetIndex().index < entities.size()) entities[entity->GetIndex().index] = entity;
        else entities.push_back(entity);

        // Add the entity to appropriate storage
        if (registrationData.second) packedTools.push_back(entity);
        else packedEntities.push_back(entity);
    }

    // Once all entities are added, sort the packed index
    SortByZIndex();

    // Remove all entities from the queue
    registeredEntities.clear();
}

void EntityStorage::SortByZIndex() {
    // Sort packedEntities by Z index - lowest to highest
    std::sort(packedEntities.begin(), packedEntities.end(), [](Entity* a, Entity* b){ 
        return a->GetZIndex() < b->GetZIndex();
    });
}

void EntityStorage::ReSortByZIndex(Entity* entity, int oldZIdx)
{
    // Get the index of the entity in packed storage
    uint32_t packedIdx = GetEntityIndex(entity, packedEntities);

    if (packedIdx != -1)
    {
        // Check if the new z index is greater than the old z index. If it is, start sorting
        // from the entity's index.
        bool isGreater = entity->GetZIndex() > oldZIdx;
        auto begin = isGreater ? packedEntities.begin() + packedIdx : packedEntities.begin();
        auto end = isGreater ? packedEntities.end() : packedEntities.begin() + packedIdx;

        // Sort EntityStorage from either beginning -> entity index, or entity index -> end
        std::partial_sort(begin, end, packedEntities.end(), [](Entity* a, Entity* b){
            return a->GetZIndex() < b->GetZIndex();
        });
    }
}

void EntityStorage::Remove(Entity* entity) 
{
    // Check if the entity's index is in bounds
    if (entity->GetIndex().index >= entities.size()) return;

    // De-allocate the entity's index
    allocator.Deallocate(entity->GetIndex());

    // Get the packed index
    uint32_t index = GetEntityIndex(entity, packedEntities);

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
