//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_PHYSICALDEVICE_H
#define SIEGE_ENGINE_PHYSICALDEVICE_H

#include "Config.h"
#include "Instance.h"

namespace Siege::Vulkan
{
class PhysicalDevice
{
public:
    PhysicalDevice() = default;
    ~PhysicalDevice() = default;

    PhysicalDevice(const Instance& vulkanInstance);
    PhysicalDevice(PhysicalDevice&& other);

    PhysicalDevice& operator=(PhysicalDevice&& other);

    VkPhysicalDeviceProperties GetProperties() const { return properties; }
    VkPhysicalDevice GetDevice() const { return device; }
private:
    void Move(PhysicalDevice& other);

    VkPhysicalDevice device {VK_NULL_HANDLE};
    VkPhysicalDeviceProperties properties {};
};
} // namespace Siege

#endif // SIEGE_ENGINE_PHYSICALDEVICE_H
