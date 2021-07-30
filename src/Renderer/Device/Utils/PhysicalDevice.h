#pragma once

#include "../../Renderer.h"
#include "QueueFamilyIndices.h"
#include "SwapChainSupportDetails.h"

namespace SnekVk::PhysicalDevice
{
    bool CheckExtensionSupport(
        VkPhysicalDevice device, 
        const char* const* deviceExtensions, 
        size_t deviceExtensionCount
    ); 

    bool IsSuitable(VkPhysicalDevice device, VkSurfaceKHR& surface, const char *const * deviceExtensions, size_t deviceExtensionCount); 
}