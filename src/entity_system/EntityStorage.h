#ifndef A_DARK_DISCOMFORT_ENTITYSTORAGE_H
#define A_DARK_DISCOMFORT_ENTITYSTORAGE_H

#include "IndexAllocator.h"
#include <vector>
#include "../entities/Entity.h"

class EntityStorage 
{
public:

    // Public functions

    /**
     * Queues an entity for initialisation in the next frame
     * @param entity - the entity to register in storage
     * @param isTool - optional, flags the entity to be stored as a tool
     *                 entity rather than a game entity if set to true
     */
    static void Register(Entity* entity, bool isTool = false);

    /**
     * Removes an entity from storage
     * @param entity - entity to be removed from storage
     */
    static void Remove(Entity* entity);

    /**
     * Returns all packed entities (for iteration purposes)
     * @return A reference to the vector of all packed entities
     */
    static const std::vector<Entity*>& GetAllEntities()
    {
        return allPackedEntities;
    }

    /**
     * Returns packed game entities (for iteration purposes)
     * @return A reference to the vector of packed game entities
     */
    static const std::vector<Entity*>& GetEntities()
    {
        return packedEntities;
    }

    /**
     * Returns packed tool entities (for iteration purposes)
     * @return A reference to the vector of packed tool entities
     */
    static const std::vector<Entity*>& GetTools()
    {
        return packedTools;
    }

    /**
     * Returns an entity in the packed list
     * @param index - the index to access
     * @return The entity at the supplied index
     * @warning Accessing an out of bounds index will result in
     *          undefined behaviour
     */
    static Entity* GetPackedEntity(size_t index)
    {
        return packedEntities[index];
    }

    /**
     * Queues an entity for freeing at the end of the frame
     * @param entity - the entity to free
     */
    static void QueueFree(Entity* entity);

    /**
     * Frees all queued entities
     */
    static void FreeEntities();

    /**
     * Registers all entities separately, should be called after
     * the update loop
     */
    static void RegisterEntities();

    /**
     * Operator overload for index operator
     * @param index - the generational index to be accessed
     * @return The entity at the specified generational index
     * @note Will return a nullptr if the entity cannot be found
     */
    Entity* operator[](GenerationalIndex index);

private:

    // Private Functions

    // TODO properly comment API functions

    // Returns the entity's index in the packed vector
    static uint32_t GetEntityIndex(Entity*, std::vector<Entity*>& entityStorage);

    // Private fields

    // Handles generational index allocation/de-allocation
    static IndexAllocator allocator;

    // The vector that stores all entities (this storage will always match the size of all
    // entites that were oreviously made)
    static std::vector<Entity*> entities;

    // The packed vector of entities - contains all available non-tool-entities in no particular order
    static std::vector<Entity*> packedEntities;

    // A packed vector of all tools - contains all tool entities
    static std::vector<Entity*> packedTools;

    // Holds every single entity in a packed index
    static std::vector<Entity*> allPackedEntities;

    // Vector for storing all entities which were queued for freeing
    static std::vector<Entity*> freedEntities;

    // Vector containing all entities that were queued for adding
    static std::vector<std::pair<Entity*, bool>> registeredEntities;

    // Adds an entity to the entity storage
    static void AddEntity(Entity* entity, bool isTool);
};

#endif //A_DARK_DISCOMFORT_ENTITYSTORAGE_H
