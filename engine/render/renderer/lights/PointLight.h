//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_POINT_LIGHT_H
#define SIEGE_ENGINE_POINT_LIGHT_H

#include "../Core.h"
#include "../model/Model.h"

namespace Siege
{

class PointLight
{
public:

    struct Data
    {
        Vec4 lightColor = {1.f, 1.f, 1.f, 0.2f};
        alignas(16) Vec4 ambientLightColor = {1.f, 1.f, 1.f, .02f};
        alignas(16) Vec3 position = Vec3::Zero;
    };

    PointLight();
    PointLight(Vec3 position, Vec4 color, Vec4 ambientColor);
    ~PointLight();

    Data& GetLightData()
    {
        return lightData;
    }
    Model* GetModel()
    {
        return model;
    }
    void SetPosition(Vec3 position)
    {
        lightData.position = position;
    }
    void SetColor(Vec4 color)
    {
        lightData.lightColor = color;
    }
    void SetAmbientColor(Vec4 ambientColor)
    {
        lightData.ambientLightColor = ambientColor;
    }

    void SetModel(Model* model)
    {
        this->model = model;
    }

private:

    Data lightData;
    Model* model;
};
} // namespace Siege

#endif