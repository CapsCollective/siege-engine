//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_INSTANCE_H
#define SIEGE_ENGINE_INSTANCE_H

#include <utils/collections/HeapArray.h>

#include "utils/Types.h"

namespace Siege::Vulkan
{
class Instance
{
public:

    Instance() = default;
    Instance(MHArray<const char*> surfaceExtensions);
    Instance(Instance&& other);
    ~Instance();

    Instance& operator=(Instance&& other);

    VkInstance GetInstance() const
    {
        return instance;
    }

private:

    void CreateInstance(const Siege::MHArray<const char*>& requiredSurfaceExtensions);
    void SetupDebugMessenger();

    bool ValidateLayersExist();
    void CheckInstanceExtensionsExist(const Siege::MHArray<const char*>& requiredSurfaceExtensions);

    void Move(Instance& other);
    void DestroyDependentObjects();

    VkInstance instance {nullptr};
    VkDebugUtilsMessengerEXT debugMessenger {nullptr};
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_INSTANCE_H
