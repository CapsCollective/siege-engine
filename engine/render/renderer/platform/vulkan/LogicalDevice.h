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
    LogicalDevice(const Instance& instance, const PhysicalDevice& physicalDevice);
    LogicalDevice(LogicalDevice&& other);
    ~LogicalDevice();

    LogicalDevice& operator=(LogicalDevice&& other);

    inline VkQueue GetGraphicsQueue() { return graphicsQueue; }
    inline VkQueue GetPresentQueue() { return presentQueue; }

    inline VkCommandPool GetCommandPool() { return commandPool; }

    inline VkDevice GetDevice() { return device; }
private:
    void Move(LogicalDevice& other);

    VkDevice device;

    VkQueue graphicsQueue {VK_NULL_HANDLE};
    VkQueue presentQueue {VK_NULL_HANDLE};

    VkCommandPool commandPool;
};
} // namespace Siege

#endif // SIEGE_ENGINE_LOGICALDEVICE_H
