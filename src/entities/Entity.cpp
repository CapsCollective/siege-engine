//
// Created by Jonathan Moallem on 30/11/20.
//

#include "Entity.h"
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
