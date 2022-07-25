//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "SwapChainSupportDetails.h"

#include "../../platform/vulkan/utils/Device.h"

namespace Siege::SwapChainSupportDetails
{
SwapChainSupportDetails QuerySupport(VkPhysicalDevice device, VkSurfaceKHR& surface)
{
    using Utils::MHArray;

    SwapChainSupportDetails details;
    // Populate the surface capabilities value
    details.capabilities = Vulkan::Device::Physical::GetSurfaceCapabilities(device, surface);

    uint32_t formatCount;
    Vulkan::Device::Physical::GetSurfaceFormatCount(device, surface, formatCount);

    if (Vulkan::Device::Physical::HasFormats(device, surface))
    {
        // Populate our format array
        details.formats = MHArray<VkSurfaceFormatKHR>(static_cast<size_t>(formatCount));
        Vulkan::Device::Physical::GetSurfaceFormats(device,
                                                    surface,
                                                    formatCount,
                                                    details.formats.Data());

        details.hasFormats = true;
    }

    uint32_t presentModeCount = Vulkan::Device::Physical::GetPresentModeCount(device, surface);

    if (Vulkan::Device::Physical::HasPresentModes(device, surface))
    {
        // populate our presentModes array
        details.presentModes = MHArray<VkPresentModeKHR>(static_cast<uint32_t>(presentModeCount));
        Vulkan::Device::Physical::GetPresentModes(device,
                                                  surface,
                                                  presentModeCount,
                                                  details.presentModes.Data());

        details.hasPresentModes = true;
    }
    return details;
}
} // namespace Siege::SwapChainSupportDetails
