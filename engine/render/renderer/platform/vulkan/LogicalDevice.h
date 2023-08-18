//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
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
    LogicalDevice(const Surface& surface, PhysicalDevice& physDevice);
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

    void WaitIdle();

    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint64_t size);
    void CopyBufferToImage(VkBuffer buffer,
                           VkImage image,
                           uint32_t width,
                           uint32_t height,
                           uint32_t layerCount);

private:

    void Move(LogicalDevice& other);

    VkDevice device {nullptr};

    PhysicalDevice* physicalDevice;

    VkQueue graphicsQueue {nullptr};
    VkQueue presentQueue {nullptr};

    VkCommandPool commandPool;
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_LOGICALDEVICE_H
