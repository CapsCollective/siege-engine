//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Mesh.h"

#include <utils/Logging.h>

#include "Swapchain.h"
#include "utils/TypeAdaptor.h"

namespace Siege::Vulkan
{
Mesh::Mesh(uint32_t vertexSize, uint32_t vertexCount, uint32_t indexCount)
{
    CC_ASSERT(vertexCount < MAX_VERTICES, "The provided model has too many vertices!")

    auto frames = Swapchain::MAX_FRAMES_IN_FLIGHT;

    perFrameIndexBuffers = MHArray<IndexBufferUpdate>(frames, frames);

    perFrameVertexBuffers = MHArray<VertexBuffer>(frames, frames);

    if (vertexCount == 0) return;

    for (size_t i = 0; i < frames; i++)
    {
        auto& indexBufferUpdate = perFrameIndexBuffers[i];

        auto& vertexBufferUpdate = perFrameVertexBuffers[i];

        Buffer::DestroyBuffer(indexBufferUpdate.updateBuffer);
        AllocateBuffer(indexBufferUpdate.updateBuffer,
                       sizeof(uint32_t) * indexCount,
                       Utils::BufferType::INDEX_BUFFER);

        vertexBufferUpdate = VertexBuffer(vertexSize, vertexCount);

        indexBufferUpdate.size = indexCount;
    }
}

Mesh::Mesh(const VertexData& vertices)
{
    CC_ASSERT(vertices.count < MAX_VERTICES, "The provided model has too many vertices!")

    auto frames = Swapchain::MAX_FRAMES_IN_FLIGHT;

    perFrameVertexBuffers = MHArray<VertexBuffer>(frames, frames);

    if (vertices.count == 0) return;

    for (size_t i = 0; i < frames; i++) SetVertexBuffers(vertices, i);
}

Mesh::Mesh(const VertexData& vertices, const IndexData& indices) : Mesh(vertices)
{
    CC_ASSERT(indices.count < MAX_INDICES, "The provided model has too many indices!")

    auto frames = Swapchain::MAX_FRAMES_IN_FLIGHT;

    perFrameIndexBuffers = MHArray<IndexBufferUpdate>(frames, frames);

    if (indices.count == 0) return;

    for (size_t i = 0; i < frames; i++) SetIndexBuffers(indices, i);
}

void Mesh::Swap(Mesh& other)
{
    auto tmpPerFrameVertexBuffers2 = std::move(perFrameVertexBuffers);
    auto tmpPerFrameIndexBuffers = std::move(perFrameIndexBuffers);

    perFrameVertexBuffers = std::move(other.perFrameVertexBuffers);
    perFrameIndexBuffers = std::move(other.perFrameIndexBuffers);

    other.perFrameVertexBuffers = std::move(tmpPerFrameVertexBuffers2);
    other.perFrameIndexBuffers = std::move(tmpPerFrameIndexBuffers);
}

Mesh::~Mesh()
{
    Free();
}

void Mesh::Free()
{
    for (auto it = perFrameIndexBuffers.CreateIterator(); it; ++it)
    {
        Buffer::DestroyBuffer((*it).updateBuffer);
    }

    for (auto it = perFrameVertexBuffers.CreateIterator(); it; ++it)
    {
        it->Free();
    }

    perFrameIndexBuffers.Clear();
    perFrameVertexBuffers.Clear();
}

void Mesh::AllocateBuffer(Buffer::Buffer& buffer, uint32_t size, Utils::BufferType bufferType)
{
    Buffer::CreateBuffer(size,
                         Utils::ToVkBufferUsageFlagBits(bufferType),
                         // specifies that data is accessible on the CPU.
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         OUT buffer.buffer,
                         OUT buffer.bufferMemory);
}

void Mesh::SetVertexBuffers(const VertexData& vertices, uint32_t currentFrame)
{
    auto& vertexBufferUpdate = perFrameVertexBuffers[currentFrame];

    vertexBufferUpdate.Update(vertices.vertexSize, vertices.vertices, vertices.count);
}

void Mesh::SetIndexBuffers(const IndexData& indices, uint32_t currentFrame)
{
    auto& indexBufferUpdate = perFrameIndexBuffers[currentFrame];

    if (indices.count > indexBufferUpdate.size)
    {
        Buffer::DestroyBuffer(indexBufferUpdate.updateBuffer);
        AllocateBuffer(indexBufferUpdate.updateBuffer,
                       sizeof(uint32_t) * indices.count,
                       Utils::BufferType::INDEX_BUFFER);

        indexBufferUpdate.size = indices.count;
    }

    Buffer::CopyData(indexBufferUpdate.updateBuffer,
                     indices.count * sizeof(uint32_t),
                     indices.indices);

    indexBufferUpdate.count = indices.count;
}

void Mesh::Bind(CommandBuffer& commandBuffer, uint32_t frameIndex)
{
    perFrameVertexBuffers[frameIndex].Bind(commandBuffer);
}

void Mesh::BindIndexed(CommandBuffer& commandBuffer, uint32_t frameIndex)
{
    Bind(commandBuffer, frameIndex);
    auto& buffer = perFrameIndexBuffers[frameIndex].updateBuffer;
    vkCmdBindIndexBuffer(commandBuffer.Get(), buffer.buffer, 0, VK_INDEX_TYPE_UINT32);
}

void Mesh::Update(const VertexData& vertices, uint32_t currentFrameIndex)
{
    SetVertexBuffers(vertices, currentFrameIndex);
}

void Mesh::UpdateIndexed(const VertexData& vertices,
                         const IndexData& indices,
                         uint32_t currentFrameIndex)
{
    Update(vertices, currentFrameIndex);
    SetIndexBuffers(indices, currentFrameIndex);
}

} // namespace Siege::Vulkan