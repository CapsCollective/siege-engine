#ifndef A_DARK_DISCOMFORT_ENTITYSTORAGE_H
#define A_DARK_DISCOMFORT_ENTITYSTORAGE_H

#include "IndexAllocator.h"
#include <vector>
#include "../entities/Entity.h"

class EntityStorage {

public:

    // Constructors

    EntityStorage();

    Entity& getEntity(GenerationalIndex);

    void insert(Entity&);

    void remove(Entity&);

    // Operator overload to get entities.

    Entity& operator[](GenerationalIndex);

private:
    IndexAllocator allocator;
    std::vector<Entity*> entities;
};


#endif //A_DARK_DISCOMFORT_ENTITYSTORAGE_H
