#include "EntityStorage.h"
#include "Entity.h"
#include <utility>
#include <cmath>

BoundingBox Entity::GetBoundingBox()
{
    return BoundingBox();
}

Entity* Entity::Clone()
{
    return nullptr;
}

void Entity::QueueFree()
{
    EntityStorage::QueueFree(this);
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

int Entity::GetZIndex() const
{
    return zIndex;
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
    // Only update z index if there's an actual change
    if (idx == zIndex) return;

    int oldIndex = zIndex;
    zIndex = idx;

    EntityStorage::ReSortByZIndex(this, oldIndex);
}
