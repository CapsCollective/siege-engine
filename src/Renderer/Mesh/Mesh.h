#pragma once

#include "../Core.h"
#include "../Buffer/Buffer.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <array>

namespace SnekVk 
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    bool operator==(const Vertex& left, const Vertex& right);

    std::array<VkVertexInputBindingDescription, 1> GetVertexBindingDescriptions();
    std::array<VkVertexInputAttributeDescription, 4> GetVertexAttributeDescriptions();

    class Mesh
    {
        public:

        Mesh();
        Mesh(Vertex* vertices, u32 vertexCount, u32* indices, u32 indexCount);
        ~Mesh();

        Mesh(const Mesh&) = delete;
        void operator=(const Mesh& other) = delete;

        void LoadVertices(Vertex* vertices, u32 vertexCount, u32* indices, u32 indexCount);
        void Bind(VkCommandBuffer commandBuffer);

        bool HasIndexBuffer() { return hasIndexBuffer; }

        u32 GetVertexCount() { return vertexCount; }
        u32 GetIndexCount() { return indexCount; }

        private:

        void CreateVertexBuffers(const Vertex* vertices);
        void CreateIndexBuffer(const u32* indices);

        Buffer::Buffer vertexBuffer;
        u32 vertexCount = 0;

        bool hasIndexBuffer = false;
        Buffer::Buffer indexBuffer;
        u32 indexCount = 0;
    };
}