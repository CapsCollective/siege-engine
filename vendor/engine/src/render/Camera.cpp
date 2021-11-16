#include "Camera.h"
#include <raylib/Camera3D.hpp>

static Vec3 pos(0.f, 10.f, 10.f);

static raylib::Camera3D& GetCamera()
{
    static raylib::Camera3D camera(
            pos, Vec3::Zero, Vec3::Up, 45.f, CAMERA_PERSPECTIVE);
    return camera;
}

Cam::Cam()
{
    // TODO fix this ugly workaround
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

Vec3 Cam::GetScreenPos(const Vec3 &position)
{
    return GetWorldToScreen(position + Vec3(0.f, 4.f, 0.f), GetCamera());
}

RayCast Cam::GetMouseRay()
{
    return GetCamera().GetMouseRay(GetMousePosition());
}

Vec3 Cam::GetPosition()
{
    return GetCamera().GetPosition();
}

void Cam::SetPosition(const Vec3 &position)
{
    GetCamera().SetPosition(position);
}

void Cam::SetTarget(const Vec3 &target)
{
    GetCamera().SetTarget(target);
}
