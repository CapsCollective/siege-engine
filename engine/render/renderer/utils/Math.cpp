//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Math.h"

namespace Siege
{
Siege::Mat4 Math::CalculateTransform3D(const Siege::Vec3& position,
                                       const Siege::Vec3& rotation,
                                       const Siege::Vec3& scale)
{
    const float s3 = glm::sin(rotation.z);
    const float c3 = glm::cos(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);

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

Siege::Mat3 Math::CalculateNormalMatrix(const Siege::Vec3& rotation, const Siege::Vec3& scale)
{
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);

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
Siege::Mat2 Math::CalculateTransform2D(const Siege::Vec2& position,
                                       const float& rotation,
                                       const Siege::Vec2& scale)
{
    const float s = glm::sin(rotation);
    const float c = glm::cos(rotation);
    Siege::Mat2 rotMatrix {{c, s}, {-s, c}};

    Siege::Mat2 scaleMat {{scale.x, .0f}, {.0f, scale.y}};
    return rotMatrix * scaleMat;
}
} // namespace Siege