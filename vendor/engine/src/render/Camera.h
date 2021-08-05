#ifndef A_DARK_DISCOMFORT_CAMERA_H
#define A_DARK_DISCOMFORT_CAMERA_H

#include "../utils/Maths.h"
#include <raylib/Camera3D.hpp>

class Cam
{
public:

    Cam() :
        camera(Vec3(0.f, 10.f, 10.f),
               Vec3::Zero, Vec3::Up, 45.f, CAMERA_PERSPECTIVE)
    {
        camera.SetMode(CAMERA_CUSTOM);
    }

    void Begin3D()
    {
        camera.BeginMode();
    }

    void End3D()
    {
        camera.EndMode();
    }

    Vec3 GetScreenPos(const Vec3& position)
    {
        return GetWorldToScreen(position + Vec3(0.f, 4.f, 0.f), camera);
    }

    RayCast GetMouseRay()
    {
        return camera.GetMouseRay(GetMousePosition());
    }

    Vec3 GetPosition()
    {
        return camera.GetPosition();
    }

    void SetPosition(const Vec3& position)
    {
        camera.SetPosition(position);
    }

    void SetTarget(const Vec3& target)
    {
        camera.SetTarget(target);
    }

private:

    raylib::Camera3D camera;
};


#endif //A_DARK_DISCOMFORT_CAMERA_H
