#ifndef A_DARK_DISCOMFORT_ENTITYSTORAGE_H
#define A_DARK_DISCOMFORT_ENTITYSTORAGE_H

#include "IndexAllocator.h"
#include <vector>
#include "../entities/Entity.h"

class EntityStorage {

public:

    // Constructors

    static EntityStorage* instance;

    EntityStorage();

    Entity* GetEntity(GenerationalIndex);

    void Register(Entity*);

    void Remove(Entity*);

    // Operator overload to get entities.

    Entity* operator[](GenerationalIndex);

    static std::vector<Entity*>& GetEntities() {
        return Instance()->entities;
    }

    static EntityStorage* Instance() {
        if (!instance) {
            instance = new EntityStorage();
        }
        return instance;
    }

private:
    IndexAllocator allocator;
    std::vector<Entity*> entities;
};


#endif //A_DARK_DISCOMFORT_ENTITYSTORAGE_H
