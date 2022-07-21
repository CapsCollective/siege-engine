//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_GRAPHICS_H
#define SIEGE_ENGINE_GRAPHICS_H

#include "mat/Mat2.h"
#include "mat/Mat4.h"

namespace Siege
{
/**
 * A collection of functions for calculating common graphics related operations.
 */
class Graphics
{
public:

    /**
     * Returns a 4x4 matrix representing a perspective projection. Used for 3D projections.
     * @param fovy the Field of View (fov).
     * @param aspect the aspect ratio of the screen.
     * @param near the near clipping range.
     * @param far the far clipping range.
     * @return a 4x4 perspective projection matrix.
     */
    static Mat4 Perspective(const float& fovy,
                            const float& aspect,
                            const float& near,
                            const float& far);

    /**
     * Returns a 4x4 orthographic projection (typically used for 2D projections)
     * @param left the left-most extent of the projection.
     * @param right the right-most extent of the projection.
     * @param top the top-most extent of the projection.
     * @param bottom the bottom-most extent of the projection.
     * @param near the near clipping range.
     * @param far the far clipping range.
     * @return a 4x4 orthographic projection matrix.
     */
    static Mat4 Orthographic(const float& left,
                             const float& right,
                             const float& top,
                             const float& bottom,
                             const float& near,
                             const float& far);

    /**
     * Creates a projection matrix that's looking at a specific directioin.
     * @param position the position of the viewer.
     * @param direction the direction to point toward.
     * @param up the up direction to be used.
     * @return a 4x4 matrix set to 'look' at a direction in 3D space.
     */
    static Mat4 SetViewDirection(const Vec3& position,
                                 const Vec3& direction,
                                 const Vec3& up = {0.f, -1.f, 0.f});

    /**
     * Creates a projection matrix looking at a specific target.
     * @param position the position of the viewer object in 3D space.
     * @param target the position of the target being looked at.
     * @param up the up direction.
     * @return a 4x4 matrix set to 'look' at a targett in 3D space.
     */
    static Mat4 SetViewTarget(const Vec3& position,
                              const Vec3& target,
                              const Vec3& up = {0.f, -1.f, 0.f});

    /**
     * Returns a matrix representation of a viewer object that's rotated to look at a spot.
     * @param position the position of the viewer object.
     * @param rotation an euler angle representing the rotation on each axis.
     * @return a matrix representing the rotated perspective.
     */
    static Mat4 SetViewYXZ(const Siege::Vec3& position, const Siege::Vec3& rotation);

    /**
     * Calculates a 3D matrix representation of a position in space.
     * @param position the position of the object.
     * @param rotation the rotation of the object.
     * @param scale the scale of the object.
     * @return the transform matrix of the object's spatial positioning.
     */
    static Mat4 CalculateTransform3D(const Vec3& position, const Vec3& rotation, const Vec3& scale);

    /**
     * Calculates a 2D matrix representation of a position in space.
     * @param position the position of the object.
     * @param rotation the rotation of the object.
     * @param scale the scale of the object.
     * @return the transform matrix of the object's spatial positioning.
     */
    static Mat2 CalculateTransform2D(const Vec2& position,
                                     const float& rotation,
                                     const Vec2& scale);

    /**
     * Calculates the normal matrix of a 3D object.
     * @param rotation the rotation of the object (represented as an Euler Angle)
     * @param scale the scale of the object.
     * @return the normal matrix of the object.
     */
    static Mat3 CalculateNormal(const Vec3& rotation, const Vec3& scale);
};

} // namespace Siege

#endif // SIEGE_ENGINE_GRAPHICS_H
