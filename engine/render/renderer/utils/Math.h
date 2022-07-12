//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <utils/mat/Mat2x2.h>
#include <utils/mat/Mat3x3.h>
#include <utils/mat/Mat4x4.h>

#include <glm/glm.hpp>

namespace Siege
{
class Math
{
public:

    static Siege::Mat4x4 CalculateTransform3D(const Siege::Vec3& position,
                                              const Siege::Vec3& rotation,
                                              const Siege::Vec3& scale);
    // TODO use this function or remove it
    [[maybe_unused]] static Siege::Mat2x2 CalculateTransform2D(const Siege::Vec2& position,
                                                               const float& rotation,
                                                               const Siege::Vec2& scale);
    static Siege::Mat3x3 CalculateNormalMatrix(const Siege::Vec3& rotation,
                                               const Siege::Vec3& scale);
};
} // namespace Siege
