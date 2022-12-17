//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDERER_MATH_H
#define SIEGE_ENGINE_RENDERER_MATH_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <utils/mat/Mat2.h>
#include <utils/mat/Mat3.h>
#include <utils/mat/Mat4.h>

#include <glm/glm.hpp>

// TODO(Aryeh): Move this to utils.

namespace Siege
{
class Math
{
public:

    static Mat4 CalculateTransform3D(const Vec3& position, const Vec3& rotation, const Vec3& scale);
    // TODO use this function or remove it
    [[maybe_unused]] static Mat2 CalculateTransform2D(const Vec2& position,
                                                      const float& rotation,
                                                      const Vec2& scale);
    static Mat3 CalculateNormalMatrix(const Vec3& rotation, const Vec3& scale);
};
} // namespace Siege

#endif
