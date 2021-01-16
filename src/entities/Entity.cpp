#include "Entity.h"
#include <utility>
#include <cmath>
#include "../entity_system/EntityStorage.h"

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

const raylib::Vector3& Entity::GetPosition() const
{
    return position;
}

float Entity::GetRotation() const
{
    return rotation;
}

const std::string& Entity::GetName() const
{
    return name;
}

const GenerationalIndex& Entity::GetIndex() const
{
    return index;
}

void Entity::SetPosition(raylib::Vector3 newPosition)
{
    position = newPosition;
}

void Entity::SetRotation(float newRotation)
{
    rotation = fmod(newRotation, 360.f);
}

void Entity::SetName(std::string entityName)
{
    name = std::move(entityName);
}

void Entity::SetIndex(GenerationalIndex idx)
{
    index = idx;
}
