//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "PointLight.h"

namespace Siege
{

PointLight::PointLight() : PointLight(Vec3::Zero(), {1.0f, 1.0f, 1.0f, 0.2f}, {1.f, 1.f, 1.f, .2f})
{}

PointLight::PointLight(Vec3 position, FColour color, FColour ambientColor) :
    lightData {color, ambientColor, position}
{}

PointLight::~PointLight() {}
} // namespace Siege