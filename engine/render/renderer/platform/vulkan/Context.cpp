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

namespace Siege::Vulkan
{
Context::~Context()
{
    logicalDevice.~LogicalDevice();
    vulkanInstance.~Instance();
}

void Context::Init(Instance::GetSurfaceExtensionsCallback surfaceExtensionsCallback,
                 Instance::GetWindowSurfaceCallBack windowSurfaceCallback)
{
    CC_ASSERT(volkInitialize() == VK_SUCCESS, "Unable to initialise Volk!")

    vulkanInstance = Instance(surfaceExtensionsCallback, windowSurfaceCallback);

    physicalDevice = PhysicalDevice(vulkanInstance);

    logicalDevice = LogicalDevice(vulkanInstance, physicalDevice);
}

PhysicalDevice& Context::GetPhysicalDevice()
{
    return physicalDevice;
}

LogicalDevice& Context::GetLogicalDevice()
{
    return logicalDevice;
}
}