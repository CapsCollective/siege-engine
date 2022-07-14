//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_CAMERA_H
#define SIEGE_ENGINE_CAMERA_H

#include <utils/Maths.h>

namespace Siege
{
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
}

#endif // SIEGE_ENGINE_CAMERA_H
