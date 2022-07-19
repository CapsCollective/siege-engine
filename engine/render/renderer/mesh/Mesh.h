//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MESH_H
#define SIEGE_ENGINE_MESH_H

#include "../Core.h"
#include "../buffer/Buffer.h"
#include "../pipeline/PipelineConfig.h"

namespace Siege
{
struct Vertex
{
    Vec3 position;
    Vec3 color;
    Vec3 normal;
    Vec2 uv;
};

struct Vertex2D
{
    Vec2 position;
    Vec3 color;
};

struct BillboardVertex
{
    Vec2 position;
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
        uint64_t vertexSize {0};
        const void* vertices {nullptr};
        uint32_t vertexCount {0};
        uint32_t* indices {nullptr};
        uint32_t indexCount {0};
    };

    Mesh();
    Mesh(const MeshData& meshData);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    void operator=(const Mesh& other) = delete;

    void LoadVertices(const Mesh::MeshData& meshData);
    void UpdateVertices(const Mesh::MeshData& meshData);

    void UpdateVertexBuffer(const void* vertices);
    void UpdateIndexBuffer(uint32_t* indices);

    void Bind(VkCommandBuffer commandBuffer);
    void DestroyMesh();

    bool HasIndexBuffer()
    {
        return hasIndexBuffer;
    }

    uint32_t GetVertexCount()
    {
        return vertexCount;
    }
    uint32_t GetIndexCount()
    {
        return indexCount;
    }

private:

    // NOTE: This will likely need to be moved at some point.
    // We'd like to try and get this to a point where we can
    // bindlessly render models. This would require an allocation
    // of a single, large vertex and index buffer.

    void CreateVertexBuffers(const void* vertices);
    void CreateIndexBuffer(const uint32_t* indices);

    Buffer::Buffer globalStagingBuffer;
    Buffer::Buffer globalIndexStagingBuffer;

    Buffer::Buffer vertexBuffer;
    Buffer::Buffer indexBuffer;

    bool hasIndexBuffer = false;
    bool hasVertexBuffer = false;

    uint32_t indexCount = 0;
    uint32_t vertexCount = 0;

    uint64_t indexSize {sizeof(uint32_t)};
    uint64_t vertexSize = 0;

    bool isFreed = false;
};
} // namespace Siege

#endif