//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#define VOLK_IMPLEMENTATION
#include "Instance.h"

namespace Siege::Vulkan::Instance
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

uint32_t GetInstanceExtensionCount()
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, OUT & extensionCount, nullptr);
    return extensionCount;
}

void GetInstanceExtensionProperties(uint32_t count, VkExtensionProperties* properties)
{
    vkEnumerateInstanceExtensionProperties(nullptr, &count, OUT properties);
}

uint32_t GetInstanceLayerCount()
{
    uint32_t count = 0;
    vkEnumerateInstanceLayerProperties(OUT &count, nullptr);
    return count;
}

void GetInstanceLayerProperties(uint32_t count, VkLayerProperties* properties)
{
    vkEnumerateInstanceLayerProperties(&count, OUT properties);
}

} // namespace Siege::Vulkan