//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDER_CAMERA_H
#define SIEGE_ENGINE_RENDER_CAMERA_H

#include "../Core.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <utils/mat/Mat4.h>
#include <utils/vec/Vec3.h>

#include <glm/glm.hpp>

namespace Siege
{
class Camera
{
public:

    struct GPUCameraData
    {
        Mat4 projection;
        Mat4 view;
        Mat4 projView;
    };

    void SetOrthographicProjection(float left,
                                   float right,
                                   float top,
                                   float bottom,
                                   float near,
                                   float far);

    void SetPerspectiveProjection(float fovy, float aspect, float near, float far);

    void SetViewDirection(const Vec3& position,
                          const Vec3& direction,
                          const Vec3& up = {0.f, -1.f, 0.f});

    void SetViewTarget(const Vec3& position, const Vec3& target, const Vec3& up = {0.f, -1.f, 0.f});

    void SetViewYXZ(const Vec3& position, const Vec3& rotation);

    const Mat4& GetProjection()
    {
        return projectionMatrix;
    }
    const Mat4& GetView()
    {
        return viewMatrix;
    }
    Mat4 GetProjView()
    {
        return projectionMatrix * viewMatrix;
    }
    GPUCameraData GetCameraData()
    {
        return {projectionMatrix, viewMatrix, GetProjView()};
    }

private:

    Mat4 projectionMatrix {1.f};
    Mat4 viewMatrix {1.f};
};
} // namespace Siege

#endif