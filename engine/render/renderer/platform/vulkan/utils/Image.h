//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_UTILS_IMAGE_H
#define SIEGE_ENGINE_VULKAN_UTILS_IMAGE_H

#include <volk/volk.h>

namespace Siege::Vulkan::Utils
{
class Image
{
public:

    struct SubResourceRange
    {
        SubResourceRange WithAspect(const VkImageAspectFlagBits& aspectFlag);
        SubResourceRange WithMipLevel(const uint32_t& newMipLevel);
        SubResourceRange WithLevelCount(const uint32_t& newLevelCount);
        SubResourceRange WithArrayLayer(const uint32_t& newArrayLayer);
        SubResourceRange WithLayerCount(const uint32_t& newLayerCount);

        VkImageSubresourceRange Build();

        VkImageAspectFlagBits aspect {VK_IMAGE_ASPECT_NONE};
        uint32_t mipLevel {0};
        uint32_t levelCount {0};
        uint32_t arrayLayer {0};
        uint32_t layerCount {0};
    };

    struct Barrier
    {
        Barrier& FromAccessMask(const VkAccessFlags& mask);
        Barrier& ToAccessMask(const VkAccessFlags& mask);
        Barrier& FromLayout(const VkImageLayout& layout);
        Barrier& ToLayout(const VkImageLayout& layout);
        Barrier& FromQueueFamilyIndex(const uint32_t& index);
        Barrier& ToQueueFamilyIndex(const uint32_t& index);
        Barrier& WithImage(const VkImage& index);
        Barrier& WithSubResourceRange(const VkImageSubresourceRange& index);

        VkImageMemoryBarrier Build();

        VkAccessFlags srcAccessMask {VK_ACCESS_NONE};
        VkAccessFlags dstAccessMask {VK_ACCESS_NONE};
        VkImageLayout oldLayout {VK_IMAGE_LAYOUT_UNDEFINED};
        VkImageLayout newLayout {VK_IMAGE_LAYOUT_UNDEFINED};
        uint32_t srcQueueFamilyIndex {0};
        uint32_t dstQueueFamilyIndex {0};
        VkImage image {VK_NULL_HANDLE};
        VkImageSubresourceRange subresourceRange {};
    };

    static VkImageView CreateImageView(VkImage image,
                                       VkImageViewType viewType,
                                       VkFormat format,
                                       VkComponentMapping componentMapping,
                                       VkImageSubresourceRange subResourceRange,
                                       const void* pNext = nullptr,
                                       VkImageViewCreateFlags createFlags = 0);

    static VkImageCreateInfo CreateImageCreateInfo(
        VkImageType type,
        VkFormat format,
        VkExtent3D extent,
        uint32_t mipLevels,
        uint32_t arrayLevels,
        VkSampleCountFlagBits sampleFlags,
        VkImageTiling tiling,
        VkImageUsageFlags usage,
        VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        uint32_t queueFamilyCount = 0,
        const uint32_t* pQueueFamilyIndices = VK_NULL_HANDLE,
        VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        VkImageCreateFlags flags = 0,
        const void* pNext = nullptr);

    static void AllocateImageMemory(VkImage& image, VkDeviceMemory& memory);

    static VkImage CreateImage(VkImageType type,
                               VkFormat format,
                               VkExtent3D extent,
                               uint32_t mipLevels,
                               uint32_t arrayLevels,
                               VkSampleCountFlagBits sampleFlags,
                               VkImageTiling tiling,
                               VkImageUsageFlags usage,
                               VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                               uint32_t queueFamilyCount = 0,
                               const uint32_t* pQueueFamilyIndices = 0,
                               VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                               VkImageCreateFlags flags = 0,
                               const void* pNext = nullptr);

private:

    static VkImageViewCreateInfo CreateImageViewCreateInfo(VkImage image,
                                                           VkImageViewType viewType,
                                                           VkFormat format,
                                                           VkComponentMapping componentMapping,
                                                           VkImageSubresourceRange subResourceRange,
                                                           const void* pNext = nullptr,
                                                           VkImageViewCreateFlags createFlags = 0);
};
} // namespace Siege::Vulkan::Utils

#endif // SIEGE_ENGINE_VULKAN_UTILS_IMAGE_H
