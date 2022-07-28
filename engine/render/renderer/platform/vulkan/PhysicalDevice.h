//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_PHYSICALDEVICE_H
#define SIEGE_ENGINE_PHYSICALDEVICE_H

#include "../../utils/Array.h"
#include "VulkanInstance.h"
#include "utils/Device.h"

namespace Siege::Vulkan
{

class PhysicalDevice
{
public:

    PhysicalDevice() = default;

    PhysicalDevice(VulkanInstance& instance);

    PhysicalDevice(const PhysicalDevice& other);
    PhysicalDevice(PhysicalDevice&& other);

    void operator=(PhysicalDevice&& other);
    void operator=(const PhysicalDevice& other);

    Array<VkSurfaceFormatKHR> GetSurfaceFormats(VkSurfaceKHR surface);
    Array<VkPresentModeKHR> GetPresentModes(VkSurfaceKHR surface);
    VkSurfaceCapabilitiesKHR GetCapabilities(VkSurfaceKHR surface);

    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags newProperties);
    VkFormat FindSupportedFormat(const VkFormat* candidates,
                                 size_t formatCount,
                                 VkImageTiling tiling,
                                 VkFormatFeatureFlags features);

    ~PhysicalDevice() = default;

    VkPhysicalDevice Device()
    {
        return device;
    }
    VkPhysicalDeviceProperties Properties()
    {
        return properties;
    }

private:

    VkPhysicalDevice device {VK_NULL_HANDLE};
    VkPhysicalDeviceProperties properties {};
};

} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_PHYSICALDEVICE_H
