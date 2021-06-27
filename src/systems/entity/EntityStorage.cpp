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

    // Flags to determine the types of entities that were added
    bool isToolAdded = false;
    bool isEntityAdded = false;

    // Iterate over queued entities and initialise them
    for (auto& registrationData : registeredEntities)
    {
        auto& entity = registrationData.first;
        bool isTool = registrationData.second;
        
        // If the entity's given index already exists then override the existing entry
        if (entity->GetIndex().index < entities.size()) entities[entity->GetIndex().index] = entity;
        else entities.push_back(entity);

        auto& storage = isTool ? packedTools : packedEntities;

        isToolAdded = isTool || isToolAdded;
        isEntityAdded = !isTool || isEntityAdded;

        storage.push_back(entity);
    }

    // Once all entities are added, sort the packed storage
    if (isEntityAdded) Sort(packedEntities);
    if (isToolAdded) Sort(packedTools);

    // Remove all entities from the queue
    registeredEntities.clear();
}

void EntityStorage::Sort(std::vector<Entity*>& storage) {
    // Sort packedEntities by Z index - lowest to highest
    std::sort(storage.begin(), storage.end(), [](Entity* a, Entity* b){ 
        return a->GetZIndex() < b->GetZIndex();
    });
}

void EntityStorage::SortPartial(Entity* entity, int oldZIdx)
{
    // Get the index of the entity in packed storage
    int32_t index = GetEntityIndex(entity, packedEntities);
    
    auto& storage = packedEntities;

    // If the entity doesn't exist in our packed entities, then search our tools
    if (index == -1) 
    {
        index = GetEntityIndex(entity, packedTools);
        storage = packedTools;
    }

    if (index != -1)
    {
        // Check if the new z index is greater than the old z index. If it is, start sorting
        // from the entity's index.
        bool isGreater = entity->GetZIndex() > oldZIdx;
        auto begin = isGreater ? storage.begin() + index : storage.begin();
        auto end = isGreater ? storage.end() : storage.begin() + index;

        // Sort EntityStorage from either beginning -> entity index, or entity index -> end
        std::partial_sort(begin, end, storage.end(), [](Entity* a, Entity* b){
            return a->GetZIndex() < b->GetZIndex();
        });
    }
}

void EntityStorage::Remove(Entity* entity, std::vector<Entity*>& storage)
{
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

void EntityStorage::Reset()
{
    freedEntities.insert(freedEntities.begin(), packedEntities.begin(), packedEntities.end());

    for (auto& e : freedEntities) {
        Remove(e, packedEntities);
    }

    freedEntities.clear();

    freedEntities.insert(freedEntities.begin(), packedTools.begin(), packedTools.end());

    for (auto& e : freedEntities) {
        Remove(e, packedTools);
    }

    freedEntities.clear();
}

void EntityStorage::FreeEntities() 
{
    // Iterate over all entities that need to be freed
    for (auto& entity : freedEntities)
    {
        // Remove them and free their memory
        Remove(entity, packedEntities);
    }
    // Clear the storage.
    freedEntities.clear();
}

int32_t EntityStorage::GetEntityIndex(Entity *entity, std::vector<Entity *> &storage) {
    // Try find the entity, and return the index of the entity or -1 if not found
    auto it = std::find(storage.begin(), storage.end(), entity);
    return (it != storage.end()) ? (int32_t) std::distance(storage.begin(), it) : -1;
}
