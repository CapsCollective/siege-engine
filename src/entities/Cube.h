//
// Created by Jonathan Moallem on 30/11/20.
//

#ifndef A_DARK_DISCOMFORT_CUBE_H
#define A_DARK_DISCOMFORT_CUBE_H

#include "Entity.h"

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
        raylib::Vector3::Zero();
    };

protected:

    // Protected overrides

    void onUpdate() override;

    void onDraw() override;
};

#endif //A_DARK_DISCOMFORT_CUBE_H
