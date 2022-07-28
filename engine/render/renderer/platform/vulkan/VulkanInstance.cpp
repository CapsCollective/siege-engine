//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "VulkanInstance.h"

#include <unordered_set>

#include "Config.h"
#include "Macros.h"
#include "render/renderer/platform/vulkan/utils/DebugUtilsMessenger.h"

#if ENABLE_VALIDATION_LAYERS == 1

#define ASSERT_LAYERS_EXIST                                               \
    CC_ASSERT(Siege::Vulkan::VulkanInstance::ValidationLayersSupported(), \
              "Validation Layers are not supported!");

#define CREATE_VULKAN_INSTANCE(appInfo, extensions, extensionCount, layers, layerCount, flags) \
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;                                        \
                                                                                               \
    CREATE_DEBUG_MESSENGER(debugCreateInfo)                                                    \
    CREATE_INSTANCE(appInfo,                                                                   \
                    extensionCount,                                                            \
                    extensions,                                                                \
                    layerCount,                                                                \
                    layers,                                                                    \
                    0,                                                                         \
                    reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&debugCreateInfo))

#define REQUIRED_EXT_COUNT(count) count + 1;
#define SET_DEBUG_EXT(count, arr) arr[count - 1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
#define SETUP_UTILS_MESSENGER SetupDebugMessenger();
#else
#define ASSERT_LAYERS_EXIST
#define CREATE_VULKAN_INSTANCE(appInfo, extensions, extensionCount, layers, layerCount, flags) \
    CREATE_INSTANCE(appInfo, extensionCount, extensions, layerCount, layers, flags)
#define REQUIRED_EXT_COUNT(count) count
#define SET_DEBUG_EXT(...)
#define SETUP_UTILS_MESSENGER
#endif

namespace Siege::Vulkan
{
VulkanInstance::VulkanInstance(SurfaceFunc surfaceCreationCallback,
                               const char** requiredExtensions,
                               const size_t& count)
{
    CC_ASSERT(volkInitialize() == VK_SUCCESS, "Unable to initialise Volk!")

    ASSERT_LAYERS_EXIST

    // Specify general app information.
    auto appInfo = Instance::AppInfo("Siege Renderer",
                                     VK_MAKE_VERSION(1, 0, 0),
                                     "No Engine",
                                     VK_MAKE_VERSION(1, 0, 0),
                                     VK_API_VERSION_1_2);

    auto extensions = GetRequiredExtensions(requiredExtensions, count);

    CREATE_VULKAN_INSTANCE(appInfo,
                           extensions.Data(),
                           static_cast<uint32_t>(extensions.Size()),
                           Config::validationLayers.data(),
                           static_cast<uint32_t>(Config::validationLayers.size()),
                           0)

    HasRequiredGLFWInstanceExtensions(requiredExtensions, count);

    volkLoadInstance(instance);

    SETUP_UTILS_MESSENGER

    surfaceCreationCallback(instance, OUT & surface);
}

VulkanInstance::VulkanInstance(VulkanInstance&& other)
{
    instance = other.instance;
    debugMessenger = other.debugMessenger;
    surface = other.surface;

    other.instance = VK_NULL_HANDLE;
    other.debugMessenger = VK_NULL_HANDLE;
    other.surface = VK_NULL_HANDLE;
}

VulkanInstance::~VulkanInstance()
{
    DESTROY_DEBUG_MESSENGER(debugMessenger)

    if (instance) vkDestroySurfaceKHR(instance, surface, nullptr);

    vkDestroyInstance(instance, nullptr);

    surface = VK_NULL_HANDLE;
    debugMessenger = VK_NULL_HANDLE;
    instance = VK_NULL_HANDLE;
}

VulkanInstance& VulkanInstance::operator=(VulkanInstance&& other)
{
    instance = other.instance;
    debugMessenger = other.debugMessenger;
    surface = other.surface;

    other.instance = VK_NULL_HANDLE;
    other.debugMessenger = VK_NULL_HANDLE;
    other.surface = VK_NULL_HANDLE;
    return *this;
}

Array<const char*> VulkanInstance::GetRequiredExtensions(const char** requiredExtensions,
                                                         const size_t& count)
{
    size_t requiredCount = REQUIRED_EXT_COUNT(count);
    const char* extensions[requiredCount];
    memcpy(extensions, requiredExtensions, sizeof(const char**) * count);
    SET_DEBUG_EXT(requiredCount, extensions)

    return Array<const char*>(extensions, requiredCount);
}

void VulkanInstance::HasRequiredGLFWInstanceExtensions(const char** requiredExtensions,
                                                       const size_t& count)
{
    auto instanceCount = Instance::GetInstanceExtensionCount();
    VkExtensionProperties extensions[instanceCount];
    Instance::GetInstanceExtensionProperties(instanceCount, extensions);

    String availableExtensions;

    std::unordered_set<String> available;
    for (size_t i = 0; i < instanceCount; i++)
    {
        VkExtensionProperties extension = extensions[i];
        availableExtensions += String("\n\t %s").Formatted(extension.extensionName);
        available.insert(extension.extensionName);
    }

    CC_LOG_INFO("Available Extensions: {}", availableExtensions)

    String requiredExtensionsMsg;

    auto reqExtensions = GetRequiredExtensions(requiredExtensions, count);
    for (const auto& required : reqExtensions)
    {
        requiredExtensionsMsg += String("\n\t %s").Formatted(required);
        CC_ASSERT(available.find(required) != available.end(), "Failed to find surface Extensions!")
    }

    CC_LOG_INFO("Required Extensions: {}", requiredExtensionsMsg)
}

bool VulkanInstance::ValidationLayersSupported()
{
    // Get an array of all available validation layers.
    uint32_t layerCount = Instance::GetInstanceLayerCount();
    VkLayerProperties availableLayers[layerCount];
    Instance::GetInstanceLayerProperties(layerCount, availableLayers);

    auto layers = Config::validationLayers;

    for (size_t i = 0; i < layers.size(); i++)
    {
        const char* layerName = layers[i];
        bool layerFound = false;

        for (size_t j = 0; j < layerCount; j++)
        {
            VkLayerProperties layerProperties = availableLayers[j];
            if (strcmp(layerName, layerProperties.layerName) == 0)
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

void VulkanInstance::SetupDebugMessenger()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    DebugUtilsMessenger::PopulateCreateInfo(OUT createInfo);

    CC_ASSERT(DebugUtilsMessenger::CreateMessenger(instance,
                                                   &createInfo,
                                                   nullptr,
                                                   OUT & debugMessenger) == VK_SUCCESS,
              "Failed to create DebugUtilsMessenger!")
}

} // namespace Siege::Vulkan