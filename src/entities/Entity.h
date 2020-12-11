//
// Created by Jonathan Moallem on 30/11/20.
//

#ifndef A_DARK_DISCOMFORT_ENTITY_H
#define A_DARK_DISCOMFORT_ENTITY_H

#include <Vector3.hpp>

class Entity
{
public:

    // Virtual methods

    virtual void onUpdate();

    virtual void onDraw();

    virtual void onUIDraw();

    virtual BoundingBox getBoundingBox();

    // Public methods

    void setPosition(raylib::Vector3 newPosition);

    raylib::Vector3 getPosition();

protected:

    // Protected constructors

    Entity() :
    Entity(raylib::Vector3::Zero())
    {};

    explicit Entity(raylib::Vector3 position) :
    position(position)
    {};

    // Protected fields

    raylib::Vector3 position;
};

#endif //A_DARK_DISCOMFORT_ENTITY_H
