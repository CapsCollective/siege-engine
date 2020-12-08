//
// Created by Jonathan Moallem on 30/11/20.
//

#ifndef A_DARK_DISCOMFORT_ENTITY_H
#define A_DARK_DISCOMFORT_ENTITY_H

#include <Vector3.hpp>

class Entity
{
public:

    // Pure virtual methods

    virtual void onUpdate() = 0;

    virtual void onDraw() = 0;

protected:

    // Protected constructors

    Entity() :
    Entity(raylib::Vector3::Zero())
    {};

    explicit Entity(raylib::Vector3 pos) :
    position(pos)
    {};

    // Protected fields

    raylib::Vector3 position;
};

#endif //A_DARK_DISCOMFORT_ENTITY_H
