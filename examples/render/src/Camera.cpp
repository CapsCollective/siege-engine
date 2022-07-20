//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Camera.h"

#include <render/input/Input.h>

void Camera::UpdatePerspectiveProjection(const float& fovy,
                                         const float& aspect,
                                         const float& near,
                                         const float& far)
{
    projectionMatrix = Siege::Graphics::Perspective(fovy, aspect, near, far);
}

void Camera::UpdateOthrographicProjection(const float& left,
                                          const float& right,
                                          const float& top,
                                          const float& bottom,
                                          const float& near,
                                          const float& far)
{
    projectionMatrix = Siege::Graphics::Orthographic(left, right, top, bottom, near, far);
}

void Camera::SetView(const Siege::Vec3& position, const Siege::Vec3& rotation)
{
    viewMatrix = Siege::Graphics::SetViewYXZ(position, rotation);
}

void Camera::MoveCamera(float deltaTime)
{
    static auto oldMousePos = Input::GetCursorPosition();
    auto mousePos = Input::GetCursorPosition();

    Siege::Vec3 rotate {};
    float lookSpeed = 4.0f;

    double differenceX = mousePos.x - oldMousePos.x;
    double differenceY = oldMousePos.y - mousePos.y;

    rotate.y += float(differenceX);
    rotate.x += float(differenceY);

    if (rotate.Dot(rotate) > Siege::Float::Epsilon())
    {
        Siege::Vec3 newRotation = obj.GetRotation() + lookSpeed * rotate.Normalise();
        obj.SetRotation(Siege::Vec3::Lerp(obj.GetRotation(), newRotation, deltaTime));
    }

    // Limit the pitch values to avoid objects rotating upside-down.
    obj.SetRotationX(Siege::Float::Clamp(obj.GetRotation().x, -1.5f, 1.5f));
    obj.SetRotationY(Siege::Float::Mod(obj.GetRotation().y, Siege::Float::TwoPi));

    float yaw = obj.GetRotation().y;
    const Siege::Vec3 forwardDir {Siege::Float::Sin(yaw), 0.f, Siege::Float::Cos(yaw)};
    const Siege::Vec3 rightDir {forwardDir.z, 0.f, -forwardDir.x};
    const Siege::Vec3 upDir {0.f, -1.f, 0.f};

    Siege::Vec3 moveDir {0.f};
    if (Input::IsKeyDown(KEY_W)) moveDir += forwardDir;
    if (Input::IsKeyDown(KEY_S)) moveDir -= forwardDir;
    if (Input::IsKeyDown(KEY_A)) moveDir -= rightDir;
    if (Input::IsKeyDown(KEY_D)) moveDir += rightDir;

    if (Input::IsKeyDown(KEY_E)) moveDir += upDir;
    if (Input::IsKeyDown(KEY_Q)) moveDir -= upDir;

    float moveSpeed = 2.f;

    if (moveDir.Dot(moveDir) > Siege::Float::Epsilon())
    {
        Siege::Vec3 newMove = obj.GetPosition() + moveSpeed * moveDir.Normalise();
        obj.SetPosition(Siege::Vec3::Lerp(obj.GetPosition(), newMove, deltaTime));
    }

    oldMousePos = mousePos;
}

void Camera::SetPosition(const Siege::Vec3& position)
{
    obj.SetPosition(position);
}

void Camera::Update()
{
    SetView(obj.GetPosition(), obj.GetRotation());
}