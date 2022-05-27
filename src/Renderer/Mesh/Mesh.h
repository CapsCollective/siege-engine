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

    struct Vertex2D
    {
        glm::vec2 position;
        glm::vec3 color;
    };

    struct BillboardVertex
    {
        glm::vec2 position;
    };

    bool operator==(const Vertex& left, const Vertex& right);
    bool operator==(const Vertex2D& left, const Vertex2D& right);

    class Mesh
    {
        public:

        static constexpr size_t MAX_VERTICES = 10000;
        static constexpr size_t MAX_INDICES = 100000;

        struct MeshData
        {
            u64 vertexSize {0};
            const void* vertices {nullptr};
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
        void UpdateVertices(const Mesh::MeshData& meshData);

        void UpdateVertexBuffer(const void* vertices);
        void UpdateIndexBuffer(u32* indices);

        void Bind(VkCommandBuffer commandBuffer);
        void DestroyMesh();

        bool HasIndexBuffer() { return hasIndexBuffer; }

        u32 GetVertexCount() { return vertexCount; }
        u32 GetIndexCount() { return indexCount; }

        private:

        // NOTE: This will likely need to be moved at some point. 
        // We'd like to try and get this to a point where we can 
        // bindlessly render models. This would require an allocation
        // of a single, large vertex and index buffer. 

        void CreateVertexBuffers(const void* vertices);
        void CreateIndexBuffer(const u32* indices);

        Buffer::Buffer globalStagingBuffer;
        Buffer::Buffer globalIndexStagingBuffer;

        Buffer::Buffer vertexBuffer;
        Buffer::Buffer indexBuffer;

        bool hasIndexBuffer = false;
        bool hasVertexBuffer = false;

        u32 indexCount = 0;
        u32 vertexCount = 0;

        u64 vertexSize = 0;

        bool isFreed = false;
    };
}