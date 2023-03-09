//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Swapchain.h"

#include <utils/Logging.h>

#include <unordered_set>

#include "../Context.h"
#include "Device.h"

namespace Siege::Vulkan::Utils
{
SwapchainBuilder& SwapchainBuilder::WithMinImageCount(uint32_t count)
{
    minImageCount = count;
    return *this;
}
SwapchainBuilder& SwapchainBuilder::WithSurfaceFormat(const VkSurfaceFormatKHR& surfaceFormat)
{
    return WithImageFormat(surfaceFormat.format).WithColourSpace(surfaceFormat.colorSpace);
}

SwapchainBuilder& SwapchainBuilder::WithSurface(VkSurfaceKHR imageSurface)
{
    surface = imageSurface;
    return *this;
}

SwapchainBuilder& SwapchainBuilder::WithImageFormat(VkFormat format)
{
    imageFormat = format;
    return *this;
}

SwapchainBuilder& SwapchainBuilder::WithColourSpace(VkColorSpaceKHR colourSpace)
{
    imageColorSpace = colourSpace;
    return *this;
}

SwapchainBuilder& SwapchainBuilder::WithImageExtent(const VkExtent2D& extent)
{
    imageExtent = extent;
    return *this;
}

SwapchainBuilder& SwapchainBuilder::WithImageArrayLayers(uint32_t layers)
{
    imageArrayLayers = layers;
    return *this;
}

SwapchainBuilder& SwapchainBuilder::WithImageUsage(VkImageUsageFlags usageFlags)
{
    imageUsage = usageFlags;
    return *this;
}

SwapchainBuilder& SwapchainBuilder::WithQueueFamilyIndices(std::initializer_list<uint32_t> list)
{
    std::unordered_set<uint32_t> uniqueIndices;

    for (auto& family : list)
    {
        uniqueIndices.emplace(family);
    }

    bool queuesAreEqual = uniqueIndices.size() == 1;

    imageSharingMode = static_cast<VkSharingMode>((!queuesAreEqual * VK_SHARING_MODE_CONCURRENT) +
                                                  (queuesAreEqual * VK_SHARING_MODE_EXCLUSIVE));

    queueFamilyIndexCount = !queuesAreEqual * uniqueIndices.size();

    pQueueFamilyIndices =
        !queuesAreEqual ? std::vector<uint32_t>(uniqueIndices.begin(), uniqueIndices.end()).data() :
                          nullptr;

    return *this;
}

SwapchainBuilder& SwapchainBuilder::WithPreTransform(const VkSurfaceTransformFlagBitsKHR transform)
{
    preTransform = transform;
    return *this;
}

SwapchainBuilder& SwapchainBuilder::WithCompositeAlpha(const VkCompositeAlphaFlagBitsKHR alpha)
{
    compositeAlpha = alpha;
    return *this;
}

SwapchainBuilder& SwapchainBuilder::WithPresentMode(const VkPresentModeKHR mode)
{
    presentMode = mode;
    return *this;
}

SwapchainBuilder& SwapchainBuilder::IsClipped(const VkBool32 isClipped)
{
    clipped = isClipped;
    return *this;
}

SwapchainBuilder& SwapchainBuilder::FromOldSwapchain(const VkSwapchainKHR swapchain)
{
    oldSwapchain = swapchain;
    return *this;
}

VkSwapchainKHR SwapchainBuilder::Build()
{
    VkSwapchainKHR swapchain;
    auto device = Context::GetVkLogicalDevice();
    // Now we populate the base swapchain creation struct
    VkSwapchainCreateInfoKHR createInfo {};

    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.pNext = pNext;
    createInfo.minImageCount = minImageCount;
    createInfo.imageFormat = imageFormat;
    createInfo.imageColorSpace = imageColorSpace;
    createInfo.imageExtent = imageExtent;
    createInfo.imageArrayLayers = imageArrayLayers;
    createInfo.imageUsage = imageUsage;
    createInfo.imageSharingMode = imageSharingMode;
    createInfo.queueFamilyIndexCount = queueFamilyIndexCount;
    createInfo.pQueueFamilyIndices = pQueueFamilyIndices;
    createInfo.preTransform = preTransform;
    createInfo.compositeAlpha = compositeAlpha;
    createInfo.presentMode = presentMode;
    createInfo.clipped = clipped;
    createInfo.oldSwapchain = oldSwapchain;

    CC_ASSERT(vkCreateSwapchainKHR(device, &createInfo, nullptr, OUT & swapchain) == VK_SUCCESS,
              "Failed to create Swapchain!")

    return swapchain;
}

VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const MHArray<VkSurfaceFormatKHR>& formats)
{
    // Ideally, we want to support colors in 4 dimensional vectors (R, G, B, A) in SRGB colour
    // space.
    for (auto it = formats.CreateFIterator(); it; ++it)
    {
        auto format = *it;
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
            format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return format;
    }

    // If we can't find what we want then we use whatever is available on the GPU.
    return formats[0];
}

VkPresentModeKHR ChoosePresentMode(const MHArray<VkPresentModeKHR>& presentModes)
{
    for (auto it = presentModes.CreateFIterator(); it; ++it)
    {
        auto presentMode = *it;
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            CC_LOG_INFO("Present Mode: Mailbox")
            return presentMode;
        }
    }

    // If triple buffering is not available then use v-sync
    CC_LOG_INFO("Present Mode: V-Sync")
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities,
                            const VkExtent2D& windowExtent)
{
    // We want to make sure that the extents provided are within a reasonable range.
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        return capabilities.currentExtent;
    else
    {
        // Otherwise get the smaller extent between the window and the swapchain image
        // extents and return that.
        VkExtent2D actualExtent = windowExtent;
        actualExtent.width =
            std::max(capabilities.minImageExtent.width,
                     std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height =
            std::max(capabilities.minImageExtent.height,
                     std::min(capabilities.maxImageExtent.height, actualExtent.height));
        return actualExtent;
    }
}

} // namespace Siege::Vulkan::Utils