#ifndef A_DARK_DISCOMFORT_ENTITYSYSTEM_H
#define A_DARK_DISCOMFORT_ENTITYSYSTEM_H

#include <map>
#include <vector>

#include "IndexAllocator.h"

class EntitySystem
{
public:

    // Public functions

    /**
     * Queues an entity to be added to the scene at the end of the frame.
     * @param entity - the entity to add to the storage queue storage
     */
    void Add(class Entity* entity);

    /**
     * Queues a batch of entities to be added in the scene at the end of the frame.
     * @param newEntities - A vector of entities to be added to storage
     */
    void Add(const std::vector<Entity*>& newEntities);

    /**
     * Returns packed game entities (for iteration purposes)
     * @return a reference to the vector of packed game entities
     */
    const std::vector<Entity*>& GetEntities()
    {
        return packedEntities;
    }

    /**
     * Queues an entity for freeing at the end of the frame
     * @param entity - the entity to free
     */
    void QueueFree(Entity* entity);

    /**
     * Frees all queued entities
     */
    void FreeEntities();

    /**
     * Registers all entities separately, should be called before
     * the update loop
     */
    void RegisterEntities();

    /**
     * Resets the entity storage and removes all queued and non-queued entities.
     * @warning THIS IS VERY UNSAFE AND SHOULD NOT BE USED IN THE MAIN LOOP.
     */
    void Reset();

    /**
     * Determines whether a particular entity is valid.
     * @param index - the generational index of the entity.
     * @return true if entity is valid, false otherwise.
     */
    bool IsLive(const GenerationalIndex& index);

    void SetAllowDeregistration(bool canDeregister);

private:

    // Private Functions

    /**
     * Sorts the entity packed storage by Z index.
     */
    void SortStorage();

    /**
     * Re-sorts the entity packed index by Z-index.
     * Implements a partial sort, so only relevant sections of the
     * packed storage will be sorted.
     * @param entity - the entity being compared
     * @param oldIdx - the old Z index (for comparison)
     */
    friend class Entity;
    void SortPartial(Entity* entity, int oldZIdx);

    /**
     * Removes an entity from storage
     * @param entity - entity to be removed from storage
     */
    void Remove(Entity* entity, std::vector<Entity*>& storage);

    /**
     * Cleared a specific entity storage vector of all entities
     * @param storage - the specific storage that needs to be cleared
     */
    void ClearStorage(std::vector<Entity*>& storage);

    /**
     * Returns the index of a given element within a given entity vector
     * @param entity - the entity pointer you want to find
     * @param storage - the entity vector that you want to search over
     * @return a -1 when no index is found, or the index if the entity
     *         is found
     */
    int32_t GetEntityIndex(Entity* entity, const std::vector<Entity*>& storage);

    // Private fields

    bool allowDeregistration = true;

    /**
     * The allocator used to provide a generation index to a new entity
     */
    IndexAllocator allocator;

    /**
     * A full vector containing all entities
     * @warning This storage is not packed, so gaps will exist between
     *          entities if entities are de-allocated
     */
    std::vector<Entity*> entities;

    /**
     * A full vector containing all entities
     * @note This storage is packed, so entities are placed in no
     *       particular order
     */
    std::vector<Entity*> packedEntities;

    /**
     * Vector for storing all entities which were queued for freeing
     */
    std::vector<Entity*> freedEntities;

    /**
     * Vector containing all entities that were queued for adding
     */
    std::vector<Entity*> registeredEntities;
};

class EntitySystemRegister
{
public:

    static void Register(Entity* entity, EntitySystem* system);

    static void Deregister(Entity* entity);

    static EntitySystem* GetSystem(Entity* entity);

    static void Clear(EntitySystem* system);

    static void Reset();

private:

    static std::map<const Entity*, EntitySystem*> entitySystemMap;
};

#endif // A_DARK_DISCOMFORT_ENTITYSYSTEM_H
