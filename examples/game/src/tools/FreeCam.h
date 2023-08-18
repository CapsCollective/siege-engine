//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
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

    FreeCam() : camera(ServiceLocator::GetFPSCamera()), rotation(Siege::Vec3 {0.f, -1.f, -1.f}) {}

    void OnDraw3D() override;

protected:

    // Protected overrides

    void OnUpdate() override;

private:

    // Private fields

    FPSCamera* camera;

    Siege::Vec3 rotation;
};

#endif // SIEGE_ENGINE_FREECAM_H
