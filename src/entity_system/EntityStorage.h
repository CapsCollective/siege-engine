#ifndef A_DARK_DISCOMFORT_ENTITYSTORAGE_H
#define A_DARK_DISCOMFORT_ENTITYSTORAGE_H

#include "IndexAllocator.h"
#include <vector>
#include "../entities/Entity.h"

class EntityStorage 
{
public:

    // Constructors

    EntityStorage() :
    allocator(IndexAllocator())
    {
        instance = nullptr;
    };

    // Public fields

    static EntityStorage* instance;

    // Public functions

    static void Register(Entity*);

    void Remove(Entity*);

    // Static instance getters

    static std::vector<Entity*>& GetEntities() 
    {
        return Instance()->packedEntities;
    }

    static EntityStorage* Instance() {
        if (!instance) instance = new EntityStorage();
        return instance;
    }

    static Entity* GetEntity(GenerationalIndex index) 
    {
        // Check that the index is in bounds
        bool isInBounds = index.index < Instance()->entities.size();

        // If the entity index is live (is still in use)
        if(isInBounds && Instance()->allocator.IsLive(index)) 
        {
            // Return the entity
            return Instance()->entities[index.index];
        } 
        else return nullptr;
    }

    static Entity* GetPackedEntity(size_t index)
    {
        return Instance()->packedEntities[index];
    }

    // Operator overloads

    Entity* operator[](GenerationalIndex);

    // Destructor Methods

    static void QueueFree(Entity*);

    void FreeEntities();

    static void RegisterEntities();

private:

    // Utility Functions

    static uint32_t GetEntityIndex(Entity*, std::vector<Entity*>& entityStorage);

    // Private fields

    IndexAllocator allocator;

    std::vector<Entity*> entities;

    std::vector<Entity*> packedEntities;

    // Deferred vectors - for modifying containers.
    std::vector<Entity*> freedEntities;

    std::vector<Entity*> registeredEntities;

    void AddEntity(Entity *entity);
};

#endif //A_DARK_DISCOMFORT_ENTITYSTORAGE_H
