#include "EntityStorage.h"
#include <cstdint>
#include <algorithm>

// Static members

EntityStorage* EntityStorage::instance;

void EntityStorage::Register(Entity* entity)
{
    if (!entity) return;

    GenerationalIndex index = Instance()->allocator.AllocateIndex();
    entity->SetIndex(index);

    Instance()->registeredEntities.push_back(entity);
}

void EntityStorage::AddEntity(Entity* entity) {

    if (entity->GetIndex().index < entities.size())
    {
        entities[entity->GetIndex().index] = entity;
    }
    else
    {
        entities.push_back(entity);
    }

    packedEntities.push_back(entity);
}

void EntityStorage::RegisterEntities() {
    for (auto& entity : Instance()->registeredEntities) {
        Instance()->AddEntity(entity);
    }
    Instance()->registeredEntities.clear();
}

void EntityStorage::Remove(Entity* entity) 
{
    if (entity->GetIndex().index < entities.size()) 
    {
        allocator.Deallocate(entity->GetIndex());

        int32_t index = GetEntityIndex(entity, packedEntities);
        if (index != -1) 
        {
            packedEntities.erase(packedEntities.begin() + index);
        }
        size_t entityIndex = entity->GetIndex().index;
        delete entities[entityIndex];
    }
}

Entity* EntityStorage::operator[](GenerationalIndex index) 
{
    if (allocator.IsLive(index)) 
    {
        return entities[index.index];
    } 
    else 
    {
        return nullptr;
    }
}

void EntityStorage::QueueFree(Entity* entity) 
{
    int32_t index = GetEntityIndex(entity, Instance()->freedEntities);
    if (index == -1) 
    {
        Instance()->freedEntities.push_back(entity);
    }
}

uint32_t EntityStorage::GetEntityIndex(Entity* entity, std::vector<Entity*>& entityStorage) 
{
    auto it = std::find(entityStorage.begin(), entityStorage.end(), entity);
    int32_t index = -1;
    if (it != entityStorage.end()) 
    {
        index = std::distance(entityStorage.begin(), it);
    }
    return index;
}

void EntityStorage::FreeEntities() 
{
    if (!freedEntities.empty()) 
    {
        for (auto& entity : freedEntities)
        {
            Remove(entity);
        }
        freedEntities.clear();
    }
}