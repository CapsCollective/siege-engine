#include "PhysicalDevice.h"
#include <cstring>

namespace SnekVK
{
    bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const char** deviceExtensions, size_t deviceExtensionCount)
    {
        QueueFamilyIndices indices = FindQueueFamilies(device, surface);

        bool extensionsSupported = CheckDeviceExtensionSupport(device, deviceExtensions, deviceExtensionCount);

        bool swapChainAdequate = false;

        if (extensionsSupported)
        {
            SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device, surface);
            swapChainAdequate = swapChainSupport.hasPresentMode && swapChainSupport.hasSurfaceFormat;
            DestroySwapChainSupportDetails(swapChainSupport);
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        return IsComplete(indices) && extensionsSupported && swapChainAdequate &&
                supportedFeatures.samplerAnisotropy;
    }

    bool CheckDeviceExtensionSupport(VkPhysicalDevice device, const char** deviceExtensions, size_t deviceExtensionCount)
    {
        u32 extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        VkExtensionProperties availableExtensions[extensionCount];
        vkEnumerateDeviceExtensionProperties(
            device, 
            nullptr,
            &extensionCount,
            availableExtensions
        );

        bool hasRequiredExtensions = true;

        for (size_t i = 0; i < deviceExtensionCount; i++)
        {
            const char* requiredExtension = deviceExtensions[i];
            bool hasExtension = false;
            for (size_t i = 0; i < extensionCount; i++)
            {
                const char* deviceExtension = availableExtensions[i].extensionName;
                if (strcmp(requiredExtension, deviceExtension) == 0)
                {
                    hasExtension = true;
                    break;
                }
            }
            if (!hasExtension)
            {
                hasRequiredExtensions = false;
                break;
            }
        }

        return hasRequiredExtensions;
    }
}