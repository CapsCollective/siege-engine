//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "VertexBuffer.h"

#include <utils/Logging.h>

#include <cstdlib>
#include <cstring>

#include "Context.h"
#include "utils/Buffer.h"

namespace Siege::Vulkan
{

VertexBuffer::VertexBuffer(unsigned long bufferSize) : size {bufferSize}
{
    CC_ASSERT(bufferSize > 0,
              "Cannot allocate vertex buffer of size 0. If you wanted to create an "
              "empty vertex buffer then please use the default constructor")

    auto device = Context::GetVkLogicalDevice();

    Allocate(device);
}

VertexBuffer::VertexBuffer(void* data, unsigned long bufferSize) : VertexBuffer(bufferSize)
{
    Copy(data, bufferSize);
}

void VertexBuffer::Copy(const void* data, unsigned long dSize, unsigned long offset)
{
    auto device = Context::GetVkLogicalDevice();

    void* dst = reinterpret_cast<char*>(rawBuffer) + offset;
    memcpy(dst, data, dSize);
    Utils::CopyData(device, memory, dSize, dst, offset);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other)
{
    Swap(other);
}

VertexBuffer::VertexBuffer(const VertexBuffer& other) : VertexBuffer(other.rawBuffer, other.size) {}

VertexBuffer::~VertexBuffer()
{
    Free();
}

void VertexBuffer::Bind(const CommandBuffer& commandBuffer,
                        const uint64_t* offset,
                        unsigned int binding)
{
    vkCmdBindVertexBuffers(commandBuffer.Get(), binding, 1, &buffer, offset);
}

void VertexBuffer::Free()
{
    VkDevice device = Vulkan::Context::GetVkLogicalDevice();

    if (device == nullptr) return;

    free(rawBuffer);
    Utils::FreeBuffer(device, buffer, memory);
    size = 0;
    rawBuffer = nullptr;
    buffer = nullptr;
    memory = nullptr;
}

VertexBuffer& VertexBuffer::operator=(const VertexBuffer& other)
{
    size = other.size;

    auto device = Context::GetVkLogicalDevice();

    Allocate(device);

    Copy(other.rawBuffer, size);

    return *this;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other)
{
    Swap(other);
    return *this;
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
} // namespace Siege::Vulkan