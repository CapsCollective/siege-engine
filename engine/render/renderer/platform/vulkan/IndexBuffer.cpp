//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "IndexBuffer.h"

#include <cstdint>

#include "Context.h"
#include "utils/Buffer.h"

namespace Siege::Vulkan
{

IndexBuffer::IndexBuffer(uint32_t count, const uint32_t* indices) : count {count}
{
    if (count == 0) return;

    size = INDEX_BUFFER_ELEMENT_SIZE * count;

    auto device = Context::GetVkLogicalDevice();

    Allocate(device);

    if (indices == nullptr) return;

    Copy(device, size, indices);
}

IndexBuffer::IndexBuffer(uint32_t count) : count {count}
{
    if (count == 0) return;

    size = INDEX_BUFFER_ELEMENT_SIZE * count;

    auto device = Context::GetVkLogicalDevice();

    Allocate(device);
}

void IndexBuffer::Update(unsigned int* data, unsigned int indexCount)
{
    auto device = Context::GetVkLogicalDevice();

    const uint64_t newSize = INDEX_BUFFER_ELEMENT_SIZE * indexCount;

    if (newSize > size)
    {
        Free();
        size = newSize;
        Allocate(device);
    }

    Copy(device, size, data);

    count = indexCount;
}

void IndexBuffer::Bind(CommandBuffer& commandBuffer)
{
    vkCmdBindIndexBuffer(commandBuffer.Get(), buffer, 0, VK_INDEX_TYPE_UINT32);
}

void IndexBuffer::Free()
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

void IndexBuffer::Swap(IndexBuffer& other)
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

void IndexBuffer::Allocate(VkDevice device)
{
    rawBuffer = static_cast<uint32_t*>(malloc(size));

    auto physicalDevice = Context::GetPhysicalDevice()->GetDevice();

    Utils::CreateBuffer(device,
                        physicalDevice,
                        size,
                        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                        buffer,
                        memory);
}

void IndexBuffer::Copy(VkDevice device, unsigned int newSize, const unsigned int* data)
{
    memcpy(rawBuffer, data, newSize);
    Utils::CopyData(device, memory, newSize, rawBuffer, 0);
}
} // namespace Siege::Vulkan