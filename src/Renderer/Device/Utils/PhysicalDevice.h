#pragma once

#include "QueueFamilyIndices.h"
#include "SwapChainSupportDetails.h"

namespace SnekVK
{
    bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const char** deviceExtensions, size_t deviceExtensionCount);

    bool CheckDeviceExtensionSupport(VkPhysicalDevice device, const char** deviceExtensions, size_t deviceExtensionCount);
}