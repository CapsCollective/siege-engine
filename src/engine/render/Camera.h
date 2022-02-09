#ifndef A_DARK_DISCOMFORT_CAMERA_H
#define A_DARK_DISCOMFORT_CAMERA_H

#include <utils/Maths.h>

class Cam
{
public:

    Cam();

    void Begin3D();

    void End3D();

    Vec3 GetScreenPos(const Vec3& position);

    RayCast GetMouseRay();

    Vec3 GetPosition();

    void SetPosition(const Vec3& position);

    void SetTarget(const Vec3& target);

private:
};

#endif // A_DARK_DISCOMFORT_CAMERA_H
