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

Instance Context::instance = {};

void Context::Init(Instance::GetSurfaceExtensionsCallback surfaceExtensionsCallback,
                 Instance::GetWindowSurfaceCallBack windowSurfaceCallback)
{
    CC_ASSERT(volkInitialize() == VK_SUCCESS, "Unable to initialise Volk!")

    instance = Instance(surfaceExtensionsCallback, windowSurfaceCallback);
}

void Context::Destroy()
{
    instance.~Instance();
}
}