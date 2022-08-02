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
#include "PhysicalDevice.h"
#include "LogicalDevice.h"

namespace Siege::Vulkan
{
class Context
{
public:
    Context() = default;
    ~Context();

    void Init(Instance::GetSurfaceExtensionsCallback surfaceExtensionsCallback,
                     Instance::GetWindowSurfaceCallBack windowSurfaceCallback);

    static inline Instance& GetInstance() { return vulkanInstance; }
    static inline VkInstance GetVkInstance() { return vulkanInstance.GetInstance(); }
    PhysicalDevice& GetPhysicalDevice();
    LogicalDevice& GetLogicalDevice();

private:
    static inline Instance vulkanInstance;
    static inline Context* instance;

    PhysicalDevice physicalDevice;
    LogicalDevice logicalDevice;
};
}

#endif // SIEGE_ENGINE_CONTEXT_H
