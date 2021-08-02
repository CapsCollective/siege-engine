#pragma once

#include "../../Core.h"
#include <vector>

// TODO: Move this out of device utils (since it's being used by the swapchain)

namespace SnekVk::SwapChainSupportDetails
{
    /**
     * A struct for storing swapchain support information. 
     * This struct stores surface capabilites, surface image formats, and present modes.    
     **/
    struct SwapChainSupportDetails {
        
        VkSurfaceCapabilitiesKHR capabilities {};

        // An array of available surface formats
        Utils::Array<VkSurfaceFormatKHR> formats{};

        // A array of present modes
        Utils::Array<VkPresentModeKHR> presentModes{};
        
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
}
