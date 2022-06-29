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
    PointLight(Vec3::Zero, glm::vec4(1.0f, 1.0f, 1.0f, 0.2f), glm::vec4(1.f, 1.f, 1.f, .2f))
{}

PointLight::PointLight(Vec3 position, glm::vec4 color, glm::vec4 ambientColor) :
    lightData {color, ambientColor, position}
{}

PointLight::~PointLight() {}
} // namespace Siege