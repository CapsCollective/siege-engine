//
// Created by Jonathan Moallem on 10/12/20.
//

#include "Geometry.h"

void Geometry::onDraw()
{
    // Draw geometry to current position
    DrawCube(position, dimensions.x, dimensions.y, dimensions.z, BLUE);
    DrawCubeWires(position, dimensions.x, dimensions.y, dimensions.z, DARKBLUE);
}

BoundingBox Geometry::getBoundingBox()
{
    return BoundingBox{
            position - raylib::Vector3(dimensions.x/2.f, dimensions.y/2.f, dimensions.z/2.f),
            position + raylib::Vector3(dimensions.x/2.f, dimensions.y/2.f, dimensions.z/2.f),
    };
}
