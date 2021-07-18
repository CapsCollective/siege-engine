#pragma once

#include "../../Renderer.h"

namespace SnekVK
{
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        
        VkSurfaceFormatKHR* formats;
        bool hasSurfaceFormat = false;
        
        VkPresentModeKHR* presentModes;
        bool hasPresentMode = false; 
    };

    void DestroySwapChainSupportDetails(SwapChainSupportDetails& swapChainSupport)
    {
        delete swapChainSupport.formats;
        delete swapChainSupport.presentModes;
    };

    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
}