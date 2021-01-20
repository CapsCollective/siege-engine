#include "EntityStorage.h"
#include "Entity.h"
#include <utility>
#include <cmath>

BoundingBox Entity::GetBoundingBox()
{
    return BoundingBox();
}

void Entity::QueueFree()
{
    EntityStorage::QueueFree(this);
}

void Entity::Free()
{
    EntityStorage::Remove(this);
}

const std::string& Entity::GetName() const
{
    return name;
}

const GenerationalIndex& Entity::GetIndex() const
{
    return index;
}

void Entity::SetName(std::string entityName)
{
    name = std::move(entityName);
}

void Entity::SetIndex(GenerationalIndex idx)
{
    index = idx;
}

const raylib::Vector3& Entity::GetPosition() const
{
    return position;
}

float Entity::GetRotation() const
{
    return rotation;
}

void Entity::SetPosition(raylib::Vector3 newPosition)
{
    position = newPosition;
}

void Entity::SetRotation(float newRotation)
{
    rotation = fmod(newRotation, 360.f);
}

void Entity::SetZIndex(int idx)
{
    // TODO add z-sorting to entity storage
    zIndex = idx;
}