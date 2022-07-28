//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_INSTANCE_H
#define SIEGE_ENGINE_VULKAN_INSTANCE_H

#include <utils/Logging.h>
#include <volk/volk.h>

#if ENABLE_VALIDATION_LAYERS == 1
#define CREATE_INSTANCE(appInfo, extensionSize, extensions, layerCount, layers, ...) \
    VkInstanceCreateInfo createInfo = {};                                            \
    createInfo = Vulkan::Instance::CreateInfo(&appInfo,                              \
                                              extensionSize,                         \
                                              extensions,                            \
                                              layerCount,                            \
                                              layers,                                \
                                              __VA_ARGS__);                          \
    Vulkan::Instance::Create(&createInfo, nullptr, &instance);
#else
#define CREATE_INSTANCE(appInfo, extensionSize, extensions, layerCount, layers, ...) \
    VkInstanceCreateInfo createInfo = {};                                            \
    createInfo = Vulkan::Instance::CreateInfo(&appInfo, extensionSize, extensions);  \
    Vulkan::Instance::Create(&createInfo, nullptr, &instance);
#endif

namespace Siege::Vulkan::Instance
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

uint32_t GetInstanceExtensionCount();
void GetInstanceExtensionProperties(uint32_t count, VkExtensionProperties* properties);

uint32_t GetInstanceLayerCount();
void GetInstanceLayerProperties(uint32_t count, VkLayerProperties* properties);

inline void Create(const VkInstanceCreateInfo* createInfo,
                   const VkAllocationCallbacks* callbacks,
                   ::VkInstance* instance)
{
    CC_ASSERT(vkCreateInstance(createInfo, callbacks, OUT instance) == VK_SUCCESS,
              "Unable to create Vulkan Instance!")
}
} // namespace Siege::Vulkan::Instance

#endif // SIEGE_ENGINE_INSTANCE_H
