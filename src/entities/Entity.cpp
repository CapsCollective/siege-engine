#include "Entity.h"
#include <utility>
#include <cmath>
#include "../entity_system/EntityStorage.h"

BoundingBox Entity::GetBoundingBox()
{
    return BoundingBox();
}

void Entity::SetPosition(raylib::Vector3 newPosition)
{
    position = newPosition;
}

raylib::Vector3 Entity::GetPosition()
{
    return position;
}

void Entity::SetRotation(float newRotation)
{
    rotation = fmod(newRotation, 360.f);
}

float Entity::GetRotation() const
{
    return rotation;
}

void Entity::QueueFree()
{
    EntityStorage::QueueFree(this);
}

void Entity::Free()
{
    // Remove entity from storage - NOT SAFE, USE QUEUE FREE WHEN POSSIBLE!
    EntityStorage::Remove(this);
}

std::string Entity::GetName()
{
    return name;
}

void Entity::SetName(std::string entityName)
{
    name = std::move(entityName);
}

Entity* Entity::Clone()
{
    return nullptr;
}
