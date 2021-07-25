#include "PhysicalDevice.h"
#include <set>

namespace SnekVk
{
    bool CheckDeviceExtensionSupport(
        VkPhysicalDevice device, 
        const char* const* deviceExtensions, 
        size_t deviceExtensionCount
    ) 
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        VkExtensionProperties availableExtensions[extensionCount];
        vkEnumerateDeviceExtensionProperties(
            device,
            nullptr,
            &extensionCount,
            availableExtensions);

        std::set<std::string> requiredExtensions;

        for (size_t i = 0; i < deviceExtensionCount; i++)
        {
            requiredExtensions.insert(deviceExtensions[i]);
        }

        bool foundAll = true;
        for (size_t i = 0; i < extensionCount; i++) 
        {
            VkExtensionProperties extension = availableExtensions[i];
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR& surface, const char *const * deviceExtensions, size_t deviceExtensionCount) 
    {
        SnekVk::QueueFamilyIndices indices = SnekVk::FindQueueFamilies(device, surface);

        bool extensionsSupported = SnekVk::CheckDeviceExtensionSupport(device, deviceExtensions, deviceExtensionCount);

        bool swapChainAdequate = false;
        if (extensionsSupported) 
        {
            SnekVk::SwapChainSupportDetails swapChainSupport = SnekVk::QuerySwapChainSupport(device, surface);
            swapChainAdequate = swapChainSupport.hasFormats && swapChainSupport.hasPresentModes;
            SnekVk::DestroySwapChainSupportDetails(swapChainSupport);
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        return SnekVk::IsComplete(indices) && extensionsSupported && swapChainAdequate &&
                supportedFeatures.samplerAnisotropy;
    }
}