//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Camera.h"

#include <raylib/Camera3D.hpp>

#include "../TransitionAdapter.h"

static Siege::Vec3 pos(0.f, 10.f, 10.f);

static raylib::Camera3D& GetCamera()
{
    static raylib::Camera3D camera(FromVec3(pos),
                                   FromVec3(Siege::Vec3::Zero),
                                   FromVec3(Siege::Vec3::Up),
                                   45.f,
                                   CAMERA_PERSPECTIVE);
    return camera;
}

Cam::Cam()
{
    GetCamera().SetMode(CAMERA_CUSTOM);
}

void Cam::Begin3D()
{
    GetCamera().BeginMode();
}

void Cam::End3D()
{
    GetCamera().EndMode();
}

Siege::Vec3 Cam::GetScreenPos(const Siege::Vec3& position)
{
    return ToVec3(GetWorldToScreen(FromVec3(position + Siege::Vec3(0.f, 4.f, 0.f)), GetCamera()));
}

RayCast Cam::GetMouseRay()
{
    return ToRayCast(GetCamera().GetMouseRay(GetMousePosition()));
}

Siege::Vec3 Cam::GetPosition()
{
    return ToVec3(GetCamera().GetPosition());
}

void Cam::SetPosition(const Siege::Vec3& position)
{
    GetCamera().SetPosition(FromVec3(position));
}

void Cam::SetTarget(const Siege::Vec3& target)
{
    GetCamera().SetTarget(FromVec3(target));
}
