#include "EntityStorage.h"
#include <cstdint>
#include <algorithm>

// Static member initialisations of basic fields
IndexAllocator EntityStorage::allocator = IndexAllocator();
std::vector<Entity*> EntityStorage::entities = std::vector<Entity*>();

// Static member initialisations of vectors for holding filtered entities
std::vector<Entity*> EntityStorage::packedEntities = std::vector<Entity*>();
std::vector<Entity*> EntityStorage::packedTools = std::vector<Entity*>();


std::vector<Entity3D*> EntityStorage::packedEntity3Ds = std::vector<Entity3D*>();
std::vector<Entity2D*> EntityStorage::packedEntity2Ds = std::vector<Entity2D*>();

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

    auto entity3D = dynamic_cast<Entity3D*>(entity);

    if (entity3D) {
        packedEntity3Ds.emplace_back(entity3D);
    } else {
        auto entity2D = dynamic_cast<Entity2D*>(entity);

        if (entity2D) {
            packedEntity2Ds.emplace_back(entity2D);
        }
    }

    // Add the entity to the end of its appropriate packed entities
    if (isTool) packedTools.push_back(entity);
    else packedEntities.push_back(entity);
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
    // TODO: Maybe return a struct with all of an entity's index information?
    int32_t index = GetEntityIndex<Entity>(entity, packedEntities);

    int32_t index3D = GetEntityIndex<Entity3D>(dynamic_cast<Entity3D*>(entity), packedEntity3Ds);
    int32_t index2D = GetEntityIndex<Entity2D>(dynamic_cast<Entity2D*>(entity), packedEntity2Ds);

    // Check if we found the appropriate index in our tool or entity storage
    if (index != -1)
    {
        // Erase the packed index entry from our packed entity storage
        packedEntities.erase(packedEntities.begin() + index);
    }

    if (index3D != -1)
    {
        packedEntity3Ds.erase(packedEntity3Ds.begin() + index3D);
    }

    if (index2D != -1)
    {
        packedEntity2Ds.erase(packedEntity2Ds.begin() + index2D);
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
    int32_t index = GetEntityIndex<Entity>(entity, freedEntities);
    if (index == -1) 
    {
        // Push the entity back into our queue
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
