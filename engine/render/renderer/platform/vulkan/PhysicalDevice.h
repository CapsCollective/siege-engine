//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_PHYSICALDEVICE_H
#define SIEGE_ENGINE_PHYSICALDEVICE_H

#include "Instance.h"

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
        uint64_t minStorageBufferOffsetAlignment {0};
        Utils::DepthFormat depthFormat {Utils::DEPTH_NONE};
    };

    PhysicalDevice() = default;
    ~PhysicalDevice() = default;

    PhysicalDevice(Surface surface, const Instance& vulkanInstance);
    PhysicalDevice(PhysicalDevice&& other);

    PhysicalDevice& operator=(PhysicalDevice&& other);

    VkPhysicalDevice GetDevice() const
    {
        return device;
    }

    inline constexpr size_t GetMinUniformDeviceAlignment() const
    {
        return properties.minUniformBufferOffsetAlignment;
    }

    inline constexpr size_t GetMinStorageDeviceAlignment() const
    {
        return properties.minStorageBufferOffsetAlignment;
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
