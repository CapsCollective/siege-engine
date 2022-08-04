//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_INSTANCE_H
#define SIEGE_ENGINE_INSTANCE_H

#include "utils/Types.h"
#include <utils/collections/HeapArray.h>

namespace Siege::Vulkan
{
class Instance
{
public:
    typedef Siege::Utils::MHArray<const char*> (*surfaceCallback) (void);
    typedef surfaceCallback GetSurfaceExtensionsCallback;

    typedef bool (*getSurfaceCallback)(VkInstance, VkSurfaceKHR*);
    typedef getSurfaceCallback GetWindowSurfaceCallBack;

    Instance() = default;
    Instance(GetSurfaceExtensionsCallback surfaceExtensionsCallback,
             GetWindowSurfaceCallBack windowSurfaceCallback);
    Instance(Instance&& other);
    ~Instance();

    Instance& operator=(Instance&& other);

    VkInstance GetInstance() const
    {
        return instance;
    }
    VkSurfaceKHR GetSurface() const
    {
        return windowSurface;
    }

private:
    void CreateInstance(const Siege::Utils::MHArray<const char*>& requiredSurfaceExtensions);
    void SetupDebugMessenger();

    bool ValidateLayersExist();
    void CheckInstanceExtensionsExist(const Siege::Utils::MHArray<const char*>& requiredSurfaceExtensions);

    void Move(Instance& other);
    void DestroyDependentObjects();

    VkInstance instance {nullptr};
    VkSurfaceKHR windowSurface {nullptr};
    VkDebugUtilsMessengerEXT debugMessenger {nullptr};
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_INSTANCE_H
