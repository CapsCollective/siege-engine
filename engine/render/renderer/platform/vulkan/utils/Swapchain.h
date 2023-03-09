//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_UTILS_SWAPCHAIN_H
#define SIEGE_ENGINE_VULKAN_UTILS_SWAPCHAIN_H

#include <utils/collections/HeapArray.h>
#include <volk/volk.h>

namespace Siege::Vulkan::Utils
{

struct SwapchainBuilder
{
    SwapchainBuilder& WithMinImageCount(uint32_t count);
    SwapchainBuilder& WithSurfaceFormat(const VkSurfaceFormatKHR& surfaceFormat);
    SwapchainBuilder& WithSurface(VkSurfaceKHR imageSurface);
    SwapchainBuilder& WithImageFormat(VkFormat format);
    SwapchainBuilder& WithColourSpace(VkColorSpaceKHR colourSpace);
    SwapchainBuilder& WithImageExtent(const VkExtent2D& extent);
    SwapchainBuilder& WithImageArrayLayers(uint32_t layers);
    SwapchainBuilder& WithImageUsage(VkImageUsageFlags usageFlags);
    SwapchainBuilder& WithQueueFamilyIndices(std::initializer_list<uint32_t> list);
    SwapchainBuilder& WithPreTransform(const VkSurfaceTransformFlagBitsKHR transform);
    SwapchainBuilder& WithCompositeAlpha(const VkCompositeAlphaFlagBitsKHR alpha);
    SwapchainBuilder& WithPresentMode(const VkPresentModeKHR mode);
    SwapchainBuilder& IsClipped(const VkBool32 isClipped);
    SwapchainBuilder& FromOldSwapchain(const VkSwapchainKHR swapchain);
    VkSwapchainKHR Build();

    const void* pNext {nullptr};
    VkSwapchainCreateFlagsKHR flags {0};
    VkSurfaceKHR surface {VK_NULL_HANDLE};
    uint32_t minImageCount {0};
    VkFormat imageFormat {VK_FORMAT_UNDEFINED};
    VkColorSpaceKHR imageColorSpace {VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    VkExtent2D imageExtent {0, 0};
    uint32_t imageArrayLayers {1};
    VkImageUsageFlags imageUsage {VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT};
    VkSharingMode imageSharingMode {VK_SHARING_MODE_EXCLUSIVE};
    uint32_t queueFamilyIndexCount {0};
    const uint32_t* pQueueFamilyIndices {nullptr};
    VkSurfaceTransformFlagBitsKHR preTransform {VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR};
    VkCompositeAlphaFlagBitsKHR compositeAlpha {VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR};
    VkPresentModeKHR presentMode {VK_PRESENT_MODE_IMMEDIATE_KHR};
    VkBool32 clipped {VK_FALSE};
    VkSwapchainKHR oldSwapchain {VK_NULL_HANDLE};
};

VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const MHArray<VkSurfaceFormatKHR>& formats);
VkPresentModeKHR ChoosePresentMode(const MHArray<VkPresentModeKHR>& presentModes);
VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities,
                            const VkExtent2D& windowExtent);
} // namespace Siege::Vulkan::Utils

#endif // SIEGE_ENGINE_VULKAN_UTILS_SWAPCHAIN_H
