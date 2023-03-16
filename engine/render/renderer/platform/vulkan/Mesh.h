//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_MESH_H
#define SIEGE_ENGINE_VULKAN_MESH_H

#include "render/renderer/buffer/Buffer.h"
#include "CommandBuffer.h"
#include "utils/Types.h"

#include <cstdint>
#include <utils/collections/HeapArray.h>

namespace Siege::Vulkan
{
class Mesh
{
public:
    struct VertexData
    {
        uint64_t vertexSize {0};
        const void* vertices {nullptr};
        uint32_t count {0};
    };

    struct IndexData
    {
        uint32_t* indices {nullptr};
        uint32_t count {0};
    };

    // These values are totally arbitrary right now, just need some form of upper bound
    static constexpr uint32_t MAX_VERTICES = 10000;
    static constexpr uint32_t MAX_INDICES = 100000;

    Mesh() = default;
    Mesh(const VertexData& vertices);
    Mesh(const VertexData& vertices, const IndexData& indices);
    inline Mesh(Mesh&& other) noexcept { Swap(other); }
    ~Mesh();

    void Free();

    inline Mesh& operator=(Mesh&& other)
    {
        Swap(other);
        return *this;
    }

    void Bind(CommandBuffer& commandBuffer, uint32_t frameIndex);
    void BindIndexed(CommandBuffer& commandBuffer, uint32_t frameIndex);
    void Update(const VertexData& vertices, uint32_t currentFrameIndex);
    void UpdateIndexed(const VertexData& vertices, const IndexData& indices, uint32_t currentFrameIndex);

    const uint32_t GetIndexCount(uint32_t frameIndex) const { return perFrameIndexBuffers[frameIndex].count; }
    const uint32_t GetVertexCount(uint32_t frameIndex) const { return perFrameVertexBuffers[frameIndex].count; }

private:
    struct VertexBufferUpdate
    {
        Buffer::Buffer updateBuffer {};
        uint32_t count {0};
    };

    struct IndexBufferUpdate
    {
        Buffer::Buffer updateBuffer;
        uint32_t count;
    };

    void Swap(Mesh& other);
    void AllocateBuffer(Buffer::Buffer& buffer, uint32_t size, Utils::BufferType bufferType);
    void SetVertexBuffers(const VertexData& vertices, uint32_t currentFrame);
    void SetIndexBuffers(const IndexData& indices, uint32_t currentFrame);

    MHArray<VertexBufferUpdate> perFrameVertexBuffers;
    MHArray<IndexBufferUpdate> perFrameIndexBuffers;
};

} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VULKAN_MESH_H
