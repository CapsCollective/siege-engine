//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "FPSCamera.h"

#include <utils/math/Projection.h>
#include <utils/math/Transform.h>
#include <window/Input.h>

#define DEFAULT_NEAR_POINT 0.1f
#define DEFAULT_FAR_POINT 1000.f
#define DEFAULT_VIEW_WIDTH 800.f
#define DEFAULT_VIEW_HEIGHT 600.f

FPSCamera::FPSCamera(Siege::Vec3 position,
                     Siege::Vec3 front,
                     float fov,
                     float width,
                     float height,
                     float near,
                     float far) :
    camPos {position},
    camFront {front},
    fov {fov},
    aspectRatio {width / height},
    near {near},
    far {far},
    viewWidth {width},
    viewHeight {height}
{
    cam.view = Siege::LookAt(camPos, camPos + camFront);
    cam.projection = Siege::Perspective(fov, viewWidth / viewHeight, near, far);
}

FPSCamera::FPSCamera(Siege::Vec3 position,
                     Siege::Vec3 front,
                     float fov,
                     float width,
                     float height,
                     float near) :
    FPSCamera(position, front, fov, width, height, near, DEFAULT_FAR_POINT)
{}

FPSCamera::FPSCamera(Siege::Vec3 position,
                     Siege::Vec3 front,
                     float fov,
                     float width,
                     float height) :
    FPSCamera(position, front, fov, width, height, DEFAULT_NEAR_POINT)
{}

FPSCamera::FPSCamera(Siege::Vec3 position, Siege::Vec3 front, float fov, float width) :
    FPSCamera(position, front, fov, width, DEFAULT_VIEW_HEIGHT)
{}

FPSCamera::FPSCamera(Siege::Vec3 position, Siege::Vec3 front, float fov) :
    FPSCamera(position, front, fov, DEFAULT_VIEW_WIDTH)
{}

void FPSCamera::Update(float fovy, float width, float height, float near, float far)
{
    cam.projection = Siege::Perspective(fovy, width / height, near, far);
}

void FPSCamera::LookAt(Siege::Vec3 target)
{
    Siege::Vec3 direction = target - camPos;
    cam.view = Siege::LookAt(camPos, camPos + direction);
}

void FPSCamera::MoveCamera(float deltaTime)
{
    if (!isMoveable) return;

    Siege::Vec2 mouseDir = Siege::Input::GetMouseDirection();

    const float sensitivity = 0.1f;

    yaw += mouseDir.x * sensitivity;
    pitch = Siege::Float::Clamp(pitch + (mouseDir.y * sensitivity), -89.f, 89.f);

    const float camSpeed = deltaTime * 2.5f;

    Siege::Vec3 camRight = Siege::Vec3::Normalise(Siege::Vec3::Cross(camFront, camUp));
    Siege::Vec3 camMoveDir = Siege::Vec3::Zero();

    if (Siege::Input::IsKeyDown(Siege::KEY_W)) camMoveDir += {camFront.x, 0, camFront.z};
    if (Siege::Input::IsKeyDown(Siege::KEY_S)) camMoveDir -= {camFront.x, 0, camFront.z};
    if (Siege::Input::IsKeyDown(Siege::KEY_A)) camMoveDir -= camRight;
    if (Siege::Input::IsKeyDown(Siege::KEY_D)) camMoveDir += camRight;
    if (Siege::Input::IsKeyDown(Siege::KEY_Q)) camMoveDir -= camUp;
    if (Siege::Input::IsKeyDown(Siege::KEY_E)) camMoveDir += camUp;

    camPos += Siege::Vec3::Normalise(camMoveDir) * camSpeed;

    Siege::Vec3 direction;
    direction.x = Siege::Float::Cos(Siege::Float::Radians(yaw)) *
                  Siege::Float::Cos(Siege::Float::Radians(pitch));
    direction.y = Siege::Float::Sin(Siege::Float::Radians(pitch));
    direction.z = Siege::Float::Sin(Siege::Float::Radians(yaw)) *
                  Siege::Float::Cos(Siege::Float::Radians(pitch));
    camFront = Siege::Vec3::Normalise(direction);

    cam.view = Siege::LookAt(camPos, camPos + camFront);
}

Siege::RayCast FPSCamera::GetMouseRay(float mouseX, float mouseY, float windowX, float windowY)
{
    Siege::Vec2 cursorPos = {mouseX, mouseY};

    // Normalise the device coordinates to be between -1 (left/top) and 1 (right/bottom)
    Siege::Vec2 deviceCoords = {((float) cursorPos.x * 2.f) / windowX - 1.f,
                                ((float) cursorPos.y * 2.f) / windowY - 1.f};

    Siege::Mat4 proj =
        Siege::Perspective(Siege::Float::Radians(60.f), windowX / windowY, 0.1f, 1000.f);

    Siege::Vec3 rayNear = Siege::UnProjectPoint3D(cam.view, proj, {deviceCoords.XY(), 0.f, 1.f});
    Siege::Vec3 rayFar = Siege::UnProjectPoint3D(cam.view, proj, {deviceCoords.XY(), 1.f, 1.f});

    Siege::Vec3 direction = Siege::Vec4::Normalise(rayFar - rayNear);

    return {{camPos.x, camPos.y, camPos.z}, {direction.x, direction.y, direction.z}};
}

void FPSCamera::SetAspectRatio(float aspect)
{
    aspectRatio = aspect;
    cam.projection = Siege::Perspective(fov, aspectRatio, near, far);
}
