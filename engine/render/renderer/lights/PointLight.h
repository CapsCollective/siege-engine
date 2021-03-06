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
        glm::vec4 lightColor = glm::vec4(1.f, 1.f, 1.f, 0.2f);
        alignas(16) glm::vec4 ambientLightColor = glm::vec4(1.f, 1.f, 1.f, .02f);
        alignas(16) glm::vec3 position = glm::vec3(0.f);
    };

    PointLight();
    PointLight(glm::vec3 position, glm::vec4 color, glm::vec4 ambientColor);
    ~PointLight();

    Data& GetLightData()
    {
        return lightData;
    }
    Model* GetModel()
    {
        return model;
    }
    void SetPosition(glm::vec3 position)
    {
        lightData.position = position;
    }
    void SetColor(glm::vec4 color)
    {
        lightData.lightColor = color;
    }
    void SetAmbientColor(glm::vec4 ambientColor)
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