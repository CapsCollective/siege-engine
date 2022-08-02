//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "PhysicalDevice.h"
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

void PhysicalDevice::Move(PhysicalDevice& other)
{
    device = other.device;
    properties = other.properties;

    other.device = VK_NULL_HANDLE;
    other.properties = {};
}
} // namespace Siege