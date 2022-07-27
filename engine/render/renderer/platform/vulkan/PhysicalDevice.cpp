//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "PhysicalDevice.h"

namespace Siege::Vulkan
{

PhysicalDevice::PhysicalDevice(VkInstance instance,
                               VkSurfaceKHR surface,
                               const char* const* extensions,
                               uint32_t extensionCount)
{
    device = Device::Physical::FindSuitableDevice(instance, surface, extensions, extensionCount);

    CC_ASSERT(device != VK_NULL_HANDLE, "Failed to find a suitable GPU!")

    properties = Vulkan::Device::Physical::GetDeviceProperties(device);

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

} // namespace Siege