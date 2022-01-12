#pragma once

#include "../Core.h"
#include "../Model/Model.h"
#include "../Material/Material.h"
#include "../Utils/Math.h"
#include "../Lights/PointLight.h"
#include "../Camera/Camera.h"
#include "Renderer3D/DebugRenderer3D.h"

namespace SnekVk
{
    class Renderer3D
    {
        public:

        struct GlobalData 
        {
            CameraData cameraData;
            PointLight::Data lightData;
        };

        static void Initialise();

        static void DrawModel(Model* model, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation);
        static void DrawModel(Model* model, const glm::vec3& position, const glm::vec3& scale);
        static void DrawModel(Model* model, const glm::vec3& position);

        static void DrawBillboard(Model* model, const glm::vec3& position, const glm::vec2& scale, const float& rotation);

        // Debug rendering
        // TODO(Aryeh): Move this to it's own renderer module?
        static void DrawLine(const glm::vec3& origin, const glm::vec3& destination, const glm::vec3& colour);
        static void DrawRect(const glm::vec3& position, const glm::vec2& scale, glm::vec3 color);

        static void DrawLight(Model* model);

        static void EnableGrid();

        static void RecreateMaterials();

        static void Render(VkCommandBuffer& commandBuffer, const GlobalData& globalData);
        static void Flush();

        static void DestroyRenderer3D();

        private:

        static void RenderModels(VkCommandBuffer& commandBuffer, const GlobalData& globalData);
        static void RenderLights(VkCommandBuffer& commandBuffer, const GlobalData& globalData);
        static void RenderLines(VkCommandBuffer& commandBuffer, const GlobalData& globalData);
        //static void RenderRects(VkCommandBuffer& commandBuffer, const GlobalData& globalData);
        static void RenderGrid(VkCommandBuffer& commandBuffer, const GlobalData& globalData);
        
        static constexpr size_t MAX_OBJECT_TRANSFORMS = 1000;

        static Utils::StackArray<Model::Transform, MAX_OBJECT_TRANSFORMS> transforms;
        static Utils::StackArray<Model*, MAX_OBJECT_TRANSFORMS> models;

        static u64 transformSize;

        static Utils::StringId transformId;
        static Utils::StringId globalDataId;

        static Material* currentMaterial; 
        static Model* currentModel;

        // FIXME(Aryeh): Everything below this needs to change.

        static DebugRenderer3D debugRenderer;

        // Lights need to exist in their own collection.
        static Model* lightModel;

        static Material gridMaterial;

        static bool gridEnabled;
    };
}