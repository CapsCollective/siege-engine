#pragma once

#include "../Core.h"
#include "../Model/Model.h"
#include "../Material/Material.h"
#include "../Utils/Math.h"
#include "../Camera/Camera.h"

namespace SnekVk
{
    class Renderer2D
    {
        public:

        struct GlobalData
        {
            CameraData cameraData;
        };

        static void Initialise();

        static void DrawModel(Model* model, const glm::vec2& position, const glm::vec2& scale, const float& rotation, const float& zIndex);
        static void DrawModel(Model* model, const glm::vec2& position, const glm::vec2& scale, const float& zIndex);
        static void DrawModel(Model* model, const glm::vec2& position);

        static void RecreateMaterials();

        static void Render(VkCommandBuffer& commandBuffer, const GlobalData& globalData);
        static void Flush();

        private:

        static constexpr size_t MAX_OBJECT_TRANSFORMS = 1000;

        static Utils::StackArray<Model::Transform2D, MAX_OBJECT_TRANSFORMS> transforms;
        static Utils::StackArray<Model*, MAX_OBJECT_TRANSFORMS> models;

        static u64 transformSize;

        static Utils::StringId transformId;
        static Utils::StringId globalDataId;

        static Material* currentMaterial; 
        static Model* currentModel;
    };
}