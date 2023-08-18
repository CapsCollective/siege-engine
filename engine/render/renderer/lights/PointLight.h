//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_POINT_LIGHT_H
#define SIEGE_ENGINE_POINT_LIGHT_H

#include <utils/Colour.h>
#include <utils/math/vec/Vec3.h>

namespace Siege
{

class PointLight
{
public:

    struct Data
    {
        FColour lightColor = {1.f, 1.f, 1.f, 0.2f};
        alignas(16) FColour ambientLightColor = {1.f, 1.f, 1.f, .02f};
        alignas(16) Vec3 position = Vec3::Zero();
    };

    PointLight();
    PointLight(Vec3 position, FColour color, FColour ambientColor);
    ~PointLight();

    Data& GetLightData()
    {
        return lightData;
    }

    void SetPosition(Vec3 position)
    {
        lightData.position = position;
    }
    void SetColor(FColour color)
    {
        lightData.lightColor = color;
    }
    void SetAmbientColor(FColour ambientColor)
    {
        lightData.ambientLightColor = ambientColor;
    }

private:

    Data lightData;
};
} // namespace Siege

#endif