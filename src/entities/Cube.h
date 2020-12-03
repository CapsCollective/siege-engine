//
// Created by Jonathan Moallem on 30/11/20.
//

#ifndef A_DARK_DISCOMFORT_CUBE_H
#define A_DARK_DISCOMFORT_CUBE_H

#include "Entity.h"
#include <Vector3.hpp>

class Cube : public Entity
{
public:

    // Constructors

    explicit Cube(raylib::Vector3 pos) :
    position(pos),
    velocity(raylib::Vector3::Zero())
    {};

    // Public overrides

    void onUpdate() override;

    void onDraw() override;

private:

    // Private fields

    float speed = 1.5f;

    raylib::Vector3 position;

    raylib::Vector3 velocity;
};

#endif //A_DARK_DISCOMFORT_CUBE_H
