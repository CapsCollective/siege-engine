#include "SwapChainSupportDetails.h"

namespace SnekVk::SwapChainSupportDetails
{
    SwapChainSupportDetails QuerySupport(VkPhysicalDevice device, VkSurfaceKHR& surface) 
    {
        SwapChainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, OUT &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, OUT &formatCount, nullptr);

        if (formatCount != 0) 
        {
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, OUT details.formats);
            details.hasFormats = true;
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, OUT &presentModeCount, nullptr);

        if (presentModeCount != 0) 
        {
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                device,
                surface,
                &presentModeCount,
                OUT details.presentModes);
            details.hasPresentModes = true;
        }
        return details;
    }
}