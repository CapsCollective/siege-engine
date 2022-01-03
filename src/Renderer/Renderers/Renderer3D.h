#pragma once

#include "../Core.h"
#include "../Model/Model.h"
#include "../Material/Material.h"
#include "../Utils/Math.h"
#include "../Lights/PointLight.h"
#include "../Camera/Camera.h"

namespace SnekVk
{
    class Renderer3D
    {
        public:

        static void Initialise();

        static void DrawModel(Model* model, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation);
        static void DrawModel(Model* model, const glm::vec3& position, const glm::vec3& scale);
        static void DrawModel(Model* model, const glm::vec3& position);

        static void DrawBillboard(Model* model, const glm::vec3& position, const glm::vec2& scale, const float& rotation);
        static void DrawLine(const glm::vec2& origin, const glm::vec2& destination);

        static void DrawLight(Model* model);

        static void RecreateMaterials();

        static void Render(VkCommandBuffer& commandBuffer, const void* globalData, const u64 globalDataSize);
        static void Flush();

        static void DestroyRenderer3D();

        private:

        static void RenderModels(VkCommandBuffer& commandBuffer, const void* globalData, const u64& globalDataSize);
        static void RenderLights(VkCommandBuffer& commandBuffer, const void* globalData, const u64& globalDataSize);
        
        static constexpr size_t MAX_OBJECT_TRANSFORMS = 1000;

        static Utils::StackArray<Model::Transform, MAX_OBJECT_TRANSFORMS> transforms;
        static Utils::StackArray<Model*, MAX_OBJECT_TRANSFORMS> models;

        static u64 transformSize;

        static Utils::StringId transformId;
        static Utils::StringId globalDataId;

        static Material* currentMaterial; 
        static Model* currentModel;

        // FIXME(Aryeh): This needs to change.
        static Model* lightModel;
        static Material lineMaterial;
    };
}