#pragma once

#include "../../Core.h"
#include "../../Model/Model.h"
#include "../../Material/Material.h"

namespace SnekVk
{
    class DebugRenderer3D
    {
        public: 

        DebugRenderer3D();
        ~DebugRenderer3D();

        void Initialise(const char* globalDataAttributeName, const u64& globalDataSize);
        void Destroy();

        // Wire primitives
        void DrawLine(const glm::vec3& origin, const glm::vec3& destination, const glm::vec3& colour);
        void DrawCube();

        void Render(VkCommandBuffer& commandBuffer, const u64& globalDataSize, const void* globalData);

        void Flush();

        private: 

        struct LineVertex
        {
            glm::vec3 position;
            glm::vec3 colour;
        };

        void RenderLines(VkCommandBuffer& commandBuffer, const u64& globalDataSize, const void* globalData);
        void RenderRects();

        glm::vec3 lineColor;

        Material lineMaterial;
        Model lineModel;

        Material rectMaterial;
        Model rectModel;

        Utils::StringId globalDataId;

        Utils::StackArray<LineVertex, Mesh::MAX_VERTICES> lines;
        Utils::StackArray<glm::vec3, Mesh::MAX_VERTICES> rects;
    };
}