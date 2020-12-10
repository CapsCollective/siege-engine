//
// Created by Jonathan Moallem on 10/12/20.
//

#include "Geometry.h"

void Geometry::onUpdate()
{

}

void Geometry::onDraw()
{
    // Draw geometry to current position
    DrawCube(position, dimensions.x, dimensions.y, dimensions.z, BLUE);
    DrawCubeWires(position, dimensions.x, dimensions.y, dimensions.z, DARKBLUE);
}
