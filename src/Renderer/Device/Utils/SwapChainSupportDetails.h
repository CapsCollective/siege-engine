#pragma once

#include "../../Core.h"
#include <vector>

namespace SnekVk
{
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities {};

        VkSurfaceFormatKHR formats[10] {};
        size_t availableFormatCount {0};
        VkPresentModeKHR presentModes[10] {};
        size_t availablePresentModeCount {0};
        
        bool hasFormats = false;
        bool hasPresentModes = false;
    };

    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR& surface);
}