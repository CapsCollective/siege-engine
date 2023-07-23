//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_TRANSITIONADAPTER_H
#define SIEGE_ENGINE_TRANSITIONADAPTER_H

#include <utils/math/Maths.h>
#include <utils/math/vec/Vec3.h>

#include <raylib/Vector3.hpp>

#include "../utils/Colour.h"

namespace Siege
{
// Raylib conversions

Vec3 ToVec3(raylib::Vector3 vector);

raylib::Vector3 FromVec3(Vec3 vector);

Vec3 ToVec3(Vector2 vector);

RayCast ToRayCast(Ray ray);

Ray FromRayCast(RayCast ray);

BoundedBox ToBoundedBox(BoundingBox bbox);

BoundingBox FromBoundedBox(BoundedBox bbox);

IColour ToColour(Color color);

Color FromColour(IColour color);
} // namespace Siege

#endif // SIEGE_ENGINE_TRANSITIONADAPTER_H
