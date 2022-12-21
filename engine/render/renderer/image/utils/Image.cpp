//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Image.h"

#include <utils/Logging.h>

VkImageView Image::CreateImageView(VkDevice device,
                                   VkImage image,
                                   VkImageViewType viewType,
                                   VkFormat format,
                                   VkComponentMapping componentMapping,
                                   VkImageSubresourceRange subResourceRange,
                                   const void* pNext,
                                   VkImageViewCreateFlags createFlags)
{
    VkImageView imageView;
    VkImageViewCreateInfo createInfo = CreateImageViewCreateInfo(image,
                                                                 viewType,
                                                                 format,
                                                                 componentMapping,
                                                                 subResourceRange,
                                                                 pNext,
                                                                 createFlags);

    CC_ASSERT(vkCreateImageView(device, &createInfo, nullptr, OUT & imageView) == VK_SUCCESS,
              "Failed to create texture image view!");

    return imageView;
}

VkImageViewCreateInfo Image::CreateImageViewCreateInfo(VkImage image,
                                                       VkImageViewType viewType,
                                                       VkFormat format,
                                                       VkComponentMapping componentMapping,
                                                       VkImageSubresourceRange subResourceRange,
                                                       const void* pNext,
                                                       VkImageViewCreateFlags createFlags)
{
    return {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            pNext,
            createFlags,
            image,
            viewType,
            format,
            componentMapping,
            subResourceRange};
}

VkImageCreateInfo Image::CreateImageCreateInfo(VkImageType type,
                                               VkFormat format,
                                               uint32_t imageWidth,
                                               uint32_t imageHeight,
                                               uint32_t imageDepth,
                                               uint32_t mipLevels,
                                               uint32_t arrayLevels,
                                               VkSampleCountFlagBits sampleFlags,
                                               VkImageTiling tiling,
                                               VkImageUsageFlags usage,
                                               VkSharingMode sharingMode,
                                               uint32_t queueFamilyCount,
                                               const uint32_t* pQueueFamilyIndices,
                                               VkImageLayout initialLayout,
                                               VkImageCreateFlags flags,
                                               const void* pNext)
{
    return {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            pNext,
            flags,
            type,
            format,
            {imageWidth, imageHeight, imageDepth},
            mipLevels,
            arrayLevels,
            sampleFlags,
            tiling,
            usage,
            sharingMode,
            queueFamilyCount,
            pQueueFamilyIndices,
            initialLayout};
}
