//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#pragma once

#include "../Core.h"
#include "../model/Model.h"

namespace Siege
{

class PointLight
{
public:

    struct Data
    {
        Siege::Vec4 lightColor = {1.f, 1.f, 1.f, 0.2f};
        alignas(16) Siege::Vec4 ambientLightColor = {1.f, 1.f, 1.f, .02f};
        alignas(16) Siege::Vec3 position = Siege::Vec3::Zero;
    };

    PointLight();
    PointLight(Siege::Vec3 position, Siege::Vec4 color, Siege::Vec4 ambientColor);
    ~PointLight();

    Data& GetLightData()
    {
        return lightData;
    }
    Model* GetModel()
    {
        return model;
    }
    void SetPosition(Siege::Vec3 position)
    {
        lightData.position = position;
    }
    void SetColor(Siege::Vec4 color)
    {
        lightData.lightColor = color;
    }
    void SetAmbientColor(Siege::Vec4 ambientColor)
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
