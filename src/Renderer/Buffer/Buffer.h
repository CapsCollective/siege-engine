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

    /**
     * Creates a memory buffer for transferring data to our GPU. Allocates resulting data to the 'buffer'
     * and 'bufferMemory' variables respectively. 
     * 
     * @param size - specifies the size of the buffer.
     * @param usage - specifies what the buffer will be used for (i.e: vertex definitions).
     * @param properties - specifies the the properties the buffer should have.
     * @param buffer - the buffer that the function should write data to.
     * @param bufferMemory - the buffer memory data the function should write data to.
     **/
    void CreateBuffer(
        VkDeviceSize size, 
        VkBufferUsageFlags usage, 
        VkMemoryPropertyFlags properties, 
        VkBuffer &buffer,
		VkDeviceMemory &bufferMemory);

    /**
     * Returns a bitmask value representing the memory type required to allocate GPU memory.
     * Different rendering operations require different memory types. These are typically
     * differentiated using a bitmask. This functions throws an exception if the required 
     * memory type is unavailable.
     * @param typeFilter - The bitmask for the required memory type
     * @param properties - The available device memory properties. Used to query for available memory. 
     **/
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    /**
     * Copies data into a buffer. Data can be any type as specified by 
     * the templated type. 
     * 
     * @param dstBuffer - the buffer which data should be copied to.
     * @param size - the size of the buffer
     * @param bufferData - the data being copied into the buffer
     **/
    template<typename T>
    void CopyData(Buffer& dstBuffer, VkDeviceSize size, const T* bufferData, VkDeviceSize offset = 0)
    {
        void* data;
        vkMapMemory(VulkanDevice::GetDeviceInstance()->Device(), dstBuffer.bufferMemory, offset, size, 0, &data);
        memcpy(data, bufferData, size);
        vkUnmapMemory(VulkanDevice::GetDeviceInstance()->Device(), dstBuffer.bufferMemory);
    }

    /**
     * Copies a buffer. This is generally used when copying the values within a buffer into
     * another (such as when setting up a staging buffer between the CPU and GPU).
     * 
     * @param srcBuffer - the buffer being copied.
     * @param dstBuffer - the buffer where the data is being copied into.
     * @param size - the expected size of the resulting buffer.
     **/
    void CopyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize size);

    /**
     * Destroys a buffer struct and releases memory back to the device. 
     * 
     * @param buffer - the buffer to be destroyed.
     **/
    void DestroyBuffer(Buffer& buffer);
}
