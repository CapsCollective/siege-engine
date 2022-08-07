//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_INSTANCE_H
#define SIEGE_ENGINE_VULKAN_INSTANCE_H

#include <utils/Logging.h>
#include <utils/collections/HeapArray.h>
#include <volk/volk.h>

namespace Siege::Vulkan::VulkanInstance
{
VkApplicationInfo AppInfo(const char* appName,
                          uint32_t version,
                          const char* engineName,
                          uint32_t engineVersion,
                          uint32_t apiVersion,
                          const void* pNext = nullptr);

VkInstanceCreateInfo CreateInfo(const VkApplicationInfo* appInfo,
                                uint32_t enabledExtensionCount,
                                const char* const* enabledExtensions,
                                uint32_t enabledLayerCount = 0,
                                const char* const* enabledLayers = nullptr,
                                VkInstanceCreateFlags flags = 0,
                                const void* pNext = nullptr);

inline void Create(const VkInstanceCreateInfo* createInfo,
                   const VkAllocationCallbacks* callbacks,
                   VkInstance* instance);

VkInstance CreateInstance(VkApplicationInfo appInfo,
                          uint32_t extensionSize,
                          const char* const* extensions,
                          uint32_t layerSize,
                          const char* const* layers,
                          VkDebugUtilsMessengerCreateInfoEXT* debugUils = nullptr);

Siege::Utils::MHArray<VkLayerProperties> GetInstanceLayerProperties();
Siege::Utils::MHArray<VkExtensionProperties> GetInstanceExtensionProperties();
} // namespace Siege::Vulkan::VulkanInstance

#endif // SIEGE_ENGINE_VULKAN_INSTANCE_H
