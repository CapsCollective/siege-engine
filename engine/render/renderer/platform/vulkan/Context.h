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
#include "Swapchain.h"

namespace Siege::Vulkan
{
class Context
{
public:

    typedef std::function<bool(VkInstance, VkSurfaceKHR*)> getSurfaceCallback;
    typedef getSurfaceCallback GetWindowSurfaceCallBack;

    Context() = default;
    ~Context();

    void Init(const Utils::Extent2D& extent,
              Instance::GetSurfaceExtensionsCallback surfaceExtensionsCallback,
              const GetWindowSurfaceCallBack& windowSurfaceCallback);

    static Context& Get();

    static LogicalDevice* GetCurrentDevice()
    {
        return &Get().logicalDevice;
    }
    static PhysicalDevice* GetPhysicalDevice()
    {
        return &Get().physicalDevice;
    }

    static Surface& GetSurface()
    {
        return Get().surface;
    }

    static VkDevice GetVkLogicalDevice()
    {
        return Get().logicalDevice.GetDevice();
    }

    static Swapchain& GetSwapchain()
    {
        return Get().swapchain;
    }

    static void RecreateSwapchain(const Utils::Extent2D& extent);

private:

    void RecreateSwapchain(const Utils::Extent2D& extent, VkSwapchainKHR oldSwapchain);

    static inline Instance vulkanInstance;

    PhysicalDevice physicalDevice;
    Surface surface {VK_NULL_HANDLE};
    LogicalDevice logicalDevice;
    Swapchain swapchain;
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_CONTEXT_H
