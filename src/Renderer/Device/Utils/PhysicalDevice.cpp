#include "PhysicalDevice.h"
#include <set>

namespace SnekVk::PhysicalDevice
{
    bool CheckExtensionSupport(
        VkPhysicalDevice device, 
        const char* const* deviceExtensions, 
        size_t deviceExtensionCount) 
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, OUT &extensionCount, nullptr);

        VkExtensionProperties availableExtensions[extensionCount];
        vkEnumerateDeviceExtensionProperties(
            device,
            nullptr,
            &extensionCount,
            OUT availableExtensions);

        // We use a set here so that we can store only unique values.
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
            // Check if the device supports the image formats and present modes needed to render to the screen.
            SwapChainSupportDetails::SwapChainSupportDetails swapChainSupport = SwapChainSupportDetails::QuerySupport(device, surface);
            swapChainAdequate = swapChainSupport.hasFormats && swapChainSupport.hasPresentModes;
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, OUT &supportedFeatures);

        // A device is only suitable if it ticks the following boxes:
        // 1) All extensions are supported.
        // 2) It has the supported formats and present modes.
        // 3) It has a graphics and present queues.
        // 4) It supports sampler anistropy.
        return QueueFamilyIndices::IsComplete(indices) && extensionsSupported && swapChainAdequate &&
                supportedFeatures.samplerAnisotropy;
    }
}