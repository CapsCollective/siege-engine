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
VertexBuffer::VertexBuffer(uint32_t vertexSize, void* vertices, uint32_t vertexCount) :
    size {vertexSize * vertexCount},
    count {vertexCount}
{
    if (vertexCount == 0) return;

    auto device = Context::GetVkLogicalDevice();

    Allocate(device);

    if (vertices == nullptr) return;

    Copy(device, size, vertices);
}

VertexBuffer::VertexBuffer(uint32_t vertexSize, uint32_t vertexCount) :
    size {vertexSize * vertexCount},
    count {vertexCount}
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
    count = 0;
}

void VertexBuffer::Update(uint32_t vertexSize, void* vertices, uint32_t vertexCount)
{
    auto device = Context::GetVkLogicalDevice();

    const uint64_t newSize = vertexSize * vertexCount;

    if (newSize > size)
    {
        Free();
        size = newSize;
        Allocate(device);
    }

    Copy(device, size, vertices);

    count = vertexCount;
}

void VertexBuffer::Bind(CommandBuffer& commandBuffer)
{
    VkBuffer buffers[] = {buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer.Get(), 0, 1, buffers, offsets);
}

void VertexBuffer::Swap(VertexBuffer& other)
{
    auto tmpRawBuffer = rawBuffer;
    auto tmpSize = size;
    auto tmpBuffer = buffer;
    auto tmpMemory = memory;
    auto tmpCount = count;

    rawBuffer = other.rawBuffer;
    size = other.size;
    buffer = other.buffer;
    memory = other.memory;
    count = other.count;

    other.rawBuffer = tmpRawBuffer;
    other.size = tmpSize;
    other.buffer = tmpBuffer;
    other.memory = tmpMemory;
    other.count = tmpCount;
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
                        OUT buffer,
                        OUT memory);
}

void VertexBuffer::Copy(VkDevice device, uint32_t dataSize, void* data)
{
    memcpy(rawBuffer, data, dataSize);
    Utils::CopyData(device, memory, dataSize, rawBuffer, 0);
}
} // namespace Siege::Vulkan