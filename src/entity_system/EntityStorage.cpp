#include "EntityStorage.h"
#include <optional>

EntityStorage::EntityStorage() {
    allocator = IndexAllocator();
}

//Entity& EntityStorage::getEntity(GenerationalIndex index) {
//    bool isInBounds = index.index < entities.size();
//
//    if(isInBounds && allocator.isLive(index)) {
//        return entities[index.index];
//    } else {
//        return nullptr;
//    }
//}

void EntityStorage::insert(Entity *) {

}



