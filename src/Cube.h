//
// Created by Jonathan Moallem on 30/11/20.
//

#ifndef A_DARK_DISCOMFORT_CUBE_H
#define A_DARK_DISCOMFORT_CUBE_H

#include "Entity.h"
#include "../include/raylib-cpp.hpp"

class Cube : public Entity
{
public:

    Cube(raylib::Vector3 pos) :
    position(pos) {};

    void onUpdate() override;

    void onDraw() override;

private:

    raylib::Vector3 position;
};


#endif //A_DARK_DISCOMFORT_CUBE_H
