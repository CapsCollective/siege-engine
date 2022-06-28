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
#include <utils/String.h>
#include <vector>

namespace Siege::Extensions
{
bool CheckValidationLayerSupport(const char* const* validationLayers, size_t size)
{
    // Get an array of all available validation layers.
    u32 layerCount;
    vkEnumerateInstanceLayerProperties(OUT & layerCount, nullptr);

    VkLayerProperties availableLayers[layerCount];
    vkEnumerateInstanceLayerProperties(&layerCount, OUT availableLayers);

    for (size_t i = 0; i < size; i++)
    {
        const char* layerName = validationLayers[i];
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

std::vector<const char*> GetRequiredExtensions(bool enableValidationLayers)
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

void HasGflwRequiredInstanceExtensions(bool enableValidationLayers)
{
    // Get an array of all available instance extensions.
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, OUT & extensionCount, nullptr);
    VkExtensionProperties extensions[extensionCount];
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, OUT extensions);

    String availableExtensionMessage = "Available extensions:\n";

    std::unordered_set<std::string> available;
    for (size_t i = 0; i < extensionCount; i++)
    {
        VkExtensionProperties extension = extensions[i];
        availableExtensionMessage.Append(String("\t") + String(extension.extensionName) + String("\n"));
        available.insert(extension.extensionName);
    }

    CC_LOG_INFO("{}",availableExtensionMessage.Str())

    String requiredExtensionMessage = "Required extensions:";
    auto requiredExtensions = GetRequiredExtensions(enableValidationLayers);
    for (const auto& required : requiredExtensions)
    {
        requiredExtensionMessage.Append(String("\t") + String(required) + String("\n"));
        CC_ASSERT(available.find(required) != available.end(), "Failed to find GLFW Extensions!")
    }
}
} // namespace Siege::Extensions