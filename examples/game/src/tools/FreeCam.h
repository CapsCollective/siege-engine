//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_FREECAM_H
#define SIEGE_ENGINE_FREECAM_H

#include <core/entity/Entity.h>

#include "../ServiceLocator.h"

class FreeCam : public Siege::Entity
{
public:

    // 'Structors

    FreeCam() :
        moveSpeed(10.f),
        lookSpeed(0.1f),
        camera(ServiceLocator::GetFPSCamera()),
        rotation(Siege::Vec3 {0.f, -1.f, -1.f}),
        previousMousePosition({0.f, 0.f})
    {}

    void OnDraw3D() override;

protected:

    // Protected overrides

    void OnUpdate() override;

private:

    // Private fields

    float moveSpeed;

    float lookSpeed;

    FPSCamera* camera;

    Siege::Vec3 rotation;

    Siege::MousePosition previousMousePosition;
};

#endif // SIEGE_ENGINE_FREECAM_H
