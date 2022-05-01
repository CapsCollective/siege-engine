#pragma once

#include "../../Core.h"
#include <vector>

namespace SnekVk::SwapChainSupportDetails
{
    /**
     * A struct for storing swapchain support information. 
     * This struct stores surface capabilites, surface image formats, and present modes.    
     **/
    struct SwapChainSupportDetails {
        
        VkSurfaceCapabilitiesKHR capabilities {};

        // An array of available surface formats
        VkSurfaceFormatKHR* formats {};
        size_t availableFormatCount {0};

        // A array of present modes
        VkPresentModeKHR* presentModes {};
        size_t availablePresentModeCount {0};
        
        bool hasFormats = false;
        bool hasPresentModes = false;
    };

    /**
     * Queries a physical device for compatibility with a window surface. This should allow
     * us to see whether this device can provide a swapchain that can render images correctly. 
     * 
     * @param device the physical device being evaluated
     * @param surface the window surface that images need to be rendered to.
     **/
    SwapChainSupportDetails QuerySupport(VkPhysicalDevice device, VkSurfaceKHR& surface);

    /**
     * Cleans up memory for the presentModes and formats arrays.  
     **/
    void DestroySwapChainSupportDetails(SwapChainSupportDetails& details);
}
