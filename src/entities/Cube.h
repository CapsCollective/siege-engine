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

    Cube() :
    Cube(raylib::Vector3::Zero())
    {};

    explicit Cube(raylib::Vector3 pos) :
    Entity(pos)
    {
        // Set default fields
        speed = 1.5;
        raylib::Vector3::Zero();
    };

protected:

    // Protected overrides

    void onUpdate() override;

    void onDraw() override;

private:

    // Private fields

    float speed;

    raylib::Vector3 velocity;
};

#endif //A_DARK_DISCOMFORT_CUBE_H
