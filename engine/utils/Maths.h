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

struct Xform
{
public:

    Xform() : Xform(Siege::Vec3::Zero) {}

    explicit Xform(Siege::Vec3 position) : Xform(position, 0.f) {}

    Xform(Siege::Vec3 position, float rotation) : Xform(position, rotation, Siege::Vec3::One) {}

    Xform(Siege::Vec3 position, float rotation, Siege::Vec3 scale) :
        position(position),
        rotation(rotation),
        scale(scale)
    {}

    const Siege::Vec3& GetPosition() const
    {
        return position;
    }

    float GetRotation() const
    {
        return rotation;
    }

    const Siege::Vec3& GetScale() const
    {
        return scale;
    }

    void SetPosition(const Siege::Vec3& newPosition)
    {
        position = newPosition;
    }

    void SetRotation(float newRotation)
    {
        rotation = fmod(newRotation, 360.f);
    }

    void SetScale(const Siege::Vec3& newScale)
    {
        scale = newScale;
    }

private:

    Siege::Vec3 position;
    float rotation;
    Siege::Vec3 scale;
};

struct BoundedBox
{
    // 'Structors

    BoundedBox() : BoundedBox(Siege::Vec3(), Siege::Vec3()) {}

    BoundedBox(Siege::Vec3 min, Siege::Vec3 max) : min(min), max(max) {}

    // Public methods

    bool Intersects(const BoundedBox& other) const;

    bool Intersects(const struct RayCast& ray) const;

    // Public members

    Siege::Vec3 min;
    Siege::Vec3 max;
};

struct RayCast
{
    // 'Structors

    RayCast() : RayCast(Siege::Vec3(), Siege::Vec3()) {}

    RayCast(Siege::Vec3 position, Siege::Vec3 direction) : position(position), direction(direction)
    {}

    // Public members

    Siege::Vec3 position;
    Siege::Vec3 direction;
};

#endif // SIEGE_ENGINE_MATHS_H
