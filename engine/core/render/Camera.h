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

class Cam
{
public:

    Cam();

    void Begin3D();

    void End3D();

    Siege::Vec::Vec3 GetScreenPos(const Siege::Vec::Vec3& position);

    RayCast GetMouseRay();

    Siege::Vec::Vec3 GetPosition();

    void SetPosition(const Siege::Vec::Vec3& position);

    void SetTarget(const Siege::Vec::Vec3& target);

private:
};

#endif // SIEGE_ENGINE_CAMERA_H
