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
#include <array>

namespace Siege::Vulkan
{
class Context
{
public:
    Context() = default;
    ~Context() = default;

    static void Init(Instance::GetSurfaceExtensionsCallback surfaceExtensionsCallback,
                     Instance::GetWindowSurfaceCallBack windowSurfaceCallback);

    static inline Instance& GetInstance() { return instance; }
    static inline VkInstance GetVkInstance() { return instance.GetInstance(); }

    static void Destroy();

private:
    static Instance instance;
};

}

#endif // SIEGE_ENGINE_CONTEXT_H
