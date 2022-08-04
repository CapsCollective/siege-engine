//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_PHYSICALDEVICE_H
#define SIEGE_ENGINE_PHYSICALDEVICE_H

#include "Instance.h"
#include "utils/Types.h"

namespace Siege::Vulkan
{
class PhysicalDevice
{
public:

    struct DeviceProperties
    {
        // TODO: Add new properties as new needs are discovered.
        const char* deviceName;
        uint64_t minUniformBufferOffsetAlignment {0};
        Utils::DepthFormat depthFormat {Utils::DEPTH_NONE};
    };

    PhysicalDevice() = default;
    ~PhysicalDevice() = default;

    PhysicalDevice(const Instance& vulkanInstance);
    PhysicalDevice(PhysicalDevice&& other);

    PhysicalDevice& operator=(PhysicalDevice&& other);

    DeviceProperties GetProperties() const
    {
        return properties;
    }

    VkPhysicalDevice GetDevice() const
    {
        return device;
    }

    inline constexpr size_t GetMinDeviceAlignment() const
    {
        return properties.minUniformBufferOffsetAlignment;
    }

    inline constexpr Utils::DepthFormat GetDepthFormat() const
    {
        return properties.depthFormat;
    }

    Utils::DepthFormat FindDepthFormat();

    uint32_t GetGraphicsFamilyQueueIndex();
    uint32_t GetPresentFamilyQueueIndex();

private:

    void Move(PhysicalDevice& other);

    VkPhysicalDevice device {nullptr};
    DeviceProperties properties {};
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_PHYSICALDEVICE_H
