//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "PhysicalDevice.h"

#include "Config.h"
#include "render/renderer/platform/vulkan/Context.h"
#include "utils/Device.h"
#include "utils/TypeAdaptor.h"

namespace Siege::Vulkan
{
PhysicalDevice::PhysicalDevice(Surface surface, const Instance& vulkanInstance)
{
    auto deviceExtensions = Vulkan::Config::deviceExtensions;

    device = Vulkan::Device::Physical::FindSuitableDevice(vulkanInstance.GetInstance(),
                                                          surface,
                                                          deviceExtensions.Data(),
                                                          deviceExtensions.Size());

    CC_ASSERT(device != VK_NULL_HANDLE, "Failed to find a suitable GPU!")

    auto vkProperties = Vulkan::Device::Physical::GetDeviceProperties(device);

    properties = {vkProperties.deviceName,
                  vkProperties.limits.minUniformBufferOffsetAlignment,
                  vkProperties.limits.minStorageBufferOffsetAlignment,
                  FindDepthFormat()};

    CC_LOG_INFO("Physical device found: {} with minumum buffer alignment of {}",
                properties.deviceName,
                properties.minUniformBufferOffsetAlignment)
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

Utils::DepthFormat PhysicalDevice::FindDepthFormat()
{
    const size_t formatCount = 5;
    VkFormat depthFormats[] = {VK_FORMAT_D32_SFLOAT_S8_UINT,
                               VK_FORMAT_D32_SFLOAT,
                               VK_FORMAT_D24_UNORM_S8_UINT,
                               VK_FORMAT_D16_UNORM_S8_UINT,
                               VK_FORMAT_D16_UNORM};

    return Utils::ToDepthFormat(
        Device::Physical::FindSupportedFormat(device,
                                              depthFormats,
                                              formatCount,
                                              VK_IMAGE_TILING_OPTIMAL,
                                              VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT));
}

uint32_t PhysicalDevice::GetGraphicsFamilyQueueIndex()
{
    return Device::Physical::GetMatchingQueueFamilyIndex(
        device,
        Vulkan::Context::GetSurface(),
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
        Vulkan::Context::GetSurface(),
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
