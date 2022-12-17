//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "SwapChainSupportDetails.h"

namespace Siege::SwapChainSupportDetails
{
SwapChainSupportDetails QuerySupport(VkPhysicalDevice device, VkSurfaceKHR& surface)
{
    SwapChainSupportDetails details;
    // Populate the surface capabilities value
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, OUT & details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, OUT & formatCount, nullptr);

    if (formatCount != 0)
    {
        // Populate our format array
        details.formats = HeapArray<VkSurfaceFormatKHR>(static_cast<size_t>(formatCount));
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.Data());

        details.hasFormats = true;
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, OUT & presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        // populate our presentModes array
        details.presentModes = HeapArray<VkPresentModeKHR>(static_cast<uint32_t>(presentModeCount));
        vkGetPhysicalDeviceSurfacePresentModesKHR(device,
                                                  surface,
                                                  &presentModeCount,
                                                  details.presentModes.Data());

        details.hasPresentModes = true;
    }
    return details;
}
} // namespace Siege::SwapChainSupportDetails
