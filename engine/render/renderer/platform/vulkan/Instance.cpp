//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Instance.h"

#include <utils/Logging.h>

#include <unordered_set>

#include "render/renderer/platform/vulkan/utils/DebugUtilsMessenger.h"
#include "utils/Instance.h"

#if ENABLE_VALIDATION_LAYERS == 1

#define ASSERT_LAYERS_EXIST(func) CC_ASSERT(func(), "Validation Layers are not supported!")

#define SETUP_UTILS_MESSENGER SetupDebugMessenger();

#define DEBUG_EXTENSIONS(name, extensions)                               \
    name = Array<const char*>(extensions.Data(), extensions.Size() + 1); \
    name[name.Size() - 1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

#define CREATE_VULKAN_INSTANCE(appInfo, extensionCount, extensions, layerCount, layers, flags) \
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;                                        \
                                                                                               \
    CREATE_DEBUG_MESSENGER(debugCreateInfo)                                                    \
    CREATE_INSTANCE(appInfo,                                                                   \
                    extensionCount,                                                            \
                    extensions,                                                                \
                    layerCount,                                                                \
                    layers,                                                                    \
                    flags,                                                                     \
                    reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&debugCreateInfo))
#else

#define ASSERT_LAYERS_EXIST(...)
#define DEBUG_EXTENSIONS(...)
#define SETUP_UTILS_MESSENGER

#define CREATE_VULKAN_INSTANCE(appInfo, extensionCount, extensions, layerCount, layers, flags) \
    CREATE_INSTANCE(appInfo, extensionCount, extensions, layerCount, layers, flags)

#endif

namespace Siege::Vulkan
{
Instance::Instance(GetSurfaceExtensionsCallback surfaceExtensionsCallback,
                   GetWindowSurfaceCallBack windowSurfaceCallback)
{
    CreateInstance(surfaceExtensionsCallback());

    SETUP_UTILS_MESSENGER

    CC_ASSERT(windowSurfaceCallback(instance, &windowSurface), "Unable to create window surface!")
}

Instance::Instance(Instance&& other)
{
    Move(other);
}

Instance::~Instance()
{
    if (instance) DestroyDependentObjects();

    vkDestroyInstance(instance, nullptr);

    instance = VK_NULL_HANDLE;
    windowSurface = VK_NULL_HANDLE;
    debugMessenger = VK_NULL_HANDLE;

    CC_LOG_INFO("Vulkan Instance has been destroyed.")
}

Instance& Instance::operator=(Instance&& other)
{
    Move(other);
    return *this;
}

void Instance::CreateInstance(const Array<const char*>& requiredSurfaceExtensions)
{
    ASSERT_LAYERS_EXIST(ValidateLayersExist)

    // Specify general app information.
    auto appInfo = VulkanInstance::AppInfo("Siege Renderer",
                                           VK_MAKE_VERSION(1, 0, 0),
                                           "No Engine",
                                           VK_MAKE_VERSION(1, 0, 0),
                                           VK_API_VERSION_1_2);

    Array<const char*> extensions(requiredSurfaceExtensions.Size());
    DEBUG_EXTENSIONS(extensions, requiredSurfaceExtensions)

    CREATE_VULKAN_INSTANCE(appInfo,
                           extensions.Size(),
                           extensions.Data(),
                           Config::validationLayers.size(),
                           Config::validationLayers.data(),
                           0)

    CheckInstanceExtensionsExist(extensions);

    volkLoadInstance(instance);
}

void Instance::Move(Instance& other)
{
    instance = other.instance;
    windowSurface = other.windowSurface;
    debugMessenger = other.debugMessenger;

    other.instance = VK_NULL_HANDLE;
    other.windowSurface = VK_NULL_HANDLE;
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

    for (auto& requiredLayer : layers)
    {
        bool layerFound = false;

        for (auto& availableLayer : availableLayers)
        {
            if (strcmp(requiredLayer, availableLayer.layerName) == 0)
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

void Instance::CheckInstanceExtensionsExist(const Array<const char*>& requiredSurfaceExtensions)
{
    auto extensions = VulkanInstance::GetInstanceExtensionProperties();

    String availableExtensions;

    std::unordered_set<String> available(requiredSurfaceExtensions.Size());
    for (auto& extension : extensions)
    {
        availableExtensions += String("\n\t %s").Formatted(extension.extensionName);
        available.insert(extension.extensionName);
    }

    CC_LOG_INFO("Available Extensions: {}", availableExtensions)

    String requiredExtensionsMsg;

    for (auto& required : requiredSurfaceExtensions)
    {
        requiredExtensionsMsg += String("\n\t %s").Formatted(required);
        CC_ASSERT(available.find(required) != available.end(), "Failed to find GLFW Extensions!")
    }

    CC_LOG_INFO("Required Extensions: {}", requiredExtensionsMsg)
}

void Instance::DestroyDependentObjects()
{
    DESTROY_DEBUG_MESSENGER(debugMessenger)
    vkDestroySurfaceKHR(instance, windowSurface, nullptr);
}
} // namespace Siege::Vulkan