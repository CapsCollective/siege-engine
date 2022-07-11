//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "CollisionSystem.h"

#include <algorithm>

void CollisionSystem::Add(Entity* entity)
{
    // Set the entity for registration
    addedEntities.push_back(entity);
}

void CollisionSystem::Remove(Entity* entity)
{
    // Set the entity for de-registration
    removedEntities.push_back(entity);
}

void CollisionSystem::RegisterEntities()
{
    // Register all entities for addition
    for (auto& entity : addedEntities)
    {
        collidableEntities.push_back(entity);
    }
    addedEntities.clear();
}

void CollisionSystem::FreeEntities()
{
    // Find and deregister all entities for removal
    for (auto& entity : removedEntities)
    {
        auto it = std::find(collidableEntities.begin(), collidableEntities.end(), entity);
        if (it != collidableEntities.end()) collidableEntities.erase(it);
    }
    removedEntities.clear();
}

Siege::Vec::Vec3 CollisionSystem::MoveAndSlide(const BoundedBox& boundingBox,
                                               Siege::Vec::Vec3 velocity)
{
    // TODO convert this system to use OBBs with separating plane theorem
    // TODO add collision sweeping for more accurate results
    // Get the min and max values for the bounding box
    const Siege::Vec::Vec3& boxMin = boundingBox.min;
    const Siege::Vec::Vec3& boxMax = boundingBox.max;

    // Calculate the bounding box positions for each component of the object's velocity
    BoundedBox yBox = {boxMin + velocity.YComp(), boxMax + velocity.YComp()};
    BoundedBox xBox = {boxMin + velocity.XComp(), boxMax + velocity.XComp()};
    BoundedBox zBox = {boxMin + velocity.ZComp(), boxMax + velocity.ZComp()};

    // TODO make collisions call OnCollision for Collidables
    // Check for collision against all collidable entities
    for (auto& entity : collidableEntities)
    {
        // Nullify the any colliding velocity components
        if (yBox.Intersects(entity->GetBoundingBox())) velocity.y = 0.f;
        if (xBox.Intersects(entity->GetBoundingBox())) velocity.x = 0.f;
        if (zBox.Intersects(entity->GetBoundingBox())) velocity.z = 0.f;
    }
    return velocity;
}

bool CollisionSystem::CheckCollision(const BoundedBox& boundingBox)
{
    // TODO make storage use an octree for improved performance
    // Check collision for each registered entity against the bounding box
    for (auto& entity : collidableEntities)
    {
        if (boundingBox.Intersects(entity->GetBoundingBox())) return true;
    }
    return false;
}
