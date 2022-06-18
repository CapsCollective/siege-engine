#ifndef SIEGE_ENGINE_COLLISIONSYSTEM_H
#define SIEGE_ENGINE_COLLISIONSYSTEM_H

#include <vector>

#include "../entity/Entity.h"

class CollisionSystem
{
public:

    // Public methods

    /**
     * Registers a given entity for collision detection
     * @param entity - the entity to register
     */
    void Add(Entity* entity);

    /**
     * De-registers a given entity for collision detection
     * @param entity - the entity to deregister
     */
    void Remove(Entity* entity);

    /**
     * Registers all added entities, should be called before
     * the update loop
     */
    void RegisterEntities();

    /**
     * Frees all removed entities, should be called after
     * the update loop
     */
    void FreeEntities();

    /**
     * Checks the collision of the object while moving
     * it along a vector to find its movable velocity
     * @param boundingBox - the bounding box to collide
     * @param velocity - the starting velocity of the
     *                   colliding object
     * @return the resulting linear velocity after
     *         applying any collision events
     */
    Vec3 MoveAndSlide(const BoundedBox& boundingBox, Vec3 velocity);

    /**
     * Checks a given bounding box for collisions against
     * any registered entities
     * @param boundingBox - the bounding box to check
     * @return whether there were any collisions
     */
    bool CheckCollision(const BoundedBox& boundingBox);

private:

    // Private fields

    std::vector<Entity*> collidableEntities;

    std::vector<Entity*> addedEntities;

    std::vector<Entity*> removedEntities;
};

#endif // SIEGE_ENGINE_COLLISIONSYSTEM_H
