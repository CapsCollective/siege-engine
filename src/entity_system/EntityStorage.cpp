#include "EntityStorage.h"
#include <iostream>
#include <cstdint>

EntityStorage* EntityStorage::instance;

EntityStorage::EntityStorage() {
    allocator = IndexAllocator();
    instance = nullptr;
}

Entity* EntityStorage::GetEntity(GenerationalIndex index) {
    bool isInBounds = index.index < entities.size();

    if(isInBounds && allocator.IsLive(index)) {
        return entities[index.index];
    } else {
        return nullptr;
    }
}

void EntityStorage::Register(Entity* entity) {

    GenerationalIndex index = allocator.AllocateIndex();
    entity->SetIndex(index);

    if (index.index < entities.size()) {
        entities[index.index] = entity;
    } else {
        entities.push_back(entity);
    }

    packedEntities.push_back(entity);
}

void EntityStorage::Remove(Entity* entity) {
    if (entity->GetIndex().index < entities.size()) {
        allocator.Deallocate(entity->GetIndex());

        int32_t index = GetEntityIndex(entity, packedEntities);
        if (index != -1) {
            packedEntities.erase(packedEntities.begin() + index);
            std::cout << "Removing entity from packed index at position: " << index << "." << std::endl;
        }
        size_t entityIndex = entity->GetIndex().index;
        delete entities[entityIndex];
    }
}

Entity* EntityStorage::operator[](GenerationalIndex index) {
    if (allocator.IsLive(index)) {
        return entities[index.index];
    } else {
        return nullptr;
    }
}

void EntityStorage::QueueFree(Entity* entity) {
    std::cout << "Adding entity at index: " << entity->GetIndex().index << " and generation: " << entity->GetIndex().generation << " for removal" << std::endl;
    int32_t index = GetEntityIndex(entity, freedEntities);
    if (index == -1) {
        std::cout << "Adding entity for removal..." << std::endl;
        freedEntities.push_back(entity);
    }
}

uint32_t EntityStorage::GetEntityIndex(Entity* entity, std::vector<Entity*>& entityStorage) {
    auto it = std::find(entityStorage.begin(), entityStorage.end(), entity);
    int32_t index = -1;
    if (it != entityStorage.end()) {
        index = std::distance(entityStorage.begin(), it);
    }
    return index;
}

void EntityStorage::FreeEntities() {
    if (!freedEntities.empty()) {
        std::cout << "Freeing entities..." << std::endl;
        for (int32_t i = freedEntities.size() - 1; i > -1; i--) {
            Remove(freedEntities[i]);
            freedEntities.erase(freedEntities.begin() + i);
        }
    }
}


