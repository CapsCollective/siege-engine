//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Instance.h"

namespace Siege::Vulkan::VulkanInstance
{

VkApplicationInfo AppInfo(const char* appName,
                                 uint32_t version,
                                 const char* engineName,
                                 uint32_t engineVersion,
                                 uint32_t apiVersion,
                                 const void* pNext)
{
    return {VK_STRUCTURE_TYPE_APPLICATION_INFO,
            pNext,
            appName,
            version,
            engineName,
            engineVersion,
            apiVersion};
}

VkInstanceCreateInfo CreateInfo(const VkApplicationInfo* appInfo,
                              uint32_t enabledExtensionCount,
                              const char* const* enabledExtensions,
                              uint32_t enabledLayerCount,
                              const char* const* enabledLayers,
                              VkInstanceCreateFlags flags,
                              const void* pNext)
{
    return {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            pNext,
            flags,
            appInfo,
            enabledLayerCount,
            enabledLayers,
            enabledExtensionCount,
            enabledExtensions};
}

Array<VkLayerProperties> GetInstanceLayerProperties()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(OUT & layerCount, nullptr);

    VkLayerProperties availableLayers[layerCount];
    vkEnumerateInstanceLayerProperties(&layerCount, OUT availableLayers);

    return Array<VkLayerProperties>(availableLayers, layerCount);
}

Array<VkExtensionProperties> GetInstanceExtensionProperties()
{
    // Get an array of all available instance extensions.
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, OUT & extensionCount, nullptr);
    VkExtensionProperties extensions[extensionCount];
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, OUT extensions);

    return Array<VkExtensionProperties>(extensions, extensionCount);
}

} // namespace Siege::Vulkan