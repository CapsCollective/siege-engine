//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "FPSCamera.h"

#include <utils/math/Projection.h>
#include <utils/math/Transform.h>
#include <window/Input.h>

void FPSCamera::Update(float fovy, float aspect, float near, float far)
{
    cam.projection = Siege::Perspective(fovy, aspect, near, far);
}

void FPSCamera::MoveCamera(float deltaTime)
{
    Siege::Vec2 mouse = {(float) Siege::Input::GetCursorPosition().x,
                         (float) Siege::Input::GetCursorPosition().y};

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

    const float sensitivity = 0.1f;

    float xDelta = (oldMousePos.x - mouse.x) * sensitivity;
    float yDelta = (mouse.y - oldMousePos.y) * sensitivity;

    yaw += xDelta;
    pitch = Siege::Float::Clamp(pitch + yDelta, -89.f, 89.f);

    Siege::Vec3 direction;
    direction.x = Siege::Float::Cos(Siege::Float::Radians(yaw)) *
                  Siege::Float::Cos(Siege::Float::Radians(pitch));
    direction.y = Siege::Float::Sin(Siege::Float::Radians(pitch));
    direction.z = Siege::Float::Sin(Siege::Float::Radians(yaw)) *
                  Siege::Float::Cos(Siege::Float::Radians(pitch));
    camFront = Siege::Vec3::Normalise(direction);

    cam.view = Siege::LookAt(camPos, camPos + camFront);

    oldMousePos = mouse;
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
