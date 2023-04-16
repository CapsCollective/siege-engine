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

namespace Siege::Vulkan
{
Mesh::Mesh(uint32_t vertexSize, uint32_t vertCount, uint32_t indexCount)
{
    CC_ASSERT(vertexCount < MAX_VERTICES, "The provided model has too many vertices!")

    auto frames = Swapchain::MAX_FRAMES_IN_FLIGHT;

    perFrameIndexBuffers = MHArray<IndexBuffer>(frames, frames);
    perFrameVertexBuffers = MHArray<VertexBuffer>(frames, frames);

    for (size_t i = 0; i < frames; i++)
    {
        perFrameIndexBuffers[i] = IndexBuffer(indexCount);
        perFrameVertexBuffers[i] = VertexBuffer(vertexSize, vertexCount);
    }

    vertexCount = vertCount;
}

Mesh::Mesh(const VertexData& vertices)
{
    CC_ASSERT(vertices.count < MAX_VERTICES, "The provided model has too many vertices!")

    auto frames = Swapchain::MAX_FRAMES_IN_FLIGHT;

    perFrameVertexBuffers = MHArray<VertexBuffer>(frames, frames);

    if (vertices.count == 0) return;

    for (size_t i = 0; i < frames; i++) SetVertexBuffers(vertices, i);

    vertexCount = vertices.count;
}

Mesh::Mesh(const VertexData& vertices, const IndexData& indices) : Mesh(vertices)
{
    CC_ASSERT(indices.count < MAX_INDICES, "The provided model has too many indices!")

    auto frames = Swapchain::MAX_FRAMES_IN_FLIGHT;

    perFrameIndexBuffers = MHArray<IndexBuffer>(frames, frames);

    if (indices.count == 0) return;

    for (size_t i = 0; i < frames; i++) SetIndexBuffers(indices, i);

    vertexCount = vertices.count;
}

void Mesh::Swap(Mesh& other)
{
    auto tmpPerFrameVertexBuffers2 = std::move(perFrameVertexBuffers);
    auto tmpPerFrameIndexBuffers = std::move(perFrameIndexBuffers);
    auto tmpVertexCount = vertexCount;

    perFrameVertexBuffers = std::move(other.perFrameVertexBuffers);
    perFrameIndexBuffers = std::move(other.perFrameIndexBuffers);
    vertexCount = other.vertexCount;

    other.perFrameVertexBuffers = std::move(tmpPerFrameVertexBuffers2);
    other.perFrameIndexBuffers = std::move(tmpPerFrameIndexBuffers);
    other.vertexCount = tmpVertexCount;
}

Mesh::~Mesh()
{
    Free();
}

void Mesh::Free()
{
    for (auto it = perFrameIndexBuffers.CreateIterator(); it; ++it)
    {
        it->Free();
    }

    for (auto it = perFrameVertexBuffers.CreateIterator(); it; ++it)
    {
        it->Free();
    }

    perFrameIndexBuffers.Clear();
    perFrameVertexBuffers.Clear();
    vertexCount = 0;
}

void Mesh::SetVertexBuffers(const VertexData& vertices, uint32_t currentFrame)
{
    auto& vertexBufferUpdate = perFrameVertexBuffers[currentFrame];

    vertexBufferUpdate.Update(vertices.vertexSize, vertices.vertices, vertices.count);
}

void Mesh::SetIndexBuffers(const IndexData& indices, uint32_t currentFrame)
{
    auto& indexBufferUpdate = perFrameIndexBuffers[currentFrame];

    indexBufferUpdate.Update(indices.indices, indices.count);
}

void Mesh::Bind(CommandBuffer& commandBuffer, uint32_t frameIndex)
{
    perFrameVertexBuffers[frameIndex].Bind(commandBuffer);
}

void Mesh::BindIndexed(CommandBuffer& commandBuffer, uint32_t frameIndex)
{
    Bind(commandBuffer, frameIndex);
    perFrameIndexBuffers[frameIndex].Bind(commandBuffer);
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