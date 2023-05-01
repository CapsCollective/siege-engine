//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Graphics.h"

#include "Float.h"

namespace Siege::Graphics
{

Mat4 Perspective(const float& fovy, const float& aspect, const float& near, const float& far)
{
    const float tanHalfFovy = tan(fovy / 2.f);

    Mat4 projectionMatrix = {};

    projectionMatrix[0] = 1.f / (aspect * tanHalfFovy);
    projectionMatrix[5] = 1.f / (tanHalfFovy);
    projectionMatrix[10] = far / (far - near);
    projectionMatrix[11] = 1.f;
    projectionMatrix[14] = -(far * near) / (far - near);

    return projectionMatrix;
}

Mat4 Orthographic(const float& left,
                  const float& right,
                  const float& top,
                  const float& bottom,
                  const float& near,
                  const float& far)
{
    Mat4 projectionMatrix = Siege::Mat4::Identity;

    projectionMatrix[0] = 2.f / (right - left);
    projectionMatrix[5] = 2.f / (bottom - top);
    projectionMatrix[10] = 1.f / (far - near);
    projectionMatrix[12] = -(right + left) / (right - left);
    projectionMatrix[13] = -(bottom + top) / (bottom - top);
    projectionMatrix[14] = -near / (far - near);

    return projectionMatrix;
}

Mat4 SetViewDirection(const Vec3& position, const Vec3& direction, const Vec3& up)
{
    // Sets up an orthonormal basis - 3 vectors that are unit length (length of 1),
    // and are at 90 degree angles from one another.
    const Siege::Vec3 w {direction.Normalise()};
    const Siege::Vec3 u {w.Cross(up).Normalise()};
    const Siege::Vec3 v {w.Cross(u)};

    Mat4 viewMatrix = Siege::Mat4::Identity;

    viewMatrix[0] = u.x;
    viewMatrix[1] = u.y;
    viewMatrix[2] = u.z;
    viewMatrix[4] = v.x;
    viewMatrix[5] = v.y;
    viewMatrix[6] = v.z;
    viewMatrix[8] = w.x;
    viewMatrix[9] = w.y;
    viewMatrix[10] = w.z;
    viewMatrix[12] = -u.Dot(position);
    viewMatrix[13] = -v.Dot(position);
    viewMatrix[14] = -w.Dot(position);

    return viewMatrix;
}

Mat4 SetViewTarget(const Vec3& position, const Vec3& target, const Vec3& up)
{
    // Set the direction to the direction from the camera to the target.
    return SetViewDirection(position, target - position, up);
}

Mat4 SetViewYXZ(const Siege::Vec3& position, const Siege::Vec3& rotation)
{
    // Sets up a rotation matrix.
    const float c3 = Float::Cos(rotation.z);
    const float s3 = Float::Sin(rotation.z);
    const float c2 = Float::Cos(rotation.x);
    const float s2 = Float::Sin(rotation.x);
    const float c1 = Float::Cos(rotation.y);
    const float s1 = Float::Sin(rotation.y);

    const Vec3 u {(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
    const Vec3 v {(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
    const Vec3 w {(c2 * s1), (-s2), (c1 * c2)};

    Mat4 viewMatrix = Siege::Mat4::Identity;

    viewMatrix[0] = u.x;
    viewMatrix[4] = u.y;
    viewMatrix[8] = u.z;
    viewMatrix[1] = v.x;
    viewMatrix[5] = v.y;
    viewMatrix[9] = v.z;
    viewMatrix[2] = w.x;
    viewMatrix[6] = w.y;
    viewMatrix[10] = w.z;
    viewMatrix[12] = -u.Dot(position);
    viewMatrix[13] = -v.Dot(position);
    viewMatrix[14] = -w.Dot(position);

    return viewMatrix;
}

Mat4 CalculateTransform3D(const Vec3& position, const Vec3& rotation, const Vec3& scale)
{
    const float s3 = Float::Sin(rotation.z);
    const float c3 = Float::Cos(rotation.z);
    const float c2 = Float::Cos(rotation.x);
    const float s2 = Float::Sin(rotation.x);
    const float c1 = Float::Cos(rotation.y);
    const float s1 = Float::Sin(rotation.y);

    return {{
                scale.x * (c1 * c3 + s1 * s2 * s3),
                scale.x * (c2 * s3),
                scale.x * (c1 * s2 * s3 - c3 * s1),
                0.0f,
            },
            {
                scale.y * (c3 * s1 * s2 - c1 * s3),
                scale.y * (c2 * c3),
                scale.y * (c1 * c3 * s2 + s1 * s3),
                0.0f,
            },
            {
                scale.z * (c2 * s1),
                scale.z * (-s2),
                scale.z * (c1 * c2),
                0.0f,
            },
            {position.x, position.y, position.z, 1.0f}};
}

Mat3 CalculateTransform2D(const Vec2& position, const float& rotation, const Vec2& scale)
{
    const float s = Float::Sin(rotation);
    const float c = Float::Cos(rotation);
    Siege::Mat3 rotMatrix {{c, s, 0.f}, {-s, c, 0.f}, {0.f, 0.f, 0.f}};
    Siege::Mat3 scaleMat {{scale.x, .0f, 0.f}, {.0f, scale.y, 0.f}, {}};
    Siege::Mat3 combined = rotMatrix * scaleMat;
    combined[6] = position.x;
    combined[7] = position.y;
    combined[8] = 1.f;
    return combined;
}

Mat3 CalculateNormal(const Vec3& rotation, const Vec3& scale)
{
    const float c3 = Float::Cos(rotation.z);
    const float s3 = Float::Sin(rotation.z);
    const float c2 = Float::Cos(rotation.x);
    const float s2 = Float::Sin(rotation.x);
    const float c1 = Float::Cos(rotation.y);
    const float s1 = Float::Sin(rotation.y);

    Siege::Vec3 inverseScale = 1.0f / scale;

    return {{
                inverseScale.x * (c1 * c3 + s1 * s2 * s3),
                inverseScale.x * (c2 * s3),
                inverseScale.x * (c1 * s2 * s3 - c3 * s1),
            },
            {
                inverseScale.y * (c3 * s1 * s2 - c1 * s3),
                inverseScale.y * (c2 * c3),
                inverseScale.y * (c1 * c3 * s2 + s1 * s3),
            },
            {
                inverseScale.z * (c2 * s1),
                inverseScale.z * (-s2),
                inverseScale.z * (c1 * c2),
            }};
}

} // namespace Siege::Graphics