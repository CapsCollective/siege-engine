#include "Geometry.h"

void Geometry::OnDraw()
{
    // Draw geometry to current position
    DrawCube(position, dimensions.x, dimensions.y, dimensions.z, BLUE);
    DrawCubeWires(position, dimensions.x, dimensions.y, dimensions.z, DARKBLUE);
}

BoundingBox Geometry::GetBoundingBox()
{
    return BoundingBox{
            position - raylib::Vector3(dimensions.x/2.f, dimensions.y/2.f, dimensions.z/2.f),
            position + raylib::Vector3(dimensions.x/2.f, dimensions.y/2.f, dimensions.z/2.f),
    };
}

raylib::Vector3 Geometry::GetDimensions()
{
    return dimensions;
}
