//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_INSTANCE_H
#define SIEGE_ENGINE_INSTANCE_H

#include "volk/volk.h"

namespace Siege::Vulkan
{
class Instance
{
public:

    static VkApplicationInfo AppInfo(const char* appName,
                                     uint32_t version,
                                     const char* engineName,
                                     uint32_t engineVersion,
                                     uint32_t apiVersion,
                                     const void* pNext = nullptr);

    static VkInstanceCreateInfo CreateInfo(const VkApplicationInfo* appInfo,
                                           uint32_t enabledExtensionCount,
                                           const char* const* enabledExtensions,
                                           uint32_t enabledLayerCount = 0,
                                           const char* const* enabledLayers = nullptr,
                                           VkInstanceCreateFlags flags = 0,
                                           const void* pNext = nullptr);

    static void Create(const VkInstanceCreateInfo* createInfo,
                       const VkAllocationCallbacks* callbacks,
                       VkInstance* instance);

    static VkInstance CreateInstance(VkApplicationInfo appInfo,
                                     uint32_t extensionSize,
                                     const char* const* extensions,
                                     uint32_t layerSize,
                                     const char* const* layers,
                                     VkDebugUtilsMessengerCreateInfoEXT* debugUils = nullptr);
};

} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_INSTANCE_H
