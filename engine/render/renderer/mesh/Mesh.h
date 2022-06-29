//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#pragma once

#include "../Core.h"
#include "../buffer/Buffer.h"
#include "../pipeline/PipelineConfig.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <array>
#include <glm/glm.hpp>

namespace Siege
{
struct Vertex
{
    Vec3 position;
    Vec3 color;
    Vec3 normal;
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
    explicit Mesh(const MeshData& meshData);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    void operator=(const Mesh& other) = delete;

    void LoadVertices(const Mesh::MeshData& meshData);
    void UpdateVertices(const Mesh::MeshData& meshData);

    void UpdateVertexBuffer(const void* vertices);
    void UpdateIndexBuffer(u32* indices);

    void Bind(VkCommandBuffer commandBuffer);
    void DestroyMesh();

    bool HasIndexBuffer() const
    {
        return hasIndexBuffer;
    }

    u32 GetVertexCount() const
    {
        return vertexCount;
    }
    u32 GetIndexCount() const
    {
        return indexCount;
    }

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

    u64 indexSize {sizeof(u32)};
    u64 vertexSize = 0;

    bool isFreed = false;
};
} // namespace Siege