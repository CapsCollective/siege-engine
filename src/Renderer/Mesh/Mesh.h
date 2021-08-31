#pragma once

#include "../Core.h"
#include "../Buffer/Buffer.h"
#include "../Pipeline/PipelineConfig.h"

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

    VertexDescription::Data GetDescriptionData();

    Utils::Array<VkVertexInputBindingDescription> GetVertexBindingDescriptions();
    Utils::Array<VkVertexInputAttributeDescription> GetVertexAttributeDescriptions();

    class Mesh
    {
        public:

        struct MeshData
        {
            Vertex* vertices {nullptr};
            u32 vertexCount {0}; 
            u32* indices {nullptr}; 
            u32 indexCount {0};
        };

        Mesh();
        Mesh(const MeshData& meshData);
        ~Mesh();

        Mesh(const Mesh&) = delete;
        void operator=(const Mesh& other) = delete;

        void LoadVertices(const Mesh::MeshData& meshData);
        void Bind(VkCommandBuffer commandBuffer);

        bool HasIndexBuffer() { return hasIndexBuffer; }

        u32 GetVertexCount() { return vertexCount; }
        u32 GetIndexCount() { return indexCount; }

        private:

        // NOTE: This will likely need to be moved at some point. 
        // We'd like to try and get this to a point where we can 
        // bindlessly render models. This would require an allocation
        // of a single, large vertex and index buffer. 

        void CreateVertexBuffers(const Vertex* vertices);
        void CreateIndexBuffer(const u32* indices);

        Buffer::Buffer vertexBuffer;
        u32 vertexCount = 0;

        bool hasIndexBuffer = false;
        Buffer::Buffer indexBuffer;
        u32 indexCount = 0;
    };
}