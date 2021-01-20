#include "Entity3D.h"
#include <cmath>

BoundingBox Entity3D::GetBoundingBox()
{
    return BoundingBox();
}

const raylib::Vector3& Entity3D::GetPosition() const
{
    return position;
}

float Entity3D::GetRotation() const
{
    return rotation;
}

void Entity3D::SetPosition(raylib::Vector3 newPosition)
{
    position = newPosition;
}

void Entity3D::SetRotation(float newRotation)
{
    rotation = fmod(newRotation, 360.f);
}