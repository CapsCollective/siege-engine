//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Instance.h"

#include <utils/Logging.h>
#include <utils/collections/HeapArray.h>

#ifdef __APPLE__

#define ADD_PORTABILITY_BIT(info) info.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#else

#define ADD_PORTABILITY_BIT(...)
#endif

#if ENABLE_VALIDATION_LAYERS == 1
#define CREATE_INSTANCE(instance, appInfo, extensionSize, extensions, layerCount, layers, ...) \
    VkInstanceCreateInfo createInfo = {};                                                      \
    createInfo =                                                                               \
        CreateInfo(&appInfo, extensionSize, extensions, layerCount, layers, 0, __VA_ARGS__);   \
    ADD_PORTABILITY_BIT(createInfo)                                                            \
    Create(&createInfo, nullptr, &instance);
#else
#define CREATE_INSTANCE(instance, appInfo, extensionSize, extensions, layerCount, layers, ...) \
    VkInstanceCreateInfo createInfo = {};                                                      \
    createInfo = CreateInfo(&appInfo, extensionSize, extensions);                              \
    ADD_PORTABILITY_BIT(createInfo)                                                            \
    Create(&createInfo, nullptr, &instance);
#endif

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

Utils::MHArray<VkLayerProperties> GetInstanceLayerProperties()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(OUT & layerCount, nullptr);

    VkLayerProperties availableLayers[layerCount];
    vkEnumerateInstanceLayerProperties(&layerCount, OUT availableLayers);

    return Utils::MHArray<VkLayerProperties>(availableLayers, layerCount);
}

Utils::MHArray<VkExtensionProperties> GetInstanceExtensionProperties()
{
    // Get an array of all available instance extensions.
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, OUT & extensionCount, nullptr);
    VkExtensionProperties extensions[extensionCount];
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, OUT extensions);

    return Utils::MHArray<VkExtensionProperties>(extensions, extensionCount);
}

void Create(const VkInstanceCreateInfo* createInfo,
            const VkAllocationCallbacks* callbacks,
            VkInstance* instance) {

    CC_ASSERT(vkCreateInstance(createInfo, callbacks, OUT instance) == VK_SUCCESS,
              "Unable to create Vulkan Instance!")}

VkInstance CreateInstance(VkApplicationInfo appInfo,
                          uint32_t extensionSize,
                          const char* const* extensions,
                          uint32_t layerSize,
                          const char* const* layers,
                          VkDebugUtilsMessengerCreateInfoEXT* debugUils)
{
    VkInstance instance;
    CREATE_INSTANCE(instance, appInfo, extensionSize, extensions, layerSize, layers, debugUils)
    return instance;
}

} // namespace Siege::Vulkan::VulkanInstance
