#ifndef A_DARK_DISCOMFORT_ENTITYSTORAGE_H
#define A_DARK_DISCOMFORT_ENTITYSTORAGE_H

#include "IndexAllocator.h"
#include <vector>

class EntityStorage 
{
public:

    // Public functions

    /**
     * Queues an entity to be added to the scene at the end of the frame.
     * @param entity - the entity to add to the storage queue storage
     * @param isTool - A flag to specify whether an entity is a tool (meaning it can't be removed)
     */
    static void Add(class Entity* entity, bool isTool = false);

    /**
     * Queues a batch of entities to be added in the scene at the end of the frame.
     * @param newEntities - A vector of entities to be added to storage
     * @param isTool - A flag to specif whether an entity is a tool (meaning it can't be removed)
     */
    static void Add(const std::vector<Entity*>& newEntities, bool isTool = false);

    /**
     * Returns packed game entities (for iteration purposes)
     * @return a reference to the vector of packed game entities
     */
    static const std::vector<Entity*>& GetEntities() { return packedEntities; }

    /**
     * Returns packed tool entities (for iteration purposes)
     * @return a reference to the vector of packed tool entities
     */
    static const std::vector<Entity*>& GetTools() { return packedTools; }

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
     * Registers all entities separately, should be called before
     * the update loop
     */
    static void RegisterEntities();

    /**
     * Resets the entity storage and removes all tool, non-tool, and queued entities.
     * @warning THIS IS VERY UNSAFE AND SHOULD NOT BE USED IN THE MAIN LOOP.
     */
    static void Reset();

    /**
     * TODO document me
     * @param index
     * @return
     */
    static bool IsLive(const GenerationalIndex& index);

private:

    // Private Functions

    /**
     * Sorts the entity packed storage by Z index.
     * @param storage - the entity vector that needs to be sorted.
     */
    static void Sort(std::vector<Entity*>& storage);

    /**
     * Re-sorts the entity packed index by Z-index.
     * Implements a partial sort, so only relevant sections of the
     * packed storage will be sorted.
     * @param entity - the entity being compared
     * @param oldIdx - the old Z index (for comparison)
     */
    friend class Entity;
    static void SortPartial(Entity* entity, int oldZIdx);

    /**
     * Removes an entity from storage
     * @param entity - entity to be removed from storage
     */
    static void Remove(Entity* entity, std::vector<Entity*>& storage);

    /**
     * Cleared a specific entity storage vector of all entities
     * @param storage - the specific storage that needs to be cleared
     */
    static void ClearStorage(std::vector<Entity*>& storage);

    /**
     * Returns the index of a given element within a given entity vector
     * @param entity - the entity pointer you want to find
     * @param storage - the entity vector that you want to search over
     * @return a -1 when no index is found, or the index if the entity
     *         is found
     */
    static int32_t GetEntityIndex(Entity* entity, const std::vector<Entity*>& storage);

    /**
     * Searches a node of a packed index for an entity
     * @param targetEntity - the entity pointer you want to find
     * @param storage - the entity vector that you want to search over
     * @param targetZIndex - the z index of the entity
     * @param branchIndex - the index of the branch being searched
     * @return a 0 when no entity is found, or the index of the branch if the entity was found
     */
    static size_t SearchBranch(Entity* targetEntity,
                               const std::vector<Entity*>& storage,
                               const int& targetZIndex,
                               const size_t& branchIndex);

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
    static std::vector<std::pair<Entity*, bool>> registeredEntities;
};

#endif //A_DARK_DISCOMFORT_ENTITYSTORAGE_H
