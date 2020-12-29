#include "Entity.h"
#include <utility>
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

void Entity::QueueFree(bool forceFree)
{
    if (isFreeable || forceFree) EntityStorage::QueueFree(this);
}

// A function for removing entities from the storage - NOT SAFE, USE QUEUE FREE WHEN POSSIBLE!
void Entity::Free(bool forceFree)
{
    if (isFreeable || forceFree) EntityStorage::Remove(this);
}

std::string Entity::GetName()
{
    return name;
}

void Entity::SetName(std::string entityName)
{
    name = std::move(entityName);
}

void Entity::SetSerialisable(bool serialisable)
{
    isSerialisable = serialisable;
}

bool Entity::IsSerialisable() const
{
    return isSerialisable;
}

void Entity::SetFreeable(bool freeable)
{
    isFreeable = freeable;
}

Entity* Entity::Clone()
{
    return nullptr;
}
