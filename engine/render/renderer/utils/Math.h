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
#include <glm/glm.hpp>

namespace Siege::Utils
{
class Math
{
public:

    static glm::mat4 CalculateTransform3D(const glm::vec3& position,
                                          const glm::vec3& rotation,
                                          const glm::vec3& scale);
    // TODO use this function or remove it
    [[maybe_unused]] static glm::mat2 CalculateTransform2D(const glm::vec2& position,
                                                           const float& rotation,
                                                           const glm::vec2& scale);
    static glm::mat3 CalculateNormalMatrix(const glm::vec3& rotation, const glm::vec3& scale);
};
} // namespace Siege::Utils
