//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_BUFFER_H
#define SIEGE_ENGINE_BUFFER_H

#include <volk/volk.h>

namespace Siege::Vulkan::Buffer
{
VkBufferCopy CopyRegion(const uint64_t& size, const uint64_t& srcOffset, const uint64_t& dstOffset);
void CopyWholeSingleBuffer(const uint64_t& size,
                           VkCommandBuffer commandBuffer,
                           VkBuffer src,
                           VkBuffer dst);

VkImageSubresourceLayers ImageSubresources(VkImageAspectFlags aspectMask,
                                           uint32_t mipLevel,
                                           uint32_t baseArrayLayer,
                                           uint32_t layerCount);

VkBufferImageCopy ImageCopyRegion(VkDeviceSize bufferOffset,
                                  uint32_t bufferRowLength,
                                  uint32_t bufferImageHeight,
                                  const VkImageSubresourceLayers& subResources,
                                  VkOffset3D offset,
                                  VkExtent3D extent);

void CopySingleBufferToImage(VkDeviceSize bufferOffset,
                             uint32_t bufferRowLength,
                             uint32_t bufferImageHeight,
                             const VkImageSubresourceLayers& subResources,
                             VkOffset3D offset,
                             VkExtent3D extent);

void CopyBufferToImage(VkCommandBuffer commandBuffer,
                       VkBuffer buffer,
                       VkImage image,
                       uint32_t amount,
                       VkImageLayout imageLayout,
                       const VkBufferImageCopy& imageCopy);

void CopyWholeSingleBufferToImage(VkCommandBuffer commandBuffer,
                                  VkBuffer buffer,
                                  const VkBufferImageCopy& copyRegion,
                                  VkImage image);

VkMemoryAllocateInfo AllocateInfo(VkDeviceSize allocationSize,
                                  uint32_t memoryTypeIndex,
                                  const void* pNext = nullptr);

void AllocateMemory(VkDeviceSize allocationSize,
                    uint32_t memoryTypeIndex,
                    const void* pNext = nullptr);

struct ImageSubResourcesBuilder
{
    ImageSubResourcesBuilder& WithAspectMask(const VkImageAspectFlags& mask);
    ImageSubResourcesBuilder& WithMipLevel(const uint32_t& level);
    ImageSubResourcesBuilder& WithBaseArrayLayer(const uint32_t& layer);
    ImageSubResourcesBuilder& WithLayerCount(const uint32_t& count);

    VkImageSubresourceLayers Build();

    VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    uint32_t mipLevel = 0;
    uint32_t baseArrayLayer = 0;
    uint32_t layerCount = 0;
};

struct ImageCopyBuilder
{
    ImageCopyBuilder& WithBufferOffset(const VkDeviceSize& offset);
    ImageCopyBuilder& WithRowLength(const uint32_t& rowLength);
    ImageCopyBuilder& WithImageHeight(const uint32_t& imageHeight);
    ImageCopyBuilder& WithSubResources(const VkImageSubresourceLayers& subResources);
    ImageCopyBuilder& WithOffset3D(const VkOffset3D& offset);
    ImageCopyBuilder& WithExtent3D(const VkExtent3D& extent);
    VkBufferImageCopy Build();

    VkDeviceSize bufferOffset {0};
    uint32_t bufferRowLength {0};
    uint32_t bufferImageHeight {0};
    VkImageSubresourceLayers imageSubResources {};
    VkOffset3D offset3D {0, 0, 0};
    VkExtent3D extent3D {0, 0, 0};
};
} // namespace Siege::Vulkan::Buffer

#endif // SIEGE_ENGINE_BUFFER_H
