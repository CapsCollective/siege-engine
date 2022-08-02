//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_LOGICALDEVICE_H
#define SIEGE_ENGINE_LOGICALDEVICE_H

#include "PhysicalDevice.h"

namespace Siege::Vulkan
{
class LogicalDevice
{
public:

    LogicalDevice() = default;
    LogicalDevice(const Instance& instance, PhysicalDevice& physicalDevice);
    LogicalDevice(LogicalDevice&& other);
    ~LogicalDevice();

    LogicalDevice& operator=(LogicalDevice&& other);

    inline VkQueue GetGraphicsQueue()
    {
        return graphicsQueue;
    }
    inline VkQueue GetPresentQueue()
    {
        return presentQueue;
    }

    inline VkCommandPool GetCommandPool()
    {
        return commandPool;
    }

    inline VkDevice GetDevice()
    {
        return device;
    }

    VkCommandBuffer GetCommandBuffer();

    VkCommandBuffer BeginSingleTimeCommand(VkCommandBuffer commandBuffer);
    void EndSingleTimeCommand(VkCommandBuffer commandBuffer);

    void FlushCommandBuffer(VkCommandBuffer commandBuffer);

    void SubmitToGraphicsQueue(VkCommandBuffer commandBuffer);
    void SubmitToQueue(VkCommandBuffer commandBuffer, VkQueue queue);
    void FreeCommandBuffer(VkCommandBuffer commandBuffer);

    void AllocateImageMemory(VkImage& image,
                             VkDeviceMemory& imageMemory,
                             VkMemoryPropertyFlags targetProperties);

    void AllocateMemory(VkMemoryRequirements& memRequirements,
                        VkDeviceMemory& memory,
                        VkMemoryPropertyFlags targetProperties);

    void CreateImage(const VkImageCreateInfo& createInfo, VkImage& image);
    void BindImageMemory(VkImage& image,
                         VkDeviceMemory& imageMemory,
                         const VkDeviceSize& offset = 0);

    void CreateImageWithInfo(
        const VkImageCreateInfo& createInfo,
        VkImage& image,
        VkDeviceMemory& imageMemory,
        VkMemoryPropertyFlags targetProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void CopyBufferToImage(VkBuffer buffer,
                           VkImage image,
                           uint32_t width,
                           uint32_t height,
                           uint32_t layerCount);

private:

    void Move(LogicalDevice& other);

    VkDevice device;

    PhysicalDevice* physicalDevice;

    VkQueue graphicsQueue {VK_NULL_HANDLE};
    VkQueue presentQueue {VK_NULL_HANDLE};

    VkCommandPool commandPool;
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_LOGICALDEVICE_H
