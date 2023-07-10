//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "VBuffer.h"

#include <utils/Logging.h>

#include <cstdlib>
#include <cstring>

#include "Context.h"
#include "utils/Buffer.h"

namespace Siege::Vulkan
{

VBuffer::VBuffer(unsigned long bufferSize) : size {bufferSize}
{
    CC_ASSERT(bufferSize > 0,
              "Cannot allocate vertex buffer of size 0. If you wanted to create an "
              "empty vertex buffer then please use the default constructor")

    rawBuffer = malloc(bufferSize);

    auto device = Context::GetVkLogicalDevice();

    auto physicalDevice = Context::GetPhysicalDevice()->GetDevice();

    Utils::CreateBuffer(device,
                        physicalDevice,
                        size,
                        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                        buffer,
                        memory);

    Utils::CopyData(device, memory, size, rawBuffer, 0);
}

VBuffer::VBuffer(void* data, unsigned long bufferSize) : VBuffer(bufferSize)
{
    memcpy(rawBuffer, data, bufferSize);
}

void VBuffer::Copy(void* data, unsigned long dSize, unsigned long offset)
{
    auto device = Context::GetVkLogicalDevice();

    if (size < dSize)
    {
    }

    void* dst = reinterpret_cast<char*>(rawBuffer) + offset;
    memcpy(dst, data, dSize);
    Utils::CopyData(device, memory, dSize, dst, offset);
}

VBuffer::VBuffer(VBuffer&& other)
{
    Swap(other);
}

VBuffer::VBuffer(const VBuffer& other) : VBuffer(other.rawBuffer, other.size) {}

VBuffer::~VBuffer()
{
    Free();
}

void VBuffer::Bind(const CommandBuffer& commandBuffer,
                   unsigned long long* offset,
                   unsigned int binding)
{
    vkCmdBindVertexBuffers(commandBuffer.Get(), binding, 1, &buffer, offset);
}

void VBuffer::Free()
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

VBuffer& VBuffer::operator=(const VBuffer& other)
{
    size = other.size;
    rawBuffer = malloc(size);
    memcpy(rawBuffer, other.rawBuffer, size);
    return *this;
}

VBuffer& VBuffer::operator=(VBuffer&& other)
{
    Swap(other);
    return *this;
}

void VBuffer::Allocate(void* data, unsigned long long newSize) {}

void VBuffer::Swap(VBuffer& other)
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
} // namespace Siege::Vulkan