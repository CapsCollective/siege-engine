//
// Created by Jonathan Moallem on 30/11/20.
//

#include "Cube.h"

void Cube::onUpdate()
{

}

void Cube::onDraw()
{
    // Draw cube to current position
    DrawCube(position, 2.f, 2.f, 2.f, RED);
    DrawCubeWires(position, 2.f, 2.f, 2.f, MAROON);
}
