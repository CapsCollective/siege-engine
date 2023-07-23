//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MATHS_H
#define SIEGE_ENGINE_MATHS_H

#include "Float.h"
#include "mat/Mat4.h"
#include "vec/Vec2.h"
#include "vec/Vec3.h"
#include "vec/Vec4.h"

namespace Siege
{

template<typename T>
inline constexpr T Epsilon()
{
    return static_cast<T>(1e-6);
}

struct BoundedBox
{
    // 'Structors

    BoundedBox() : BoundedBox(Vec3(), Vec3()) {}

    BoundedBox(Vec3 min, Vec3 max) : min(min), max(max) {}

    // Public methods

    bool Intersects(const BoundedBox& other) const;

    bool Intersects(const struct RayCast& ray) const;

    // Public members

    Vec3 min;
    Vec3 max;
};

struct RayCast
{
    // 'Structors

    RayCast() : RayCast(Vec3(), Vec3()) {}

    RayCast(Vec3 position, Vec3 direction) : position(position), direction(direction) {}

    // Public members

    Vec3 position;
    Vec3 direction;
};
} // namespace Siege

#endif // SIEGE_ENGINE_MATHS_H
