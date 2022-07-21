//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "PointLight.h"

namespace Siege
{

PointLight::PointLight() :
    PointLight(Siege::Vec3::Zero, {1.0f, 1.0f, 1.0f, 0.2f}, {1.f, 1.f, 1.f, .2f})
{}

PointLight::PointLight(Siege::Vec3 position, Siege::Vec4 colour, Siege::Vec4 ambientColor) :
    lightData {colour, ambientColor, position}
{}

PointLight::~PointLight() {}
} // namespace Siege