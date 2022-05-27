#include "Extensions.h"
#include <vector>
#include <GLFW/glfw3.h>
#include <unordered_set>

namespace SnekVk::Extensions
{
    bool CheckValidationLayerSupport(const char* const* validationLayers, size_t size) 
    {
        // Get an array of all available validation layers.
        u32 layerCount;
        vkEnumerateInstanceLayerProperties(OUT &layerCount, nullptr);

        VkLayerProperties availableLayers[layerCount];
        vkEnumerateInstanceLayerProperties(&layerCount, OUT availableLayers);

        for (size_t i = 0; i < size; i++) 
        {
            const char *layerName = validationLayers[i];
            bool layerFound = false;

            for (size_t i = 0; i < layerCount; i++) 
            {
                VkLayerProperties layerProperties = availableLayers[i];
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

    std::vector<const char *> GetRequiredExtensions(bool enableValidationLayers) {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    void HasGflwRequiredInstanceExtensions(bool enableValidationLayers) 
    {
        // Get an array of all available instance extensions.
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, OUT &extensionCount, nullptr);
        VkExtensionProperties extensions[extensionCount];
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, OUT extensions);

        std::cout << "available extensions:" << std::endl;
        std::unordered_set<std::string> available;
        for (size_t i = 0; i < extensionCount; i++) 
        {
            VkExtensionProperties extension = extensions[i];
            std::cout << "\t" << extension.extensionName << std::endl;
            available.insert(extension.extensionName);
        }

        std::cout << "required extensions:" << std::endl;
        auto requiredExtensions = GetRequiredExtensions(enableValidationLayers);
        for (const auto &required : requiredExtensions) 
        {
            std::cout << "\t" << required << std::endl;
            SNEK_ASSERT(available.find(required) != available.end(), 
                "Failed to find GLFW Extensions!");
        }
    }
}