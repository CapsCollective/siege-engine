//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "PhysicalDevice.h"

#include <set>

#include "../../platform/vulkan/utils/Device.h"

namespace Siege::PhysicalDevice
{
bool CheckExtensionSupport(VkPhysicalDevice device,
                           const char* const* deviceExtensions,
                           size_t deviceExtensionCount)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, OUT & extensionCount, nullptr);

    VkExtensionProperties availableExtensions[extensionCount];
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, OUT availableExtensions);

    // We use a set here so that we can store only unique values.
    std::set<String> requiredExtensions;

    for (size_t i = 0; i < deviceExtensionCount; i++)
    {
        requiredExtensions.insert(deviceExtensions[i]);
    }

    for (size_t i = 0; i < extensionCount; i++)
    {
        VkExtensionProperties extension = availableExtensions[i];
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

bool IsSuitable(VkPhysicalDevice device,
                VkSurfaceKHR& surface,
                const char* const* deviceExtensions,
                size_t deviceExtensionCount)
{
    bool indicesSupported = Vulkan::Device::Physical::HasRequiredQueues(device, surface);

    bool extensionsSupported = Vulkan::Device::Physical::SupportsExtensions(device,
                                                                            deviceExtensions,
                                                                            deviceExtensionCount);

    bool swapChainAdequate = Vulkan::Device::Physical::HasFormats(device, surface) &&
                             Vulkan::Device::Physical::HasPresentModes(device, surface);

    VkPhysicalDeviceFeatures supportedFeatures =
        Vulkan::Device::Physical::GetDeviceFeatures(device);

    // A device is only suitable if it ticks the following boxes:
    // 1) All extensions are supported.
    // 2) It has the supported formats and present modes.
    // 3) It has a graphics and present queues.
    // 4) It supports sampler anistropy.
    return indicesSupported && extensionsSupported && swapChainAdequate &&
           supportedFeatures.samplerAnisotropy;
}
} // namespace Siege::PhysicalDevice