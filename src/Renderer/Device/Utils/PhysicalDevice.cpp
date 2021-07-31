#include "PhysicalDevice.h"
#include <set>

namespace SnekVk::PhysicalDevice
{
    bool CheckExtensionSupport(
        VkPhysicalDevice device, 
        const char* const* deviceExtensions, 
        size_t deviceExtensionCount
    ) 
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, OUT &extensionCount, nullptr);

        VkExtensionProperties availableExtensions[extensionCount];
        vkEnumerateDeviceExtensionProperties(
            device,
            nullptr,
            &extensionCount,
            OUT availableExtensions);

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

    bool IsSuitable(VkPhysicalDevice device, VkSurfaceKHR& surface, const char *const * deviceExtensions, size_t deviceExtensionCount) 
    {
        QueueFamilyIndices::QueueFamilyIndices indices = QueueFamilyIndices::FindQueueFamilies(device, surface);

        bool extensionsSupported = CheckExtensionSupport(device, deviceExtensions, deviceExtensionCount);

        bool swapChainAdequate = false;
        if (extensionsSupported) 
        {
            SwapChainSupportDetails::SwapChainSupportDetails swapChainSupport = SwapChainSupportDetails::QuerySupport(device, surface);
            swapChainAdequate = swapChainSupport.hasFormats && swapChainSupport.hasPresentModes;
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, OUT &supportedFeatures);

        return QueueFamilyIndices::IsComplete(indices) && extensionsSupported && swapChainAdequate &&
                supportedFeatures.samplerAnisotropy;
    }
}