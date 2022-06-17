#ifndef SIEGE_ENGINE_ENTITYSYSTEM_H
#define SIEGE_ENGINE_ENTITYSYSTEM_H

#include <map>
#include <vector>

#include "IndexAllocator.h"

class EntitySystem
{
public:

    // Public static members

    /**
     * Determines whether a particular entity is valid
     * @param entity - the entity to check
     * @return true if entity is valid, false otherwise
     */
    static bool IsLive(class Entity* entity);

    /**
     * Queues an entity for freeing at the end of the frame
     * @param entity - the entity to free
     */
    static void QueueFree(Entity* entity);

    /**
     * Re-sorts the entity packed index by Z-index.
     * Implements a partial sort, so only relevant sections of the
     * packed storage will be sorted.
     * @param entity - the entity being compared
     * @param oldIdx - the old Z index (for comparison)
     */
    static void Resort(Entity* entity, int oldZIdx);

    // Public methods

    /**
     * Queues an entity to be added to the scene at the end of the frame.
     * @param entity - the entity to add to the storage queue storage
     */
    void Add(Entity* entity);

    /**
     * Queues a batch of entities to be added in the scene at the end of the frame.
     * @param newEntities - A vector of entities to be added to storage
     */
    void Add(const std::vector<Entity*>& newEntities);

    /**
     * Returns packed game entities (for iteration purposes)
     * @return a reference to the vector of packed game entities
     */
    const std::vector<Entity*>& GetEntities();

    /**
     * Queues an entity for freeing at the end of the frame
     * @param entity - the entity to free
     */
    void AddToFreeQueue(Entity* entity);

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

    /**
     * Sets the ability for the storage to perform entity deregistration
     * @param canDeregister - whether or not the storage can deregister entities
     */
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

    // Private static members

    /**
     * Adds an entry to the register for a given entity
     * @param entity - the entity to register
     * @param system - the system to register the entity with
     */
    static void AddToGlobalRegister(Entity* entity, EntitySystem* system);

    /**
     * Removes an entry from the register for a given entity
     * @param entity - the entity to deregister
     */
    static void RemoveFromGlobalRegister(Entity* entity);

    /**
     * Removes all entries related to a specified storage
     * @param system - the system to clear for
     */
    static void RemoveFromGlobalRegister(EntitySystem* system);

    /**
     * Searches for the entity system of a given entity
     * @param entity - the entity to lookup
     * @return a pointer to the entity's found storage, nullptr otherwise
     */
    static EntitySystem* FindInGlobalRegister(Entity* entity);

    /**
     * Resets the entire contents of the global register
     */
    static void ResetGlobalRegister();

    /**
     * A map of all entities to their registered storages
     */
    static std::map<const Entity*, EntitySystem*> globalEntityRegister;

    // Private fields

    /**
     * Controls whether or not the the storage allows for entity deregistration
     */
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

#endif // SIEGE_ENGINE_ENTITYSYSTEM_H
