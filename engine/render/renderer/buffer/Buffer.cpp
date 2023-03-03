//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Buffer.h"

#include <utils/Logging.h>

#include "render/renderer/platform/vulkan/Context.h"
#include "render/renderer/platform/vulkan/utils/Device.h"

namespace Siege::Buffer
{
// TODO: Wrap this in a static class 'BufferAllocator' so that we
// don't need to keep a static instance.

void CreateBuffer(VkDeviceSize size,
                  VkBufferUsageFlags usage,
                  VkMemoryPropertyFlags properties,
                  VkBuffer& buffer,
                  VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferInfo {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkDevice device = Vulkan::Context::GetVkLogicalDevice();

    CC_ASSERT(vkCreateBuffer(device, &bufferInfo, nullptr, OUT & buffer) == VK_SUCCESS,
              "failed to create vertex buffer!")

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex =
        Vulkan::Device::Physical::FindMemoryType(Vulkan::Context::GetPhysicalDevice()->GetDevice(),
                                                 memRequirements.memoryTypeBits,
                                                 properties);

    CC_ASSERT(vkAllocateMemory(device, &allocInfo, nullptr, OUT & bufferMemory) == VK_SUCCESS,
              "failed to allocate vertex buffer memory!");

    vkBindBufferMemory(device, buffer, bufferMemory, 0);
};

void CopyData(Buffer& dstBuffer, VkDeviceSize size, const void* bufferData, VkDeviceSize offset)
{
    auto device = Vulkan::Context::GetVkLogicalDevice();

    void* data;
    vkMapMemory(device, dstBuffer.bufferMemory, offset, size, 0, &data);
    memcpy(data, bufferData, size);
    vkUnmapMemory(device, dstBuffer.bufferMemory);
}

void AppendData(Buffer& dstBuffer, VkDeviceSize size, const void* bufferData)
{
    auto device = Vulkan::Context::GetVkLogicalDevice();

    void* data;
    vkMapMemory(device, dstBuffer.bufferMemory, dstBuffer.size, size, 0, &data);
    memcpy(data, bufferData, size);
    vkUnmapMemory(device, dstBuffer.bufferMemory);

    dstBuffer.size = dstBuffer.size + size;
}

void CopyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize size)
{
    Vulkan::Context::GetCurrentDevice()->CopyBuffer(srcBuffer, dstBuffer, size);
}

void DestroyBuffer(Buffer& buffer)
{
    VkDevice device = Vulkan::Context::GetVkLogicalDevice();
    if (device == nullptr) return;

    if (buffer.buffer != VK_NULL_HANDLE) vkDestroyBuffer(device, buffer.buffer, nullptr);
    if (buffer.bufferMemory != VK_NULL_HANDLE) vkFreeMemory(device, buffer.bufferMemory, nullptr);

    buffer.buffer = VK_NULL_HANDLE;
    buffer.bufferMemory = VK_NULL_HANDLE;
}

size_t PadUniformBufferSize(size_t originalSize)
{
    // Calculate required alignment based on minimum device offset alignment
    size_t minUboAlignment = Vulkan::Context::GetPhysicalDevice()->GetMinUniformDeviceAlignment();
    size_t alignedSize = originalSize;
    if (minUboAlignment > 0)
    {
        alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);
    }
    return alignedSize;
}

size_t PadStorageBufferSize(size_t originalSize)
{
    // Calculate required alignment based on minimum device offset alignment
    size_t minUboAlignment = Vulkan::Context::GetPhysicalDevice()->GetMinStorageDeviceAlignment();
    size_t alignedSize = originalSize;
    if (minUboAlignment > 0)
    {
        alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);
    }
    return alignedSize;
}
} // namespace Siege::Buffer
