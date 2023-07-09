//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_EXAMPLE_FPSCAMERA_H
#define SIEGE_ENGINE_EXAMPLE_FPSCAMERA_H

#include <utils/math/Maths.h>

#include "GameObject.h"
#include "render/renderer/camera/Camera.h"

class FPSCamera
{
public:

    FPSCamera() = default;
    ~FPSCamera() = default;

    FPSCamera(Siege::Vec3 position, Siege::Vec3 front) : camPos {position}, camFront {front} {}

    void Update(float fovy, float aspect, float near, float far);

    void MoveCamera(float deltaTime);

    inline const Siege::Camera& GetCamera() const
    {
        return cam;
    }

    Siege::RayCast GetMouseRay(float mouseX, float mouseY, float windowX, float windowY);

private:

    Siege::Vec2 oldMousePos {};
    Siege::Vec3 camPos {};
    Siege::Vec3 camFront {};
    Siege::Vec3 camUp {Siege::Vec3::Up()};

    float pitch {0.f};
    float yaw {0.f};

    Siege::Camera cam;
};

#endif // SIEGE_ENGINE_EXAMPLE_FPSCAMERA_H
