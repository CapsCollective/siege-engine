//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Extensions.h"

#include <GLFW/glfw3.h>

#include <unordered_set>
#include <utils/Logging.h>
#include <vector>

#ifdef __APPLE__
#define REQUIRES_PORTABILITY_EXTENSION 1

#define GET_MACOS_REQUIRED_EXTENSIONS(collection, size, offset) \
    collection = Utils::MHArray<String>(size);                \
    collection[size - offset] = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;
#else
#define REQUIRES_PORTABILITY_EXTENSION 0
#define GET_MACOS_REQUIRED_EXTENSIONS(...)
#endif

namespace Siege::Extensions
{
bool CheckValidationLayerSupport(const String* validationLayers, size_t size)
{
    // Get an array of all available validation layers.
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(OUT & layerCount, nullptr);

    VkLayerProperties availableLayers[layerCount];
    vkEnumerateInstanceLayerProperties(&layerCount, OUT availableLayers);

    for (size_t i = 0; i < size; i++)
    {
        const char* layerName = validationLayers[i].Str();
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

Utils::MHArray<String> GetRequiredExtensions(bool enableValidationLayers)
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    size_t offset = enableValidationLayers + REQUIRES_PORTABILITY_EXTENSION;

    size_t size = glfwExtensionCount + offset;

    Utils::MHArray<String> array(size);

    GET_MACOS_REQUIRED_EXTENSIONS(array, size, offset)

    for (size_t i = 0; i < glfwExtensionCount; i++) array[i] = glfwExtensions[i];

    if (enableValidationLayers) array[size - 1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

    return array;
}

void HasGflwRequiredInstanceExtensions(bool enableValidationLayers)
{
    // Get an array of all available instance extensions.
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, OUT & extensionCount, nullptr);
    VkExtensionProperties extensions[extensionCount];
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, OUT extensions);

    String availableExtensions;

    std::unordered_set<String> available;
    for (size_t i = 0; i < extensionCount; i++)
    {
        VkExtensionProperties extension = extensions[i];
        availableExtensions += String("\n\t %s").Formatted(extension.extensionName);
        available.insert(extension.extensionName);
    }

    CC_LOG_INFO("Available Extensions: {}", availableExtensions)

    String requiredExtensionsMsg;

    auto requiredExtensions = GetRequiredExtensions(enableValidationLayers);
    for (auto it = requiredExtensions.CreateIterator(); it; ++it)
    {
        auto required = *it;
        requiredExtensionsMsg += String("\n\t %s").Formatted(required.Str());
        CC_ASSERT(available.find(required) != available.end(), "Failed to find GLFW Extensions!")
    }

    CC_LOG_INFO("Required Extensions: {}", requiredExtensionsMsg)
}
} // namespace Siege::Extensions
