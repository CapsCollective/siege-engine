//
// Created by Jonathan Moallem on 30/11/20.
//

#include "Entity.h"

void Entity::onUpdate()
{

}

void Entity::onDraw()
{

}

void Entity::onUIDraw()
{

}

void Entity::setPosition(raylib::Vector3 newPosition)
{
    position = newPosition;
}

raylib::Vector3 Entity::getPosition()
{
    return position;
}

BoundingBox Entity::getBoundingBox()
{
    return BoundingBox();
}
