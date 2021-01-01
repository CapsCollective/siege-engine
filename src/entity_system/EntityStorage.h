#ifndef A_DARK_DISCOMFORT_ENTITYSTORAGE_H
#define A_DARK_DISCOMFORT_ENTITYSTORAGE_H

#include "IndexAllocator.h"
#include <vector>
#include "../entities/Entity.h"

class EntityStorage 
{
public:

    // Public functions

    // Queues an entity for initialisation in the next frame
    static void Register(Entity*, bool = false);

    // Removes an entity from storage
    static void Remove(Entity*);

    // Returns our packed entities (for iteration)
    static std::vector<Entity*>& GetEntities() 
    {
        return packedEntities;
    }

    static std::vector<Entity*>& GetAllEntities()
    {
        return allPackedEntities;
    }

    static std::vector<Entity*>& GetTools()
    {
        return packedTools;
    }

    // Returns an entity in the packed list
    static Entity* GetPackedEntity(size_t index)
    {
        return packedEntities[index];
    }

    // Operator overload for index operator
    Entity* operator[](GenerationalIndex);

    // Queues an entity for freeing at the end of the frame
    static void QueueFree(Entity*);

    // Frees all queued entities
    static void FreeEntities();

    // Registers all entities separately (usually happens after the update loop.)
    static void RegisterEntities();

private:

    // Public Functions

    // Returns the entity's index in the packed vector.
    // TODO: Make this more efficient. Currently just does an iterator search.
    static uint32_t GetEntityIndex(Entity*, std::vector<Entity*>& entityStorage);

    // Private fields

    // Handles generational index allocation/de-allocation.
    static IndexAllocator allocator;

    // The vector that stores all entities (this storage will always match the size of all
    // entites that were oreviously made)
    static std::vector<Entity*> entities;

    // The packed vector of entities - contains all available non-tool-entities in no particular order.
    static std::vector<Entity*> packedEntities;

    // A packed vector of all tools - contains all tool entities.
    static std::vector<Entity*> packedTools;

    // Holds every single entity in a packed index
    static std::vector<Entity*> allPackedEntities;

    // Vector for storing all entities which were queued for freeing.
    static std::vector<Entity*> freedEntities;

    // Vector containing all entities that were queued for adding.
    static std::vector<std::pair<Entity*, bool>> registeredEntities;

    // Adds an entity to the entity storage.
    static void AddEntity(Entity* entity, bool isTool);
};

#endif //A_DARK_DISCOMFORT_ENTITYSTORAGE_H
