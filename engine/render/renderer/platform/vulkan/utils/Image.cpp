//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Image.h"

#include "../Context.h"
#include "Device.h"

namespace Siege::Vulkan::Utils
{
VkImageView Image::CreateImageView(VkImage image,
                                   VkImageViewType viewType,
                                   VkFormat format,
                                   VkComponentMapping componentMapping,
                                   VkImageSubresourceRange subResourceRange,
                                   const void* pNext,
                                   VkImageViewCreateFlags createFlags)
{
    auto device = Context::GetVkLogicalDevice();

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

void Image::AllocateImageMemory(VkImage& image, VkDeviceMemory& memory)
{
    auto device = Context::GetVkLogicalDevice();
    auto physicalDevice = Context::GetPhysicalDevice()->GetDevice();

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, OUT & memRequirements);

    VkMemoryAllocateInfo allocInfo {
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        nullptr,
        memRequirements.size,
        Device::Physical::FindMemoryType(physicalDevice,
                                         memRequirements.memoryTypeBits,
                                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)};

    CC_ASSERT(vkAllocateMemory(device, &allocInfo, nullptr, OUT & memory) == VK_SUCCESS,
              "Failed to allocate image memory!")
}

VkImage Image::CreateImage(VkImageType type,
                           VkFormat format,
                           VkExtent3D extent,
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
    auto device = Context::GetVkLogicalDevice();

    VkImage image;
    VkImageCreateInfo createInfo = Utils::Image::CreateImageCreateInfo(type,
                                                                       format,
                                                                       extent,
                                                                       mipLevels,
                                                                       arrayLevels,
                                                                       sampleFlags,
                                                                       tiling,
                                                                       usage,
                                                                       sharingMode,
                                                                       queueFamilyCount,
                                                                       pQueueFamilyIndices,
                                                                       initialLayout,
                                                                       flags,
                                                                       pNext);

    CC_ASSERT(vkCreateImage(device, &createInfo, nullptr, OUT & image) == VK_SUCCESS,
              "Failed to create FrameImages!")

    return image;
}

VkImageCreateInfo Image::CreateImageCreateInfo(VkImageType type,
                                               VkFormat format,
                                               VkExtent3D extent,
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
            extent,
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
} // namespace Siege::Vulkan::Utils