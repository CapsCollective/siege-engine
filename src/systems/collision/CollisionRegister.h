#ifndef A_DARK_DISCOMFORT_COLLISIONREGISTER_H
#define A_DARK_DISCOMFORT_COLLISIONREGISTER_H

#include <vector>

class CollisionRegister
{
public:

    // Public methods

    /**
     * Registers a given entity for collision detection
     * @param entity - the entity to register
     */
    static void Add(Entity* entity);

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


#endif //A_DARK_DISCOMFORT_COLLISIONREGISTER_H
