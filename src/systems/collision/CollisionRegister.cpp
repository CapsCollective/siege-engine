#include <BoundingBox.hpp>
#include "../entity/Entity.h"
#include "CollisionRegister.h"

// Define static members
std::vector<Entity*> CollisionRegister::collidableEntities;

bool CollisionRegister::CheckCollision(const BoundingBox& boundingBox)
{
    for (auto& entity : collidableEntities)
    {
        if (CheckCollisionBoxes(entity->GetBoundingBox(), boundingBox)) return true;
    }
    return false;
}
