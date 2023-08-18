//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#define VOLK_IMPLEMENTATION
#define GLFW_INCLUDE_VULKAN

#include "Context.h"

#include <utils/Logging.h>

#include "render/renderer/Renderer.h"
#include "utils/TypeAdaptor.h"

// clang-format off
#include <GLFW/glfw3.h> // Must be final include
// clang-format on

namespace Siege::Vulkan
{
Context::~Context()
{
    swapchain.~Swapchain();
    logicalDevice.~LogicalDevice();
    vkDestroySurfaceKHR(vulkanInstance.GetInstance(), surface, nullptr);
    vulkanInstance.~Instance();
}

void Context::Init(Window& window)
{
    CC_ASSERT(volkInitialize() == VK_SUCCESS, "Unable to initialise Volk!")

    vulkanInstance = Instance(window.GetRequiredExtensions());

    glfwCreateWindowSurface(vulkanInstance.GetInstance(),
                            reinterpret_cast<GLFWwindow*>(window.GetRawWindow()),
                            nullptr,
                            &surface);

    CC_ASSERT(surface != VK_NULL_HANDLE, "Unable to create surface!")

    physicalDevice = PhysicalDevice(surface, vulkanInstance);

    logicalDevice = LogicalDevice(surface, physicalDevice);

    auto extents = window.GetExtents();
    swapchain = Swapchain({extents.width, extents.height});
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