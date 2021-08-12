#pragma once

#include "../Core.h"
#include "../Device/VulkanDevice.h"

namespace SnekVk::Buffer
{
    struct Buffer
    {
        VkBuffer buffer {VK_NULL_HANDLE};
        VkDeviceMemory bufferMemory {VK_NULL_HANDLE};
    };

    void CreateBuffer(
        VkDeviceSize size, 
        VkBufferUsageFlags usage, 
        VkMemoryPropertyFlags properties, 
        VkBuffer &buffer,
		VkDeviceMemory &bufferMemory);

    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    template<typename T>
    void CopyData(Buffer& dstBuffer, VkDeviceSize size, const T* bufferData)
    {
        void* data;
        vkMapMemory(VulkanDevice::GetDeviceInstance()->Device(), dstBuffer.bufferMemory, 0, size, 0, &data);
        memcpy(data, bufferData, size);
        vkUnmapMemory(VulkanDevice::GetDeviceInstance()->Device(), dstBuffer.bufferMemory);
    }

    void CopyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize size);

    void DestroyBuffer(Buffer& buffer);
}