#pragma once

#include "../Renderer.h"
#include <vector>

namespace SnekVk
{
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities {};
        VkSurfaceFormatKHR formats[10] {};
        VkPresentModeKHR presentModes[10] {};
        bool hasFormats = false;
        bool hasPresentModes = false;
    };

    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR& surface);
}