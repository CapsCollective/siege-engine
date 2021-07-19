#pragma once

#include "../Renderer.h"
#include "QueueFamilyIndices.h"
#include "SwapChainSupportDetails.h"

namespace SnekVk 
{
    bool CheckDeviceExtensionSupport(
        VkPhysicalDevice device, 
        const char* const* deviceExtensions, 
        size_t deviceExtensionCount
    ); 

    bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR& surface, const char *const * deviceExtensions, size_t deviceExtensionCount); 
}