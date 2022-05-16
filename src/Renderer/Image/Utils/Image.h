#pragma once

#include "../../Core.h"

class Image
{
public:
    static VkImageView CreateImageView(VkDevice device,
                                       VkImage image,
                                       VkImageViewType viewType,
                                       VkFormat format,
                                       VkComponentMapping componentMapping,
                                       VkImageSubresourceRange subResourceRange,
                                       const void* pNext = nullptr,
                                       VkImageViewCreateFlags createFlags = 0);

    static VkImageCreateInfo CreateImageCreateInfo(VkImageType type,
                                                   VkFormat format,
                                                   u32 imageWidth,
                                                   u32 imageHeight,
                                                   u32 imageDepth,
                                                   u32 mipLevels,
                                                   u32 arrayLevels,
                                                   VkSampleCountFlagBits sampleFlags,
                                                   VkImageTiling tiling,
                                                   VkImageUsageFlags usage,
                                                   VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                                                   u32 queueFamilyCount = 0,
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

