#include "Buffer.h"

namespace SnekVk::Buffer
{
    // TODO: Wrap this in a static class 'BufferAllocator' so that we 
    // don't need to keep a static instance.

    void CreateBuffer(
        VkDeviceSize size, 
        VkBufferUsageFlags usage, 
        VkMemoryPropertyFlags properties, 
        VkBuffer &buffer,
		VkDeviceMemory &bufferMemory)
    {
        VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        auto deviceInstance = VulkanDevice::GetDeviceInstance();
        VkDevice device = deviceInstance->Device();

		SNEK_ASSERT(vkCreateBuffer(device, &bufferInfo, nullptr, OUT &buffer) == VK_SUCCESS,
			"failed to create vertex buffer!");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = deviceInstance->FindMemoryType(memRequirements.memoryTypeBits, properties);

		SNEK_ASSERT (vkAllocateMemory(device, &allocInfo, nullptr, OUT &bufferMemory) == VK_SUCCESS,
			"failed to allocate vertex buffer memory!");

  		vkBindBufferMemory(device, buffer, bufferMemory, 0);
    };

    void CopyData(Buffer& dstBuffer, VkDeviceSize size, const void* bufferData, VkDeviceSize offset)
    {
        auto device = VulkanDevice::GetDeviceInstance()->Device();

        void* data;
        vkMapMemory(device, dstBuffer.bufferMemory, offset, size, 0, &data);
        memcpy(data, bufferData, size);
        vkUnmapMemory(device, dstBuffer.bufferMemory);
    }

    void CopyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize size)
    {
        VulkanDevice::GetDeviceInstance()->CopyBuffer(srcBuffer, dstBuffer, size);
    }

    void DestroyBuffer(Buffer& buffer)
    {
        VkDevice device = VulkanDevice::GetDeviceInstance()->Device();
        if (buffer.buffer != VK_NULL_HANDLE) vkDestroyBuffer(device, buffer.buffer, nullptr);
        if (buffer.bufferMemory != VK_NULL_HANDLE) vkFreeMemory(device, buffer.bufferMemory, nullptr);
    }
}