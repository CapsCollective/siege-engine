//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Buffer.h"

#include <utils/Logging.h>

#include "../Context.h"
#include "Device.h"

namespace Siege::Vulkan::Utils
{
VkBufferCopy CopyRegion(VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset)
{
    return {srcOffset, dstOffset, size};
}

void CopyBuffer(VkCommandBuffer commandBuffer,
                VkBuffer srcBuffer,
                VkBuffer dstBuffer,
                const VkBufferCopy& copyRegions,
                uint32_t regionCount)
{
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, &copyRegions);
}

void CreateBuffer(VkDevice logicalDevice,
                  VkPhysicalDevice physicalDevice,
                  VkDeviceSize size,
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

    CC_ASSERT(vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, OUT & buffer) == VK_SUCCESS,
              "failed to create vertex buffer!")

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(logicalDevice, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Device::Physical::FindMemoryType(physicalDevice,
                                                                 memRequirements.memoryTypeBits,
                                                                 properties);

    CC_ASSERT(
        vkAllocateMemory(logicalDevice, &allocInfo, nullptr, OUT & bufferMemory) == VK_SUCCESS,
        "failed to allocate vertex buffer memory!");

    vkBindBufferMemory(logicalDevice, buffer, bufferMemory, 0);
};

void CopyData(VkDevice device,
              VkDeviceMemory& memory,
              VkDeviceSize size,
              const void* bufferData,
              VkDeviceSize offset)
{
    void* data;
    vkMapMemory(device, memory, offset, size, 0, &data);
    memcpy(data, bufferData, size);
    vkUnmapMemory(device, memory);
}

void FreeBuffer(VkDevice device, VkBuffer buffer, VkDeviceMemory memory)
{
    vkDestroyBuffer(device, buffer, nullptr);
    vkFreeMemory(device, memory, nullptr);
}

} // namespace Siege::Vulkan::Utils