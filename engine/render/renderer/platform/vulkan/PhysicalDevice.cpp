//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "PhysicalDevice.h"

#include "Config.h"

namespace Siege::Vulkan
{

PhysicalDevice::PhysicalDevice(VulkanInstance& instance)
{
    auto extensions = Config::deviceExtensions;

    device = VulkanDevice::Physical::FindSuitableDevice(instance.Instance(),
                                                        instance.Surface(),
                                                        extensions.data(),
                                                        extensions.size());

    CC_ASSERT(device != VK_NULL_HANDLE, "Failed to find a suitable GPU!")

    properties = VulkanDevice::Physical::GetDeviceProperties(device);

    CC_LOG_INFO("Physical device found: {} with minumum buffer alignment of {}",
                properties.deviceName,
                properties.limits.minUniformBufferOffsetAlignment)
}

PhysicalDevice::PhysicalDevice(const PhysicalDevice& other)
{
    device = other.device;
    properties = other.properties;
}

PhysicalDevice::PhysicalDevice(PhysicalDevice&& other)
{
    device = other.device;
    properties = other.properties;
    other.device = VK_NULL_HANDLE;
}

void PhysicalDevice::operator=(PhysicalDevice&& other)
{
    device = other.device;
    properties = other.properties;
    other.device = VK_NULL_HANDLE;
}

void PhysicalDevice::operator=(const PhysicalDevice& other)
{
    device = other.device;
    properties = other.properties;
}

Array<VkSurfaceFormatKHR> PhysicalDevice::GetSurfaceFormats(VkSurfaceKHR surface)
{
    uint32_t formatCount;

    VulkanDevice::Physical::GetSurfaceFormatCount(device, surface, formatCount);
    Array<VkSurfaceFormatKHR> formats(formatCount);
    VulkanDevice::Physical::GetSurfaceFormats(device, surface, formatCount, formats.Data());

    return formats;
}

Array<VkPresentModeKHR> PhysicalDevice::GetPresentModes(VkSurfaceKHR surface)
{
    uint32_t presentModeCount = VulkanDevice::Physical::GetPresentModeCount(device, surface);
    auto presentModes = Array<VkPresentModeKHR>(presentModeCount);
    VulkanDevice::Physical::GetPresentModes(device, surface, presentModeCount, presentModes.Data());
    return presentModes;
}

VkSurfaceCapabilitiesKHR PhysicalDevice::GetCapabilities(VkSurfaceKHR surface)
{
    return VulkanDevice::Physical::GetSurfaceCapabilities(device, surface);
}

uint32_t PhysicalDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags newProperties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(device, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & newProperties) == newProperties)
        {
            return i;
        }
    }

    CC_ASSERT(false, "Failed to find suitable memory type!")
}

VkFormat PhysicalDevice::FindSupportedFormat(const VkFormat* candidates,
                                             size_t formatCount,
                                             VkImageTiling tiling,
                                             VkFormatFeatureFlags features)
{
    for (size_t i = 0; i < formatCount; i++)
    {
        VkFormat format = candidates[i];

        VkFormatProperties props = VulkanDevice::Physical::GetProperties(device, format);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
        {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
                 (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    CC_ASSERT(false, "Failed to find a supported format!")
}

} // namespace Siege::Vulkan