//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_XFORM_H
#define SIEGE_ENGINE_XFORM_H

#include "mat/Mat2.h"
#include "mat/Mat3.h"
#include "mat/Mat4.h"
#include "vec/Vec3.h"

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
        rotation({0.f, rotation, 0.f}),
        scale(scale)
    {}

    // Static functions

    static Mat4 Xform3D(const Vec3& position, const Vec3& rotation, const Vec3& scale);

    static Mat2 Xform2D(const Vec2& position, const float& rotation, const Vec2& scale);

    static Mat3 XformNormalMat(const Vec3& rotation, const Vec3& scale);

    const Vec3& GetPosition() const
    {
        return position;
    }

    Vec3 GetRotation() const
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

    void SetPosition(const Vec2& newPosition)
    {
        position = newPosition;
    }

    void SetPositionX(const float& newPosition)
    {
        position.x = newPosition;
    }

    void SetPositionY(const float& newPosition)
    {
        position.y = newPosition;
    }

    void SetPositionZ(const float& newPosition)
    {
        position.z = newPosition;
    }

    void SetRotation(const Vec3& newRotation)
    {
        rotation = newRotation;
    }

    void SetRotationX(const float& newRotation)
    {
        rotation.x = newRotation;
    }

    void SetRotationY(const float& newRotation)
    {
        rotation.y = newRotation;
    }

    void SetRotationZ(const float& newRotation)
    {
        rotation.z = newRotation;
    }

    void SetScale(const Vec3& newScale)
    {
        scale = newScale;
    }

    void SetScale(const Vec2& newScale)
    {
        scale = newScale;
    }

private:

    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
};
} // namespace Siege

#endif // SIEGE_ENGINE_XFORM_H
