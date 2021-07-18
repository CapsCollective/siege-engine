#include "SwapChainSupportDetails.h"

namespace SnekVK
{
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        SwapChainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        u32 formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
 
        VkSurfaceFormatKHR* formats = new VkSurfaceFormatKHR;
        if (formatCount != 0)
        {
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats);
        }

        u32 presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        VkPresentModeKHR* presentModes = new VkPresentModeKHR;

        if (presentModeCount != 0)
        {
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                device,
                surface, 
                &presentModeCount,
                presentModes
            );
        }

        details.formats = formats;
        details.hasSurfaceFormat = formatCount > 0;
        details.presentModes = presentModes;
        details.hasPresentMode = presentModeCount > 0;

        return details;
    }

    void DestroySwapChainSupportDetails(SwapChainSupportDetails& swapChainSupport)
    {
        delete swapChainSupport.formats;
        delete swapChainSupport.presentModes;
    }
}