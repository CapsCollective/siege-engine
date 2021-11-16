#ifndef A_DARK_DISCOMFORT_TRANSITIONADAPTER_H
#define A_DARK_DISCOMFORT_TRANSITIONADAPTER_H

#include "Maths.h"
#include <raylib/Vector3.hpp>

// Raylib conversions

Vec3 ToVec3(raylib::Vector3 vector)
{
    return {vector.x, vector.y, vector.z};
}

raylib::Vector3 FromVec3(Vec3 vector)
{
    return {vector.x, vector.y, vector.z};
}

Vec3 ToVec3(Vector2 vector)
{
    return {vector.x, vector.y, 0.f};
}

RayCast ToRayCast(Ray ray)
{
    return {ToVec3(ray.position), ToVec3(ray.direction)};
}

Ray FromRayCast(RayCast ray)
{
    return {FromVec3(ray.position), FromVec3(ray.direction)};
}

BoundedBox ToBoundedBox(BoundingBox bbox)
{
    return {ToVec3(bbox.min), ToVec3(bbox.max)};
}

BoundingBox FromBoundedBox(BoundedBox bbox)
{
    return {FromVec3(bbox.min), FromVec3(bbox.max)};
}

Colour ToColour(Color color)
{
    return {color.r, color.g, color.b, color.a};
}

Color FromColour(Colour color)
{
    return {(unsigned char) color.r,
            (unsigned char) color.g,
            (unsigned char) color.b,
            (unsigned char) color.a};
}

#endif //A_DARK_DISCOMFORT_TRANSITIONADAPTER_H
