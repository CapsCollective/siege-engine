//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Context.h"

#include <utils/Logging.h>

#include "render/renderer/Renderer.h"

namespace Siege::Vulkan
{
Context::~Context()
{
    swapchain.~Swapchain();
    logicalDevice.~LogicalDevice();
    vkDestroySurfaceKHR(vulkanInstance.GetInstance(), surface, nullptr);
    vulkanInstance.~Instance();
}

void Context::Init(const Utils::Extent2D& extent,
                   Instance::GetSurfaceExtensionsCallback surfaceExtensionsCallback,
                   const GetWindowSurfaceCallBack& windowSurfaceCallback)
{
    CC_ASSERT(volkInitialize() == VK_SUCCESS, "Unable to initialise Volk!")

    vulkanInstance = Instance(surfaceExtensionsCallback);

    CC_ASSERT(windowSurfaceCallback(vulkanInstance.GetInstance(), &surface),
              "Unable to create window surface!")

    physicalDevice = PhysicalDevice(surface, vulkanInstance);

    logicalDevice = LogicalDevice(surface, physicalDevice);

    swapchain = Swapchain(extent);
}

Context& Context::Get()
{
    return Renderer::Context();
}

void Context::RecreateSwapchain(const Utils::Extent2D& extent)
{
    auto oldSwapchain = Get().GetSwapchain().GetRaw();
    Get().RecreateSwapchain(extent, oldSwapchain);
}

void Context::RecreateSwapchain(const Utils::Extent2D& extent, VkSwapchainKHR oldSwapchain)
{
    swapchain = Swapchain(extent, oldSwapchain);
}

} // namespace Siege::Vulkan