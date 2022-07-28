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

typedef VkDevice VkDevice;

namespace Siege::Vulkan
{
class LogicalDevice
{
public:

    LogicalDevice() = default;

    LogicalDevice(const PhysicalDevice& physDevice, VkSurfaceKHR surface);

    LogicalDevice(LogicalDevice&& other);

    LogicalDevice& operator=(LogicalDevice&& other);

    ~LogicalDevice();

    VkDevice Device()
    {
        return device;
    }
    VkQueue GraphicsQueue()
    {
        return graphicsQueue;
    }
    VkQueue PresentQueue()
    {
        return presentQueue;
    }
    VkCommandPool CommandPool()
    {
        return commandPool;
    }

    VkCommandBuffer BeginSingleTimeCommands();
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

    VkMemoryRequirements GetImageMemoryRequirements(VkImage image);
    void AllocateMemory(VkMemoryAllocateInfo allocInfo,
                        VkDeviceMemory& memory,
                        VkAllocationCallbacks* callbacks = nullptr);

    void CreateImage(const VkImageCreateInfo& imageInfo,
                     VkImage& image,
                     VkAllocationCallbacks* callbacks = nullptr);

    void BindImageMemory(VkImage image, VkDeviceMemory memory, VkDeviceSize offset = 0);

    void CreateImageWithInfo(const VkImageCreateInfo& imageInfo,
                             VkMemoryPropertyFlags targetProperties,
                             VkImage& image,
                             VkDeviceMemory& imageMemory);

private:

    void CreateCommandPool(VkPhysicalDevice vkPhysicalDevice);

    VkCommandPool commandPool {VK_NULL_HANDLE};

    VkDevice device {VK_NULL_HANDLE};

    VkQueue graphicsQueue {VK_NULL_HANDLE};
    VkQueue presentQueue {VK_NULL_HANDLE};

    PhysicalDevice physicalDevice;
};

} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_LOGICALDEVICE_H
