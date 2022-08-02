//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "PhysicalDevice.h"

#include "render/renderer/platform/vulkan/Context.h"
#include "utils/Device.h"

namespace Siege::Vulkan
{
PhysicalDevice::PhysicalDevice(const Instance& vulkanInstance)
{
    auto deviceExtensions = Vulkan::Config::deviceExtensions;

    device = Vulkan::Device::Physical::FindSuitableDevice(vulkanInstance.GetInstance(),
                                                          vulkanInstance.GetSurface(),
                                                          deviceExtensions.Data(),
                                                          deviceExtensions.Size());

    CC_ASSERT(device != VK_NULL_HANDLE, "Failed to find a suitable GPU!")

    properties = Vulkan::Device::Physical::GetDeviceProperties(device);

    CC_LOG_INFO("Physical device found: {} with minumum buffer alignment of {}",
                properties.deviceName,
                properties.limits.minUniformBufferOffsetAlignment)
}

PhysicalDevice::PhysicalDevice(PhysicalDevice&& other)
{
    Move(other);
}

PhysicalDevice& PhysicalDevice::operator=(PhysicalDevice&& other)
{
    Move(other);
    return *this;
}

VkFormat PhysicalDevice::FindSupportedFormat(const VkFormat* candidates,
                                             size_t formatCount,
                                             VkImageTiling tiling,
                                             VkFormatFeatureFlags features)
{
    for (size_t i = 0; i < formatCount; i++)
    {
        VkFormat format = candidates[i];

        VkFormatProperties props = Vulkan::Device::Physical::GetProperties(device, format);

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

VkSurfaceCapabilitiesKHR PhysicalDevice::GetCapabilities()
{
    auto surface = Vulkan::Context::GetInstance().GetSurface();
    return Vulkan::Device::Physical::GetSurfaceCapabilities(device, surface);
}

Utils::MHArray<VkSurfaceFormatKHR> PhysicalDevice::GetSurfaceFormats()
{
    auto surface = Vulkan::Context::GetInstance().GetSurface();
    uint32_t formatCount = Vulkan::Device::Physical::GetSurfaceFormatCount(device, surface);

    auto formats = Utils::MHArray<VkSurfaceFormatKHR>(static_cast<size_t>(formatCount));
    Vulkan::Device::Physical::GetSurfaceFormats(device, surface, formatCount, formats.Data());
    return formats;
}

Utils::MHArray<VkPresentModeKHR> PhysicalDevice::GetPresentModes()
{
    auto surface = Vulkan::Context::GetInstance().GetSurface();
    uint32_t presentModeCount = Vulkan::Device::Physical::GetPresentModeCount(device, surface);

    auto presentModes = Utils::MHArray<VkPresentModeKHR>(static_cast<uint32_t>(presentModeCount));
    Vulkan::Device::Physical::GetPresentModes(device,
                                              surface,
                                              presentModeCount,
                                              presentModes.Data());
    return presentModes;
}

uint32_t PhysicalDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(device, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    CC_ASSERT(false, "Failed to find suitable memory type!")
}

uint32_t PhysicalDevice::GetGraphicsFamilyQueueIndex()
{
    return Device::Physical::GetMatchingQueueFamilyIndex(
        device,
        Vulkan::Context::GetInstance().GetSurface(),
        [](VkPhysicalDevice physicalDevice,
           VkQueueFamilyProperties props,
           size_t index,
           VkSurfaceKHR surface) -> int32_t {
            return (props.queueCount > 0 && props.queueFlags & VK_QUEUE_GRAPHICS_BIT) * index;
        });
}

uint32_t PhysicalDevice::GetPresentFamilyQueueIndex()
{
    return Device::Physical::GetMatchingQueueFamilyIndex(
        device,
        Vulkan::Context::GetInstance().GetSurface(),
        [](VkPhysicalDevice physicalDevice,
           VkQueueFamilyProperties props,
           size_t index,
           VkSurfaceKHR surface) -> int32_t {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice,
                                                 index,
                                                 surface,
                                                 OUT & presentSupport);
            return (props.queueCount > 0 && presentSupport) * index;
        });
}

void PhysicalDevice::Move(PhysicalDevice& other)
{
    device = other.device;
    properties = other.properties;

    other.device = VK_NULL_HANDLE;
    other.properties = {};
}
} // namespace Siege::Vulkan