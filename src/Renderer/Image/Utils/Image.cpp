#include "Image.h"

VkImageView Image::CreateImageView(VkDevice device,
                                   VkImage image,
                                   VkImageViewType viewType,
                                   VkFormat format,
                                   VkComponentMapping componentMapping,
                                   VkImageSubresourceRange subResourceRange,
                                   const void *pNext,
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

    SNEK_ASSERT(vkCreateImageView(device, &createInfo, nullptr, OUT &imageView) == VK_SUCCESS,
                "Failed to create texture image view!");

    return imageView;
}

VkImageViewCreateInfo Image::CreateImageViewCreateInfo(VkImage image,
                                                       VkImageViewType viewType,
                                                       VkFormat format,
                                                       VkComponentMapping componentMapping,
                                                       VkImageSubresourceRange subResourceRange,
                                                       const void *pNext,
                                                       VkImageViewCreateFlags
                                                       createFlags)
{
    return
    {
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            pNext,
            createFlags,
            image,
            viewType,
            format,
            componentMapping,
            subResourceRange
    };
}

VkImageCreateInfo Image::CreateImageCreateInfo(VkImageType type,
                                               VkFormat format,
                                               u32 imageWidth,
                                               u32 imageHeight,
                                               u32 imageDepth,
                                               u32 mipLevels,
                                               u32 arrayLevels,
                                               VkSampleCountFlagBits sampleFlags,
                                               VkImageTiling tiling,
                                               VkImageUsageFlags usage,
                                               VkSharingMode sharingMode,
                                               u32 queueFamilyCount,
                                               const uint32_t *pQueueFamilyIndices,
                                               VkImageLayout initialLayout,
                                               VkImageCreateFlags flags,
                                               const void *pNext)
{
    return
    {
            VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            pNext,
            flags,
            type,
            format,
            {
                imageWidth,
                imageHeight,
                imageDepth
            },
            mipLevels,
            arrayLevels,
            sampleFlags,
            tiling,
            usage,
            sharingMode,
            queueFamilyCount,
            pQueueFamilyIndices,
            initialLayout
    };
}
