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
    logicalDevice.~LogicalDevice();
    vkDestroySurfaceKHR(vulkanInstance.GetInstance(), surface, nullptr);
    vulkanInstance.~Instance();
}

void Context::Init(Instance::GetSurfaceExtensionsCallback surfaceExtensionsCallback,
                   GetWindowSurfaceCallBack windowSurfaceCallback)
{
    CC_ASSERT(volkInitialize() == VK_SUCCESS, "Unable to initialise Volk!")

    vulkanInstance = Instance(surfaceExtensionsCallback);

    CC_ASSERT(windowSurfaceCallback(vulkanInstance.GetInstance(), &surface),
              "Unable to create window surface!")

    physicalDevice = PhysicalDevice(surface, vulkanInstance);

    logicalDevice = LogicalDevice(surface, physicalDevice);
}

Context& Context::Get()
{
    return Renderer::Context();
}
} // namespace Siege::Vulkan