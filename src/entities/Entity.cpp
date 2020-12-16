//
// Created by Jonathan Moallem on 30/11/20.
//

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

void Entity::QueueFree()
{
    EntityStorage::QueueFree(this);
}

// A function for removing entities from the storage - NOT SAFE, USE QUEUE FREE WHEN POSSIBLE!
void Entity::Free()
{
    EntityStorage::Instance()->Remove(this);
}

std::string Entity::GetName()
{
    return name;
}

void Entity::SetName(std::string entityName)
{
    name = std::move(entityName);
}

void Entity::SetEntitySerialise(bool serialise)
{
    shouldSerialise = serialise;
}

bool Entity::ShouldSerialise() const
{
    return shouldSerialise;
}
