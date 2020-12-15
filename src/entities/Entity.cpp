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
    EntityStorage::Instance()->QueueFree(this);
}

std::string Entity::GetName()
{
    return name;
}

void Entity::SetName(std::string entityName)
{
    name = std::move(entityName);
}
