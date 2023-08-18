//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Instance.h"

#include <utils/Logging.h>

#include <unordered_set>

#include "Config.h"
#include "render/renderer/platform/vulkan/utils/DebugUtilsMessenger.h"
#include "utils/Instance.h"

// TODO: Maybe define all platform-specific macros in another file to avoid clutter?

#ifdef __APPLE__
#define REQUIRES_PORTABILITY_EXTENSION 1

#define GET_MACOS_REQUIRED_EXTENSIONS(collection)                                       \
    collection = Siege::MHArray<const char*>(collection.Data(), collection.Size() + 1); \
    collection[collection.Size() - 1] = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;
#else
#define REQUIRES_PORTABILITY_EXTENSION 0
#define GET_MACOS_REQUIRED_EXTENSIONS(...)
#endif

#if ENABLE_VALIDATION_LAYERS == 1

#define ASSERT_LAYERS_EXIST(func) CC_ASSERT(func(), "Validation Layers are not supported!")

#define SETUP_UTILS_MESSENGER SetupDebugMessenger();

#define DEBUG_EXTENSIONS(name, extensions)                                        \
    name = Siege::MHArray<const char*>(extensions.Data(), extensions.Size() + 1); \
    name[name.Size() - 1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

#define CREATE_VULKAN_INSTANCE(appInfo, extensionCount, extensions, layerCount, layers, flags) \
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;                                        \
                                                                                               \
    CREATE_DEBUG_MESSENGER(debugCreateInfo)                                                    \
    instance = VulkanInstance::CreateInstance(appInfo,                                         \
                                              extensionCount,                                  \
                                              extensions,                                      \
                                              layerCount,                                      \
                                              layers,                                          \
                                              &debugCreateInfo)
#else

#define ASSERT_LAYERS_EXIST(...)
#define DEBUG_EXTENSIONS(name, extensions) \
    name = Siege::MHArray<const char*>(extensions.Data(), extensions.Size());

#define SETUP_UTILS_MESSENGER

#define CREATE_VULKAN_INSTANCE(appInfo, extensionCount, extensions, layerCount, layers, flags) \
    instance =                                                                                 \
        VulkanInstance::CreateInstance(appInfo, extensionCount, extensions, layerCount, layers)
#endif

namespace Siege::Vulkan
{
Instance::Instance(MHArray<const char*> surfaceExtensions)
{
    CreateInstance(surfaceExtensions);

    SETUP_UTILS_MESSENGER
}

Instance::Instance(Instance&& other)
{
    Move(other);
}

Instance::~Instance()
{
    if (!instance && !debugMessenger) return;

    if (instance) DestroyDependentObjects();

    vkDestroyInstance(instance, nullptr);

    instance = VK_NULL_HANDLE;
    debugMessenger = VK_NULL_HANDLE;

    CC_LOG_INFO("Vulkan Instance has been destroyed.")
}

Instance& Instance::operator=(Instance&& other)
{
    Move(other);
    return *this;
}

void Instance::CreateInstance(const Siege::MHArray<const char*>& requiredSurfaceExtensions)
{
    ASSERT_LAYERS_EXIST(ValidateLayersExist)

    // Specify general app information.
    auto appInfo = VulkanInstance::AppInfo("Siege Renderer",
                                           VK_MAKE_API_VERSION(1, 0, 0, 0),
                                           "No Engine",
                                           VK_MAKE_API_VERSION(1, 0, 0, 0),
                                           VK_API_VERSION_1_3);

    Siege::MHArray<const char*> extensions(requiredSurfaceExtensions.Size());
    DEBUG_EXTENSIONS(extensions, requiredSurfaceExtensions)
    GET_MACOS_REQUIRED_EXTENSIONS(extensions)

    CREATE_VULKAN_INSTANCE(appInfo,
                           extensions.Size(),
                           extensions.Data(),
                           Config::validationLayers.Size(),
                           Config::validationLayers.Data(),
                           0);

    CheckInstanceExtensionsExist(extensions);

    volkLoadInstance(instance);
}

void Instance::Move(Instance& other)
{
    instance = other.instance;
    debugMessenger = other.debugMessenger;

    other.instance = VK_NULL_HANDLE;
    other.debugMessenger = VK_NULL_HANDLE;
}

void Instance::SetupDebugMessenger()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    DebugUtilsMessenger::PopulateCreateInfo(OUT createInfo);

    CC_ASSERT(DebugUtilsMessenger::CreateMessenger(instance,
                                                   &createInfo,
                                                   nullptr,
                                                   OUT & debugMessenger) == VK_SUCCESS,
              "Failed to create DebugUtilsMessenger!")
}

bool Instance::ValidateLayersExist()
{
    auto& layers = Config::validationLayers;

    auto availableLayers = VulkanInstance::GetInstanceLayerProperties();

    for (auto it = layers.CreateIterator(); it; ++it)
    {
        bool layerFound = false;

        for (auto layerIt = availableLayers.CreateIterator(); layerIt; ++layerIt)
        {
            if (strcmp(*it, (*layerIt).layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            return false;
        }
    }
    return true;
}

void Instance::CheckInstanceExtensionsExist(
    const Siege::MHArray<const char*>& requiredSurfaceExtensions)
{
    auto extensions = VulkanInstance::GetInstanceExtensionProperties();

    String availableExtensions;

    std::unordered_set<String> available(requiredSurfaceExtensions.Size());
    for (auto it = extensions.CreateFIterator(); it; ++it)
    {
        auto extension = *it;
        availableExtensions += String("\n\t %s").Formatted(extension.extensionName);
        available.insert(extension.extensionName);
    }

    CC_LOG_INFO("Available Extensions: {}", availableExtensions)

    String requiredExtensionsMsg;

    for (auto it = requiredSurfaceExtensions.CreateFIterator(); it; ++it)
    {
        auto required = *it;
        requiredExtensionsMsg += String("\n\t %s").Formatted(required);
        CC_ASSERT(available.find(required) != available.end(), "Failed to find Window Extensions!")
    }

    CC_LOG_INFO("Required Extensions: {}", requiredExtensionsMsg)
}

void Instance::DestroyDependentObjects()
{
    DESTROY_DEBUG_MESSENGER(debugMessenger)
}
} // namespace Siege::Vulkan
