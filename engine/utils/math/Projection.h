//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_UTILS_MATH_PROJECTION_H
#define SIEGE_ENGINE_UTILS_MATH_PROJECTION_H

#include "mat/Mat4.h"
#include "vec/Vec2.h"
#include "vec/Vec3.h"

namespace Siege
{
/**
 * Creates a right-handed perspective matrix
 * @param fovy the vertical field of view of the perspective projection
 * @param aspect the aspect ratio of the perspective view
 * @param zNear the zNear plane
 * @param zFar the zFar plane
 * @return a new matrix with the provided perspective values
 */
inline constexpr Mat4 Perspective(const float fovy,
                                  const float aspect,
                                  const float zNear,
                                  const float zFar)
{
    const float tanHalfFovy = std::tan(fovy / 2.f);

    Mat4 result = Mat4::Zero();
    result[0][0] = 1.f / (aspect * tanHalfFovy);
    result[1][1] = -1.f / tanHalfFovy;
    result[2][2] = zFar / (zNear - zFar);
    result[2][3] = -1.f;
    result[3][2] = -(zFar * zNear) / (zFar - zNear);
    return result;
}

/**
 * Creates a right-handed orthographic projection
 * @param left the left side of the screen
 * @param right the right side (the far edge of the screen)
 * @param bottom the bottom of the screen
 * @param top the top of the screen
 * @param zNear the near clipping plane
 * @param zFar the far clipping plane
 * @return a new matrix with the orthographic projection
 */
inline constexpr Mat4 Orthographic(float left,
                                   float right,
                                   float bottom,
                                   float top,
                                   float zNear,
                                   float zFar)
{
    Mat4 result(1);
    result[0][0] = 2.f / (right - left);
    result[1][1] = -2.f / (bottom - top);
    result[2][2] = -1.f / (zFar - zNear);
    result[3][0] = -(right + left) / (right - left);
    result[3][1] = -(bottom + top) / (bottom - top);
    result[3][2] = -zNear / (zFar - zNear);
    return result;
}

/**
 * Un-projects a point from 2D view space to 3D world space
 * @param viewMatrix the view matrix to un-project from
 * @param projection the projection matrix used by the scene
 * @param ndc the normalised device coordinates (-1 to 1 on the x and y coordinates and 0 to 1 on
 * the z coordinates)
 * @return a 3D vector specifying the position in 3D space
 */
inline constexpr Vec3 UnProjectPoint3D(const Mat4& viewMatrix,
                                       const Mat4 projection,
                                       const Vec4& ndc)
{
    Mat4 inv = Mat4::Inverse(projection * viewMatrix);

    Vec4 unProjected = inv * Vec4 {ndc.XYZ(), 1.f};
    return unProjected.XYZ() / unProjected.w;
}

/**
 * Converts a 3D coordinate to screen space
 * @param worldCoords the position in 3D space
 * @param viewProjection the combined view + projection matrix
 * @param width the width of the viewport
 * @param height the height of the viewport
 * @return a 2D vector specifying the position in screen space
 */
inline constexpr Vec2 WorldToScreen(const Vec3& worldCoords,
                                    const Mat4& viewProjection,
                                    float width,
                                    float height)
{
    Siege::Vec4 clipCoords = viewProjection * Siege::Vec4 {worldCoords.XYZ(), 1.f};
    Siege::Vec3 coords = clipCoords.XYZ() / clipCoords.w;

    float x = ((coords.x + 1.f) * 0.5f) * width;
    float y = ((coords.y + 1.f) * 0.5f) * height;

    return {x, y};
}
} // namespace Siege

#endif // SIEGE_ENGINE_UTILS_MATH_PROJECTION_H
