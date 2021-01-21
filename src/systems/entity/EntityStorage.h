#ifndef A_DARK_DISCOMFORT_ENTITYSTORAGE_H
#define A_DARK_DISCOMFORT_ENTITYSTORAGE_H

#include "IndexAllocator.h"
#include <algorithm>
#include <vector>
#include "Entity.h"

class EntityStorage 
{
public:

    // Public functions

    /**
     * Queues an entity for initialisation in the next frame
     * @param entity - the entity to register in storage
     */
    static void Register(Entity* entity);

    /**
     * Removes an entity from storage
     * @param entity - entity to be removed from storage
     */
    static void Remove(Entity* entity);

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

    /**
     * Returns the index of a given element within a given entity vector
     * @param entity - the entity pointer you want to find
     * @param storage - the entity vector that you want to search over
     * @return A -1 when no index is found, or the index if the entity
     *         is found
     */
    static uint32_t GetEntityIndex(Entity* entity, std::vector<Entity*>& storage)
    {
        // Try find the entity, and return the index of the entity or -1 if not found
        auto it = std::find(storage.begin(), storage.end(), entity);
        return (it != storage.end()) ? std::distance(storage.begin(), it) : -1;
    }

    // Private fields

    /**
     * The allocator used to provide a generation index to a new entity
     */
    static IndexAllocator allocator;

    /**
     * A full vector containing all entities
     * @warning This storage is not packed, so gaps will exist between
     *          entities if entities are de-allocated
     */
    static std::vector<Entity*> entities;

    /**
     * A full vector containing all non-tool entities
     * @note This storage is packed, so entities are placed in no
     *       particular order
     */
    static std::vector<Entity*> packedEntities;

    /**
     * A packed vector of all tools containing all tool entities
     */
    static std::vector<Entity*> packedTools;

    /**
     * Vector for storing all entities which were queued for freeing
     */
    static std::vector<Entity*> freedEntities;

    /**
     * Vector containing all entities that were queued for adding
     */
    static std::vector<Entity*> registeredEntities;

    /**
     * Adds an entity to the entity storage
     * @param entity - The entity pointer being added
     */
    static void AddEntity(Entity* entity);
};

#endif //A_DARK_DISCOMFORT_ENTITYSTORAGE_H
