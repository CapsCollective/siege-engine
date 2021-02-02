#ifndef A_DARK_DISCOMFORT_COLLISIONSYSTEM_H
#define A_DARK_DISCOMFORT_COLLISIONSYSTEM_H

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
     * Checks the collision of the object while moving
     * it along a vector to find its movable velocity
     * @param boundingBox - the bounding box to collide
     * @param velocity - the starting velocity of the
     *                   colliding object
     * @return The resulting linear velocity after
     *         applying any collision events
     */
    static raylib::Vector3 MoveAndSlide(const BoundingBox& boundingBox, raylib::Vector3 velocity);

    /**
     * Checks a given bounding box for collisions against
     * any registered entities
     * @param boundingBox - the bounding box to check
     * @return Whether there were any collisions
     */
    static bool CheckCollision(const BoundingBox& boundingBox);

private:

    // Private fields

    static std::vector<class Entity*> collidableEntities;
};


#endif //A_DARK_DISCOMFORT_COLLISIONSYSTEM_H
