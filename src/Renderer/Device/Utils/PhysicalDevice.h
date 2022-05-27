#pragma once

#include "../../Core.h"
#include "QueueFamilyIndices.h"
#include "SwapChainSupportDetails.h"

namespace SnekVk::PhysicalDevice
{
    /**
     * Checks a physical device's compaitibility with Vulkan. This 
     * is evaluated by checking the extensions the device supports. The 
     * extensions required by the device are defined by the user.
     * 
     * @param device the physical device being evaluated.
     * @param deviceExtensions the extensions that the device is required to support.
     * @param deviceExtensionCount the size of the deviceExtensions array. 
     * @returns a boolean specifying if the device supports the required extensions. 
     **/
    bool CheckExtensionSupport(
        VkPhysicalDevice device, 
        const char* const* deviceExtensions, 
        size_t deviceExtensionCount
    ); 

    /**
     * Evaluates if a device is suitable for usage by the renderer.  
     * 
     * @param device the physical device being evaulated.
     * @param surface the window surface to be rendered to.
     * @param deviceExtensions the extensions which the device must support.
     * @param deviceExtensionCount the size of the deviceExtensions array.
     * @returns a boolean specifying if the device is suitable for usage.
     **/
    bool IsSuitable(VkPhysicalDevice device, VkSurfaceKHR& surface, const char *const * deviceExtensions, size_t deviceExtensionCount); 
}