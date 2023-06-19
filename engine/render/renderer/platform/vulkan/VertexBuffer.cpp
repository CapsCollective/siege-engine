//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "VertexBuffer.h"

#include "Context.h"
#include "utils/Buffer.h"

namespace Siege::Vulkan
{
VertexBuffer::VertexBuffer(uint32_t newSize) : size {newSize}
{
    if (newSize == 0) return;

    auto device = Context::GetVkLogicalDevice();

    Allocate(device);
}

VertexBuffer::VertexBuffer(uint32_t vertexSize, void* vertices, uint32_t vertexCount) :
    size {vertexSize * vertexCount}
{
    if (vertexCount == 0) return;

    auto device = Context::GetVkLogicalDevice();

    Allocate(device);

    if (vertices == nullptr) return;

    Copy(device, size, vertices);
}

VertexBuffer::VertexBuffer(uint32_t vertexSize, uint32_t vertexCount) :
    size {vertexSize * vertexCount}
{
    if (vertexCount == 0) return;

    auto device = Context::GetVkLogicalDevice();

    Allocate(device);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other)
{
    Swap(other);
}

VertexBuffer::~VertexBuffer()
{
    Free();
}

void VertexBuffer::Free()
{
    VkDevice device = Vulkan::Context::GetVkLogicalDevice();

    if (device == nullptr) return;

    free(rawBuffer);

    Utils::FreeBuffer(device, buffer, memory);

    buffer = VK_NULL_HANDLE;
    memory = VK_NULL_HANDLE;

    rawBuffer = nullptr;

    size = 0;
}

void VertexBuffer::Update(uint32_t vertexSize,
                          void* vertices,
                          uint32_t vertexCount,
                          uint32_t offset)
{
    auto device = Context::GetVkLogicalDevice();

    const uint64_t newSize = vertexSize * vertexCount;

    if (newSize > size)
    {
        Free();
        size = newSize;
        Allocate(device);
    }

    Copy(device, newSize, vertices, offset);
}

void VertexBuffer::Bind(CommandBuffer& commandBuffer,
                        uint64_t* offsets,
                        uint32_t offsetCount,
                        uint32_t binding,
                        uint64_t firstOffset)
{
    VkBuffer buffers[offsetCount];

    for (size_t i = 0; i < offsetCount; i++) buffers[i] = buffer;

    uint64_t* vertOffsets = offsets == nullptr ? &firstOffset : offsets;

    vkCmdBindVertexBuffers(commandBuffer.Get(), binding, offsetCount, buffers, vertOffsets);
}

void VertexBuffer::Swap(VertexBuffer& other)
{
    auto tmpRawBuffer = rawBuffer;
    auto tmpSize = size;
    auto tmpBuffer = buffer;
    auto tmpMemory = memory;

    rawBuffer = other.rawBuffer;
    size = other.size;
    buffer = other.buffer;
    memory = other.memory;

    other.rawBuffer = tmpRawBuffer;
    other.size = tmpSize;
    other.buffer = tmpBuffer;
    other.memory = tmpMemory;
}

void VertexBuffer::Allocate(VkDevice device)
{
    rawBuffer = malloc(size);

    auto physicalDevice = Context::GetPhysicalDevice()->GetDevice();

    Utils::CreateBuffer(device,
                        physicalDevice,
                        size,
                        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                        buffer,
                        memory);
}

void VertexBuffer::Copy(VkDevice device, uint32_t dataSize, void* data, uint32_t offset)
{
    void* dst = (char*) rawBuffer + offset;
    memcpy(dst, data, dataSize);
    Utils::CopyData(device, memory, dataSize, dst, offset);
}
} // namespace Siege::Vulkan