#pragma once

#include "../../Core.h"
#include "../../Model/Model.h"

namespace SnekVk
{
    class LightRenderer
    {
        public:

        LightRenderer();
        ~LightRenderer();

        void Initialise(const char* globalDataAttributeName, const u64& globalDataSize);

        void Destroy();

        void DrawPointLight(const glm::vec3& position, const float& radius, const glm::vec4& colour, const glm::vec4& ambientColor);

        void Render(VkCommandBuffer& commandBuffer, const u64& globalDataSize, const void* globalData);

        void Flush();

        void RecreateMaterials();

        private: 

        struct PointLightVertex
        {
            alignas(16) glm::vec2 position;
        };

        struct LightUBO {
            glm::vec4 lightColor = glm::vec4(1.f, 1.f, 1.f, 0.2f);
            alignas(16) glm::vec4 ambientLightColor = glm::vec4(1.f, 1.f, 1.f, .02f);
            alignas(16) glm::vec3 position = glm::vec3(0.f);
            alignas(16) float radius = .05f;
        };

        Model lightModel;
        Material lightMaterial;

        Utils::StringId globalDataId;
        Utils::StringId lightDataId;

        Utils::StackArray<glm::vec2, Mesh::MAX_VERTICES> pointLightVertices;
        Utils::StackArray<u32, Mesh::MAX_INDICES> pointLightIndices;
    };
}