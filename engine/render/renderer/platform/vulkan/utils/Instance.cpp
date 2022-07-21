//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Instance.h"

namespace Siege::Vulkan
{

VkApplicationInfo Instance::AppInfo(const char* appName,
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

VkInstanceCreateInfo Instance::CreateInfo(const VkApplicationInfo* appInfo,
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

} // namespace Siege