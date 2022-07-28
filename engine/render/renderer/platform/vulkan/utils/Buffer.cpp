//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Buffer.h"

namespace Siege::Vulkan::Buffer
{
VkBufferCopy CopyRegion(const uint64_t& size, const uint64_t& srcOffset, const uint64_t& dstOffset)
{
    return {srcOffset, dstOffset, size};
}

void CopyWholeSingleBuffer(const uint64_t& size,
                           VkCommandBuffer commandBuffer,
                           VkBuffer src,
                           VkBuffer dst)
{
    VkBufferCopy copyRegion = CopyRegion(size, 0, 0);
    vkCmdCopyBuffer(commandBuffer, src, dst, 1, &copyRegion);
}

VkBufferImageCopy ImageCopyRegion(VkDeviceSize bufferOffset,
                                  uint32_t bufferRowLength,
                                  uint32_t bufferImageHeight,
                                  const VkImageSubresourceLayers& subResources,
                                  VkOffset3D offset,
                                  VkExtent3D extent)
{
    return {bufferOffset, bufferRowLength, bufferImageHeight, subResources, offset, extent};
}

VkImageSubresourceLayers ImageSubresources(VkImageAspectFlags aspectMask,
                                           uint32_t mipLevel,
                                           uint32_t baseArrayLayer,
                                           uint32_t layerCount)
{
    return {aspectMask, mipLevel, baseArrayLayer, layerCount};
}

void CopyBufferToImage(VkCommandBuffer commandBuffer,
                       VkBuffer buffer,
                       VkImage image,
                       uint32_t amount,
                       VkImageLayout imageLayout,
                       const VkBufferImageCopy& imageCopy)
{
    vkCmdCopyBufferToImage(commandBuffer, buffer, image, imageLayout, amount, &imageCopy);
}

void CopyWholeSingleBufferToImage(VkCommandBuffer commandBuffer,
                                  VkBuffer buffer,
                                  const VkBufferImageCopy& copyRegion,
                                  VkImage image)
{
    CopyBufferToImage(commandBuffer,
                      buffer,
                      image,
                      1,
                      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                      copyRegion);
}

VkMemoryAllocateInfo AllocateInfo(VkDeviceSize allocationSize,
                                  uint32_t memoryTypeIndex,
                                  const void* pNext)
{
    return {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, pNext, allocationSize, memoryTypeIndex};
}

// SubResourceBuilder

ImageSubResourcesBuilder& ImageSubResourcesBuilder::WithAspectMask(const VkImageAspectFlags& mask)
{
    aspectMask = mask;
    return *this;
}

ImageSubResourcesBuilder& ImageSubResourcesBuilder::WithMipLevel(const uint32_t& level)
{
    mipLevel = level;
    return *this;
}

ImageSubResourcesBuilder& ImageSubResourcesBuilder::WithBaseArrayLayer(const uint32_t& layer)
{
    baseArrayLayer = layer;
    return *this;
}

ImageSubResourcesBuilder& ImageSubResourcesBuilder::WithLayerCount(const uint32_t& count)
{
    layerCount = count;
    return *this;
}

VkImageSubresourceLayers ImageSubResourcesBuilder::Build()
{
    return {aspectMask, mipLevel, baseArrayLayer, layerCount};
}

// ImageCopyBuilder

ImageCopyBuilder& ImageCopyBuilder::WithBufferOffset(const VkDeviceSize& offset)
{
    bufferOffset = offset;
    return *this;
}

ImageCopyBuilder& ImageCopyBuilder::WithRowLength(const uint32_t& rowLength)
{
    bufferRowLength = rowLength;
    return *this;
}

ImageCopyBuilder& ImageCopyBuilder::WithImageHeight(const uint32_t& imageHeight)
{
    bufferImageHeight = imageHeight;
    return *this;
}

ImageCopyBuilder& ImageCopyBuilder::WithSubResources(const VkImageSubresourceLayers& subResources)
{
    imageSubResources = subResources;
    return *this;
}

ImageCopyBuilder& ImageCopyBuilder::WithOffset3D(const VkOffset3D& offset)
{
    offset3D = offset;
    return *this;
}

ImageCopyBuilder& ImageCopyBuilder::WithExtent3D(const VkExtent3D& extent)
{
    extent3D = extent;
    return *this;
}

VkBufferImageCopy ImageCopyBuilder::Build()
{
    return {bufferOffset,
            bufferRowLength,
            bufferImageHeight,
            imageSubResources,
            offset3D,
            extent3D};
}
} // namespace Siege::Vulkan::Buffer