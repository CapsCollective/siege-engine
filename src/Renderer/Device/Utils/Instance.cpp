#include <cstring>
#include <glfw/glfw3.h>

#include "Instance.h"

namespace SnekVK
{
    bool CheckValidationLayerSupport(const char** validationLayers, size_t validationLayerCount)
    {
        u32 layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        VkLayerProperties availableLayers[layerCount];
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

        for (size_t i = 0; i < validationLayerCount; i++)
        {
            const char* validationLayer = validationLayers[i];
            bool layerFound = false;
            for (size_t i = 0; i < layerCount; i++)
            {
                if (strcmp(validationLayer, availableLayers[i].layerName) == 0)
                {
                    layerFound = true;
                    break;
                }    
            }
            if (!layerFound) return false;
        }
        return true;
    }

    LayerAndExtensionInfo GetRequiredExtensions(bool enableValidationLayers) 
    {
        u32 glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        u32 totalExtensions = enableValidationLayers ? (glfwExtensionCount + 1) : glfwExtensionCount;

        const char** extensions = new const char*;

        for (size_t i = 0; i < glfwExtensionCount; i++)
        {
            extensions[i] = glfwExtensions[i];
        }

        if (enableValidationLayers) extensions[totalExtensions - 1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

        return { extensions, totalExtensions };
    }

    bool HasGlfwInstanceExtensions(bool enableValidationLayers) 
    {
        bool hasExtensions = true;
        u32 extensionCount = 0;

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        VkExtensionProperties extensions[extensionCount];
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions);

        std::cout << "Available extensions: " << std::endl;
        for (size_t i = 0; i < extensionCount; i++)
        {
            const char* extension = extensions[i].extensionName;
            std::cout << "\t" << extension << std::endl;
        }

        std::cout << "Required extensions: " << std::endl;
        LayerAndExtensionInfo requiredExtensions = GetRequiredExtensions(enableValidationLayers);

        for (size_t i = 0; i < requiredExtensions.count; i++)
        {
            const char* required = requiredExtensions.names[i];
            std::cout << "\t" << required << std::endl;
            bool found = false;
            
            for (size_t j = 0; j < extensionCount; j++)
            {
                const char* extension = extensions[j].extensionName;
                found = strcmp(extension, required) == 0;
                if (found) break;
            }

            if (!found)
            {
                hasExtensions = false;
                break;
            } 
        }

        DestroyLayerAndExtensionInfo(requiredExtensions);
        return hasExtensions;
    }

    void DestroyLayerAndExtensionInfo(LayerAndExtensionInfo& info) { delete [] info.names; }
}