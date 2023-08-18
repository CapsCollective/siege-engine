//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "IndexBuffer.h"

#include <utils/Logging.h>

#include "Context.h"
#include "utils/Buffer.h"

namespace Siege::Vulkan
{

IndexBuffer::IndexBuffer(unsigned long bufferSize) : size {bufferSize}
{
    CC_ASSERT(bufferSize > 0,
              "Cannot allocate vertex buffer of size 0. If you wanted to create an "
              "empty vertex buffer then please use the default constructor")

    auto device = Context::GetVkLogicalDevice();

    Allocate(device);
}

IndexBuffer::IndexBuffer(void* data, uint64_t dataSize) : IndexBuffer(dataSize)
{
    Copy(data, dataSize, 0);
}

IndexBuffer::IndexBuffer(const IndexBuffer& other) : IndexBuffer(other.rawBuffer, other.size) {}

IndexBuffer::IndexBuffer(IndexBuffer&& other)
{
    Swap(other);
}

IndexBuffer::~IndexBuffer()
{
    Free();
}

IndexBuffer& IndexBuffer::operator=(const IndexBuffer& other)
{
    size = other.size;

    auto device = Context::GetVkLogicalDevice();

    Allocate(device);

    Copy(other.rawBuffer, size);

    return *this;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other)
{
    Swap(other);
    return *this;
}

void IndexBuffer::Free()
{
    VkDevice device = Vulkan::Context::GetVkLogicalDevice();

    if (device == nullptr) return;

    free(rawBuffer);

    Utils::FreeBuffer(device, buffer, memory);

    buffer = nullptr;
    memory = nullptr;

    size = 0;
    rawBuffer = nullptr;
}

void IndexBuffer::Copy(const void* data, unsigned long dSize, unsigned long offset)
{
    auto device = Context::GetVkLogicalDevice();

    void* dst = reinterpret_cast<char*>(rawBuffer) + offset;
    memcpy(dst, data, dSize);
    Utils::CopyData(device, memory, dSize, dst, offset);
}

void IndexBuffer::Bind(const CommandBuffer& commandBuffer, uint64_t offset)
{
    vkCmdBindIndexBuffer(commandBuffer.Get(), buffer, offset, VK_INDEX_TYPE_UINT32);
}

void IndexBuffer::Swap(IndexBuffer& other)
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

void IndexBuffer::Allocate(VkDevice device)
{
    rawBuffer = malloc(size);

    auto physicalDevice = Context::GetPhysicalDevice()->GetDevice();

    Utils::CreateBuffer(device,
                        physicalDevice,
                        size,
                        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                        buffer,
                        memory);
}

} // namespace Siege::Vulkan