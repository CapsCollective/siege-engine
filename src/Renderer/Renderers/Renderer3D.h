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

        struct GlobalData 
        {
            CameraData cameraData;
            PointLight::Data lightData;
        };

        struct LineData
        {
            glm::vec3 color{1.f, 1.f, 1.f};
        };

        static void Initialise();

        static void DrawModel(Model* model, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation);
        static void DrawModel(Model* model, const glm::vec3& position, const glm::vec3& scale);
        static void DrawModel(Model* model, const glm::vec3& position);
        static void DrawGrid(size_t rowCount, size_t columnCount, glm::vec2 scale);

        static void DrawBillboard(Model* model, const glm::vec3& position, const glm::vec2& scale, const float& rotation);

        // Debug rendering
        // TODO(Aryeh): Move this to it's own renderer module?
        static void DrawLine(const glm::vec3& origin, const glm::vec3& destination, glm::vec3 color);
        static void DrawRect(const glm::vec3& position, const glm::vec2& scale, glm::vec3 color);

        static void DrawLight(Model* model);

        static void RecreateMaterials();

        static void Render(VkCommandBuffer& commandBuffer, const GlobalData& globalData);
        static void Flush();

        static void DestroyRenderer3D();

        private:

        static struct GridData{
            glm::vec3 rotation{0.f, 0.f, 0.f};
            glm::vec2 gridScale{1.f, 1.f};

            glm::vec3 vertices[Mesh::MAX_VERTICES];
            u32 indices[Mesh::MAX_INDICES];

            u32 indexCount = 0;
            u32 vertexCount = 0;

            u32 latestIndex = 0; 

            u32 nextTopRightIndex = 0;
            u32 nextBottomRightIndex = 0;
            u32 nextBottomLeftIndex = 0;
            u32 nextTopLeftIndex = 0;

            u32 rows = 0; 
            u32 columns = 0;

            bool enabled = false;
        } gridData;

        static void RenderModels(VkCommandBuffer& commandBuffer, const GlobalData& globalData);
        static void RenderLights(VkCommandBuffer& commandBuffer, const GlobalData& globalData);
        static void RenderLines(VkCommandBuffer& commandBuffer, const GlobalData& globalData);
        static void RenderRects(VkCommandBuffer& commandBuffer, const GlobalData& globalData);
        static void RenderGrid(VkCommandBuffer& commandBuffer, const GlobalData& globalData);
        
        static void DrawFirstGridQuad(const glm::vec3& offset); 
        static void AddWireQuad(std::initializer_list<u32> indices, const u32 indexModifier);
        static void DrawFirstRow();
        static void DrawRow(size_t row);
        
        static constexpr size_t MAX_OBJECT_TRANSFORMS = 1000;

        static Utils::StackArray<Model::Transform, MAX_OBJECT_TRANSFORMS> transforms;
        static Utils::StackArray<Model*, MAX_OBJECT_TRANSFORMS> models;

        static u64 transformSize;

        static Utils::StringId transformId;
        static Utils::StringId globalDataId;

        static Material* currentMaterial; 
        static Model* currentModel;

        // FIXME(Aryeh): Everything below this needs to change.

        // Lights need to exist in their own collection.
        static Model* lightModel;

        // Lines need to exist in their own collection as well.
        static Material lineMaterial;
        static Model lineModel;

        // This should exist possibly in an array of other debug lines.
        static LineData lineData;

        static Material rectMaterial;
        static Model rectModel;

        static Utils::StackArray<glm::vec3, Mesh::MAX_VERTICES> lines;
        static Utils::StackArray<glm::vec3, Mesh::MAX_VERTICES> rects;
    };
}