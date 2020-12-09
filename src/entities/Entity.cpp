//
// Created by Jonathan Moallem on 30/11/20.
//

#include "Entity.h"

void Entity::setPosition(raylib::Vector3 pos)
{
    position = pos;
}

raylib::Vector3 Entity::getPosition()
{
    return position;
}
