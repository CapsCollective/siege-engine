//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_PHYSICALDEVICE_H
#define SIEGE_ENGINE_PHYSICALDEVICE_H

#include "utils/Device.h"

namespace Siege::Vulkan
{
class PhysicalDevice
{
public:
    PhysicalDevice() = default;

    PhysicalDevice(VkInstance instance,
                   VkSurfaceKHR surface,
                   const char* const* extensions,
                   uint32_t extensionCount);

    PhysicalDevice(const PhysicalDevice& other);
    PhysicalDevice(PhysicalDevice&& other);

    void operator=(PhysicalDevice&& other);
    void operator=(const PhysicalDevice& other);

    ~PhysicalDevice() = default;

    VkPhysicalDevice GetDevice() { return device; }
    VkPhysicalDeviceProperties GetProperties() { return properties; }

private:
    VkPhysicalDevice device {VK_NULL_HANDLE};
    VkPhysicalDeviceProperties properties {};
};

} // namespace Siege

#endif // SIEGE_ENGINE_PHYSICALDEVICE_H
