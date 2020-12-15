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

    // Static instance getters

    static std::vector<Entity*>& GetEntities() {
        return Instance()->packedEntities;
    }

    static EntityStorage* Instance() {
        if (!instance) {
            instance = new EntityStorage();
        }
        return instance;
    }

    // Operator overloads

    Entity* operator[](GenerationalIndex);

    // Destructor Methods

    void QueueFree(Entity*);

    void FreeEntities();

private:

    // Utility Functions

    uint32_t GetEntityIndex(Entity*, std::vector<Entity*>& entityStorage);

    // Private fields

    IndexAllocator allocator;

    std::vector<Entity*> entities;
    std::vector<Entity*> packedEntities;
    std::vector<Entity*> freedEntities;
};


#endif //A_DARK_DISCOMFORT_ENTITYSTORAGE_H
