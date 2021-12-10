#pragma once

#include "../Core.h"

namespace SnekVk {
    
    class PointLight 
    {
        public: 

        struct Data {
            glm::vec4 lightColor = glm::vec4(1.f, 1.f, 1.f, 0.2f);
            alignas(16)glm::vec4 ambientLightColor = glm::vec4(1.f, 1.f, 1.f, .02f);
            alignas(16) glm::vec3 position = glm::vec3(0.f);
        };

        PointLight();
        PointLight(glm::vec3 position, glm::vec4 color, glm::vec4 ambientColor);
        ~PointLight();

        Data& GetLightData() { return lightData; }
        void SetPosition(glm::vec3 position) { lightData.position = position; }
        void SetColor(glm::vec4 color) { lightData.lightColor = color; }
        void SetAmbientColor(glm::vec4 ambientColor) { lightData.ambientLightColor = ambientColor; }

        private: 
        
        Data lightData;
    };
}
