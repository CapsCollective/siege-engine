//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_CONTEXT_H
#define SIEGE_ENGINE_CONTEXT_H

#include "Instance.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"

namespace Siege::Vulkan
{
class Context
{
public:

    Context() = default;
    ~Context();

    void Init(Instance::GetSurfaceExtensionsCallback surfaceExtensionsCallback,
              Instance::GetWindowSurfaceCallBack windowSurfaceCallback);

    static inline Instance& GetInstance()
    {
        return vulkanInstance;
    }
    static inline VkInstance GetVkInstance()
    {
        return vulkanInstance.GetInstance();
    }

    static Context& Get();

    static LogicalDevice* GetCurrentDevice()
    {
        return &Get().logicalDevice;
    }
    static PhysicalDevice* GetPhysicalDevice()
    {
        return &Get().physicalDevice;
    }

    static VkDevice GetVkLogicalDevice()
    {
        return Get().logicalDevice.GetDevice();
    }
    static VkPhysicalDevice GetVkPhysicalDevice()
    {
        return Get().physicalDevice.GetDevice();
    }

private:

    static inline Instance vulkanInstance;

    PhysicalDevice physicalDevice;
    LogicalDevice logicalDevice;
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_CONTEXT_H
