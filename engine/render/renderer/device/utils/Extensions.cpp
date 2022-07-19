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
#include <vector>

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

Array<String> GetRequiredExtensions(bool enableValidationLayers)
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    size_t size = glfwExtensionCount + (1 * enableValidationLayers);

    Array<String> array(size);

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

    std::cout << "available extensions:" << std::endl;
    std::unordered_set<String> available;
    for (size_t i = 0; i < extensionCount; i++)
    {
        VkExtensionProperties extension = extensions[i];
        std::cout << "\t" << extension.extensionName << std::endl;
        available.insert(extension.extensionName);
    }

    std::cout << "required extensions:" << std::endl;
    auto requiredExtensions = GetRequiredExtensions(enableValidationLayers);
    for (const auto& required : requiredExtensions)
    {
        std::cout << "\t" << required << std::endl;
        CC_ASSERT(available.find(required) != available.end(), "Failed to find GLFW Extensions!");
    }
}
} // namespace Siege::Extensions
