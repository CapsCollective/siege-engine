//
// Created by Jonathan Moallem on 29/1/21.
//

#ifndef A_DARK_DISCOMFORT_COLLISIONREGISTER_H
#define A_DARK_DISCOMFORT_COLLISIONREGISTER_H

#include <vector>

class CollisionRegister
{
public:

    static bool CheckCollision(const BoundingBox& boundingBox);

private:

    // Private fields

    static std::vector<class Entity*> collidableEntities;
};


#endif //A_DARK_DISCOMFORT_COLLISIONREGISTER_H
