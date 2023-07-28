//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_EXAMPLE_FPSCAMERA_H
#define SIEGE_ENGINE_EXAMPLE_FPSCAMERA_H

#include <render/renderer/camera/Camera.h>
#include <utils/math/Maths.h>

class FPSCamera
{
public:

    FPSCamera() = default;
    ~FPSCamera() = default;

    FPSCamera(Siege::Vec3 position,
              Siege::Vec3 front,
              float fov,
              float width,
              float height,
              float near,
              float far);
    FPSCamera(Siege::Vec3 position,
              Siege::Vec3 front,
              float fov,
              float width,
              float height,
              float near);
    FPSCamera(Siege::Vec3 position, Siege::Vec3 front, float fov, float width, float height);
    FPSCamera(Siege::Vec3 position, Siege::Vec3 front, float fov, float width);
    FPSCamera(Siege::Vec3 position, Siege::Vec3 front, float fov);

    void Update(float fovy, float width, float height, float near, float far);

    void MoveCamera(float deltaTime);

    void LookAt(Siege::Vec3 target);

    inline const Siege::Camera& GetCamera() const
    {
        return cam;
    }

    Siege::RayCast GetMouseRay(float mouseX, float mouseY, float windowX, float windowY);

    inline Siege::Vec3 GetPosition()
    {
        return camPos;
    }

    inline void SetIsMovable(bool state)
    {
        isMoveable = state;
    }

    void SetAspectRatio(float aspect);

private:

    Siege::Vec3 camPos {};
    Siege::Vec3 camFront {};
    Siege::Vec3 camUp {Siege::Vec3::Up()};

    float pitch {0.f};
    float yaw {90.f};
    float fov {0.f};
    float aspectRatio {0.f};
    float near {0.1f};
    float far {1000.f};
    float viewWidth {0.f};
    float viewHeight {0.f};

    Siege::Camera cam;

    bool isMoveable {true};
};

#endif // SIEGE_ENGINE_EXAMPLE_FPSCAMERA_H
