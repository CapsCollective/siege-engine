#include <BoundingBox.hpp>
#include "../entity/Entity.h"
#include "CollisionRegister.h"

// Define static members
std::vector<Entity*> CollisionRegister::collidableEntities;

void CollisionRegister::Add(Entity* entity)
{
    // Add the entity to the register
    collidableEntities.push_back(entity);
}

raylib::Vector3 CollisionRegister::MoveAndSlide(const BoundingBox& boundingBox, raylib::Vector3 velocity)
{
    // TODO convert this system to use OBBs with separating plane theorem
    // TODO add collision sweeping for more accurate results
    // Get the min and max values for the bounding box
    raylib::Vector3 boxMin = boundingBox.min;
    raylib::Vector3 boxMax = boundingBox.max;

    // Calculate the bounding box positions for each component of the object's velocity
    raylib::Vector3 yVelocity = raylib::Vector3(0.f, velocity.y, 0.f);
    raylib::Vector3 xVelocity = raylib::Vector3(velocity.x, 0.f, 0.f);
    raylib::Vector3 zVelocity = raylib::Vector3(0.f, 0.f, velocity.z);
    BoundingBox yBox = {boxMin + yVelocity, boxMax + yVelocity};
    BoundingBox xBox = {boxMin + xVelocity, boxMax + xVelocity};
    BoundingBox zBox = {boxMin + zVelocity,boxMax + zVelocity};

    // Check for collision against all collidable entities
    for (auto& entity : collidableEntities)
    {
        // Nullify the any colliding velocity components
        if (CheckCollisionBoxes(entity->GetBoundingBox(), yBox)) velocity.y = 0.f;
        if (CheckCollisionBoxes(entity->GetBoundingBox(), xBox)) velocity.x = 0.f;
        if (CheckCollisionBoxes(entity->GetBoundingBox(), zBox)) velocity.z = 0.f;
    }
    return velocity;
}

bool CollisionRegister::CheckCollision(const BoundingBox& boundingBox)
{
    // Check collision for each registered entity against the bounding box
    for (auto& entity : collidableEntities)
    {
        if (CheckCollisionBoxes(entity->GetBoundingBox(), boundingBox)) return true;
    }
    return false;
}
