#include "PointLight.h"

namespace SnekVk
{

    PointLight::PointLight()
        : PointLight(
            glm::vec3(0.0f),
            glm::vec4(1.0f, 1.0f, 1.0f, 0.2f), 
            glm::vec4(1.f, 1.f, 1.f, .2f))
    {}

    PointLight::PointLight(glm::vec3 position, glm::vec4 color, glm::vec4 ambientColor) 
        : lightData{color, ambientColor, position} 
    {}

    PointLight::~PointLight() {}
}