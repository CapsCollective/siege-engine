#pragma once

#include "../../Core.h"
#include <vector>

namespace SnekVk
{
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities {};

        VkSurfaceFormatKHR* formats {};
        size_t availableFormatCount {0};
        VkPresentModeKHR* presentModes {};
        size_t availablePresentModeCount {0};
        
        bool hasFormats = false;
        bool hasPresentModes = false;
    };

    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR& surface);

    void DestroySwapChainSupportDetails(SwapChainSupportDetails& details);
}