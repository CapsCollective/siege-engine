#ifndef A_DARK_DISCOMFORT_ENTITYSTORAGE_H
#define A_DARK_DISCOMFORT_ENTITYSTORAGE_H

#include "IndexAllocator.h"
#include <vector>
#include "../entities/Entity.h"

class EntityStorage {
public:

    // Constructors

    EntityStorage();

    // Public fields

    static EntityStorage* instance;

    // Public functions

    void Register(Entity*);

    void Remove(Entity*);

    Entity* GetEntity(GenerationalIndex);

    static std::vector<Entity*>& GetEntities() {
        return Instance()->entities;
    }

    static EntityStorage* Instance() {
        if (!instance) {
            instance = new EntityStorage();
        }
        return instance;
    }

    // Operator overloads

    Entity* operator[](GenerationalIndex);

private:

    // Private fields

    IndexAllocator allocator;

    std::vector<Entity*> entities;
};


#endif //A_DARK_DISCOMFORT_ENTITYSTORAGE_H
