//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_UTILS_MATH_TRANSFORM_H
#define SIEGE_ENGINE_UTILS_MATH_TRANSFORM_H

#include <cmath>

#include "mat/Mat4.h"
#include "vec/Vec2.h"
#include "vec/Vec3.h"

namespace Siege
{
/**
 * Creates a view matrix which is pointing at a specific location in 3D space
 * @param eye the position of the camera
 * @param center the location the camera is pointing at
 * @param up the global up direction in world space (defaults to -1 on the y axis)
 * @return a new 4x4 matrix with the view matrix values
 */
inline constexpr Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up = Vec3::Up())
{
    Vec3 const f(Vec3::Normalise(center - eye));
    Vec3 const s(Vec3::Normalise(Vec3::Cross(f, up)));
    Vec3 const u(Vec3::Cross(s, f));

    Mat4 result {1};
    result[0][0] = s.x;
    result[1][0] = s.y;
    result[2][0] = s.z;
    result[0][1] = u.x;
    result[1][1] = u.y;
    result[2][1] = u.z;
    result[0][2] = -f.x;
    result[1][2] = -f.y;
    result[2][2] = -f.z;
    result[3][0] = -Vec3::Dot(s, eye);
    result[3][1] = -Vec3::Dot(u, eye);
    result[3][2] = Vec3::Dot(f, eye);
    return result;
}

/**
 * Creates a translation matrix. A translation matrix specifies a movement in space
 * @param matrix the 4x4 matrix to translate
 * @param position the position to move the matrix to
 * @return a new matrix with the provided translation
 */
inline constexpr Mat4 Translate(const Mat4& matrix, const Vec3& position)
{
    Mat4 result = matrix;
    result[3] =
        matrix[0] * position[0] + matrix[1] * position[1] + matrix[2] * position[2] + matrix[3];
    return result;
}

/**
 * Creates a rotation matrix
 * @param matrix the matrix to rotate
 * @param angle the angle of rotation
 * @param rotation the axis to rotate the matrix on (specified by using 1 for the axis)
 * @return a new matrix specifying the rotation
 */
inline constexpr Mat4 Rotate(const Mat4& matrix, float angle, const Vec3& rotation)
{
    float const a = angle;
    float const c = cos(a);
    float const s = sin(a);

    Vec3 axis(Vec3::Normalise(rotation));
    Vec3 temp((1 - c) * axis);

    Mat4 rotate;
    rotate[0][0] = c + temp[0] * axis[0];
    rotate[0][1] = temp[0] * axis[1] + s * axis[2];
    rotate[0][2] = temp[0] * axis[2] - s * axis[1];

    rotate[1][0] = temp[1] * axis[0] - s * axis[2];
    rotate[1][1] = c + temp[1] * axis[1];
    rotate[1][2] = temp[1] * axis[2] + s * axis[0];

    rotate[2][0] = temp[2] * axis[0] + s * axis[1];
    rotate[2][1] = temp[2] * axis[1] - s * axis[0];
    rotate[2][2] = c + temp[2] * axis[2];

    Mat4 result;
    result[0] = matrix[0] * rotate[0][0] + matrix[1] * rotate[0][1] + matrix[2] * rotate[0][2];
    result[1] = matrix[0] * rotate[1][0] + matrix[1] * rotate[1][1] + matrix[2] * rotate[1][2];
    result[2] = matrix[0] * rotate[2][0] + matrix[1] * rotate[2][1] + matrix[2] * rotate[2][2];
    result[3] = matrix[3];
    return result;
}

/**
 * Creates a scale matrix
 * @param matrix the matrix to scale
 * @param scale the dimensions to scale the matrix by
 * @return a matrix with the new scaling factors
 */
inline constexpr Mat4 Scale(const Mat4& matrix, const Vec3 scale)
{
    Mat4 result;
    result[0] = matrix[0] * scale.x;
    result[1] = matrix[1] * scale.y;
    result[2] = matrix[2] * scale.z;
    result[3] = matrix[3];
    return result;
}

/**
 * Creates a transform matrix in 3D space
 * @param position the position in 3D space
 * @param rotation the rotation in 3D space
 * @param scale the scale in 3D space
 * @return a new 4x4 matrix specifying the 3D transform
 */
inline constexpr Mat4 Transform3D(const Vec3& position, const Vec3& rotation, const Vec3& scale)
{
    const float cosR = Float::Cos(rotation.z);
    const float sinR = Float::Sin(rotation.z);
    const float cosP = Float::Cos(rotation.x);
    const float sinP = Float::Sin(rotation.x);
    const float cosY = Float::Cos(rotation.y);
    const float sinY = Float::Sin(rotation.y);

    return {{scale.x * (cosY * cosR + sinY * sinP * sinR),
             scale.x * (cosP * sinR),
             scale.x * (cosY * sinP * sinR - cosR * sinY)},
            {scale.y * (cosR * sinY * sinP - cosY * sinR),
             scale.y * (cosP * cosR),
             scale.y * (cosY * cosR * sinP + sinY * sinR)},
            {scale.z * (cosP * sinY), scale.z * (-sinP), scale.z * (cosY * cosP)},
            {position.XYZ(), 1.0f}};
}

/**
 * Calculates the normal matrix for a 3D object
 * @param rotation the rotation of the object
 * @param scale the scale of the object
 * @return a 4x4 matrix specifying the object normals
 */
inline constexpr Mat4 Normal(const Vec3& rotation, const Vec3& scale)
{
    const float cosR = Float::Cos(rotation.z);
    const float sinR = Float::Sin(rotation.z);
    const float cosP = Float::Cos(rotation.x);
    const float sinP = Float::Sin(rotation.x);
    const float cosY = Float::Cos(rotation.y);
    const float SinY = Float::Sin(rotation.y);

    Siege::Vec3 inverseScale = 1.0f / scale;

    return {{
                inverseScale.x * (cosY * cosR + SinY * sinP * sinR),
                inverseScale.x * (cosP * sinR),
                inverseScale.x * (cosY * sinP * sinR - cosR * SinY),
            },
            {
                inverseScale.y * (cosR * SinY * sinP - cosY * sinR),
                inverseScale.y * (cosP * cosR),
                inverseScale.y * (cosY * cosR * sinP + SinY * sinR),
            },
            {
                inverseScale.z * (cosP * SinY),
                inverseScale.z * (-sinP),
                inverseScale.z * (cosY * cosP),
            }};
}
} // namespace Siege

#endif // SIEGE_ENGINE_UTILS_MATH_TRANSFORM_H
