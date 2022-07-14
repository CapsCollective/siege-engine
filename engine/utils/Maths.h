//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MATHS_H
#define SIEGE_ENGINE_MATHS_H

#include "vec/Vec2.h"
#include "vec/Vec3.h"
#include "vec/Vec4.h"

namespace Siege
{
struct Xform
{
public:

    Xform() : Xform(Vec3::Zero) {}

    explicit Xform(Vec3 position) : Xform(position, 0.f) {}

    Xform(Vec3 position, float rotation) : Xform(position, rotation, Vec3::One) {}

    Xform(Vec3 position, float rotation, Vec3 scale) :
        position(position),
        rotation(rotation),
        scale(scale)
    {}

    const Vec3& GetPosition() const
    {
        return position;
    }

    float GetRotation() const
    {
        return rotation;
    }

    const Vec3& GetScale() const
    {
        return scale;
    }

    void SetPosition(const Vec3& newPosition)
    {
        position = newPosition;
    }

    void SetRotation(float newRotation)
    {
        rotation = fmod(newRotation, 360.f);
    }

    void SetScale(const Vec3& newScale)
    {
        scale = newScale;
    }

private:

    Vec3 position;
    float rotation;
    Vec3 scale;
};

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

    RayCast(Vec3 position, Vec3 direction) : position(position), direction(direction)
    {}

    // Public members

    Vec3 position;
    Vec3 direction;
};
}

#endif // SIEGE_ENGINE_MATHS_H
