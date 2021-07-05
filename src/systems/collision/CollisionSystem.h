#ifndef A_DARK_DISCOMFORT_COLLISIONSYSTEM_H
#define A_DARK_DISCOMFORT_COLLISIONSYSTEM_H

#include "../entity/Entity.h"
#include <vector>

class CollisionSystem
{
public:

    // Public methods

    /**
     * Registers a given entity for collision detection
     * @param entity - the entity to register
     */
    static void Add(Entity* entity);

    /**
     * De-registers a given entity for collision detection
     * @param entity - the entity to deregister
     */
    static void Remove(Entity* entity);

    /**
     * Registers all added entities, should be called before
     * the update loop
     */
    static void RegisterEntities();

    /**
     * Frees all removed entities, should be called after
     * the update loop
     */
    static void FreeEntities();

    /**
     * Checks the collision of the object while moving
     * it along a vector to find its movable velocity
     * @param boundingBox - the bounding box to collide
     * @param velocity - the starting velocity of the
     *                   colliding object
     * @return the resulting linear velocity after
     *         applying any collision events
     */
    static raylib::Vector3 MoveAndSlide(const BoundingBox& boundingBox, raylib::Vector3 velocity);

    /**
     * Checks a given bounding box for collisions against
     * any registered entities
     * @param boundingBox - the bounding box to check
     * @return whether there were any collisions
     */
    static bool CheckCollision(const BoundingBox& boundingBox);

private:

    // Private fields

    static std::vector<Entity*> collidableEntities;

    static std::vector<Entity*> addedEntities;

    static std::vector<Entity*> removedEntities;
};


#endif //A_DARK_DISCOMFORT_COLLISIONSYSTEM_H
