//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Camera.h"

#include <glm/glm.hpp>
#include <limits>

namespace Siege
{
void Camera::SetOrthographicProjection(float left,
                                       float right,
                                       float top,
                                       float bottom,
                                       float near,
                                       float far)
{
    projectionMatrix = Mat4::Identity;
    projectionMatrix[0] = 2.f / (right - left);
    projectionMatrix[5] = 2.f / (bottom - top);
    projectionMatrix[10] = 1.f / (far - near);
    projectionMatrix[12] = -(right + left) / (right - left);
    projectionMatrix[13] = -(bottom + top) / (bottom - top);
    projectionMatrix[14] = -near / (far - near);
}

void Camera::SetPerspectiveProjection(float fovy, float aspect, float near, float far)
{
    CC_ASSERT(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f, "");

    const float tanHalfFovy = tan(fovy / 2.f);
    projectionMatrix = {};
    projectionMatrix[0] = 1.f / (aspect * tanHalfFovy);
    projectionMatrix[5] = 1.f / (tanHalfFovy);
    projectionMatrix[10] = far / (far - near);
    projectionMatrix[11] = 1.f;
    projectionMatrix[14] = -(far * near) / (far - near);
}

void Camera::SetViewDirection(const Vec3& position, const Vec3& direction, const Vec3& up)
{
    // Sets up an orthonormal basis - 3 vectors that are unit length (length of 1),
    // and are at 90 degree angles from one another.
    const Vec3 w {direction.Normalise()};
    const Vec3 u {w.Cross(up).Normalise()};
    const Vec3 v {w.Cross(u)};

    viewMatrix = Mat4::Identity;
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
}

void Camera::SetViewTarget(const Vec3& position, const Vec3& target, const Vec3& up)
{
    // Set the direction to the direction from the camera to the target.
    SetViewDirection(position, target - position, up);
}

void Camera::SetViewYXZ(const Vec3& position, const Vec3& rotation)
{
    // Sets up a rotation matrix.
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);
    const Vec3 u {(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
    const Vec3 v {(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
    const Vec3 w {(c2 * s1), (-s2), (c1 * c2)};
    viewMatrix = Mat4::Identity;
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
}
} // namespace Siege