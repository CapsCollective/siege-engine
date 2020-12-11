#include "EntityStorage.h"

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
}

void EntityStorage::Remove(Entity* entity) {

}

Entity* EntityStorage::operator[](GenerationalIndex index) {
    if (allocator.IsLive(index)) {
        return entities[index.index];
    } else {
        return nullptr;
    }
}


