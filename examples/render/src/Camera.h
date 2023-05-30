//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_CAMERA_H
#define SIEGE_ENGINE_CAMERA_H

#include <utils/math/Graphics.h>

#include "GameObject.h"

class Camera
{
public:

    Camera() = default;
    ~Camera() = default;

    void UpdatePerspectiveProjection(const float& fovy,
                                     const float& aspect,
                                     const float& near,
                                     const float& far);

    void UpdateOrthographicProjection(const float& left,
                                      const float& right,
                                      const float& top,
                                      const float& bottom,
                                      const float& near,
                                      const float& far);

    void SetView(const Siege::Vec3& position, const Siege::Vec3& rotation);
    void Update();

    const Siege::Mat4& GetProjection() const
    {
        return projectionMatrix;
    }
    const Siege::Mat4& GetView() const
    {
        return viewMatrix;
    }

    void MoveCamera(float deltaTime);

    void SetPosition(const Siege::Vec3& position);

private:

    Siege::Mat4 projectionMatrix {1.f};
    Siege::Mat4 viewMatrix {1.f};
    GameObject obj;
};

#endif // SIEGE_ENGINE_CAMERA_H
