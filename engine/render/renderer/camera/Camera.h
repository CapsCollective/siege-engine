//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#pragma once

#include "../Core.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <utils/mat/Mat4x4.h>
#include <utils/vec/Vec3.h>

#include <glm/glm.hpp>

namespace Siege
{
class Camera
{
public:

    struct GPUCameraData
    {
        Siege::Mat4x4 projection;
        Siege::Mat4x4 view;
        Siege::Mat4x4 projView;
    };

    void SetOrthographicProjection(const float& left,
                                   const float& right,
                                   const float& top,
                                   const float& bottom,
                                   const float& near,
                                   const float& far);

    void SetPerspectiveProjection(const float& fovy,
                                  const float& aspect,
                                  const float& near,
                                  const float& far);

    void SetViewDirection(const Siege::Vec3& position,
                          const Siege::Vec3& direction,
                          const Siege::Vec3& up = {0.f, -1.f, 0.f});

    void SetViewTarget(const Siege::Vec3& position,
                       const Siege::Vec3& target,
                       const Siege::Vec3& up = {0.f, -1.f, 0.f});

    void SetViewYXZ(const Siege::Vec3& position, const Siege::Vec3& rotation);

    const Siege::Mat4x4& GetProjection()
    {
        return projectionMatrix;
    }
    const Siege::Mat4x4& GetView()
    {
        return viewMatrix;
    }
    Siege::Mat4x4 GetProjView()
    {
        return projectionMatrix * viewMatrix;
    }
    GPUCameraData GetCameraData()
    {
        return {projectionMatrix, viewMatrix, GetProjView()};
    }

private:

    Siege::Mat4x4 projectionMatrix {1.f};
    Siege::Mat4x4 viewMatrix {1.f};
};
} // namespace Siege