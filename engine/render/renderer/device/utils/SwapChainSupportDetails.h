//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#pragma once

#include <vector>

#include "../../Core.h"

// TODO: Move this out of device utils (since it's being used by the swapchain)

namespace Siege::SwapChainSupportDetails
{
/**
 * A struct for storing swapchain support information.
 * This struct stores surface capabilites, surface image formats, and present modes.
 **/
struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities {};

    // An array of available surface formats
    Mat::Array<VkSurfaceFormatKHR> formats {};

    // A array of present modes
    Mat::Array<VkPresentModeKHR> presentModes {};

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
} // namespace Siege::SwapChainSupportDetails
