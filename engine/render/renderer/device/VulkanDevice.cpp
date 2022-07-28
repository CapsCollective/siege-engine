//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "VulkanDevice.h"

namespace Siege
{

// TODO: Finish cleaning this up.

VulkanDevice* VulkanDevice::vulkanDeviceInstance = nullptr;

VulkanDevice::VulkanDevice(Window* window)
{
    auto surfaceExtensions = window->GetRequiredExtensions();

    instance = Vulkan::VulkanInstance(Window::CreateWindowSurface,
                                      surfaceExtensions.Data(),
                                      surfaceExtensions.Size());

    physicalDevice = Vulkan::PhysicalDevice(instance);
    logicalDevice = Vulkan::LogicalDevice(physicalDevice, instance.Surface());

    SetVulkanDeviceInstance(this);
}

VulkanDevice::VulkanDevice()
{
    SetVulkanDeviceInstance(this);
}

VulkanDevice::VulkanDevice(VulkanDevice&& other)
{
    Move(other);
}

VulkanDevice& VulkanDevice::operator=(VulkanDevice&& other)
{
    Move(other);

    return *this;
}

void VulkanDevice::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    VkBufferCopy copyRegion {};
    copyRegion.srcOffset = 0; // Optional
    copyRegion.dstOffset = 0; // Optional
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    EndSingleTimeCommands(commandBuffer);
}

void VulkanDevice::CopyBufferToImage(VkBuffer buffer,
                                     VkImage image,
                                     uint32_t width,
                                     uint32_t height,
                                     uint32_t layerCount)
{
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    VkBufferImageCopy region {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = layerCount;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {width, height, 1};

    vkCmdCopyBufferToImage(commandBuffer,
                           buffer,
                           image,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                           1,
                           &region);
    EndSingleTimeCommands(commandBuffer);
}

void VulkanDevice::CreateImageWithInfo(const VkImageCreateInfo& imageInfo,
                                       VkMemoryPropertyFlags targetProperties,
                                       VkImage& image,
                                       VkDeviceMemory& imageMemory)
{
    CC_ASSERT(vkCreateImage(logicalDevice.Device(), &imageInfo, nullptr, OUT & image) == VK_SUCCESS,
              "Failed to create FrameImages!")

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(logicalDevice.Device(), image, OUT & memRequirements);

    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, targetProperties);

    CC_ASSERT(vkAllocateMemory(logicalDevice.Device(), &allocInfo, nullptr, OUT & imageMemory) ==
                  VK_SUCCESS,
              "Failed to allocate image memory!")

    CC_ASSERT(vkBindImageMemory(logicalDevice.Device(), OUT image, imageMemory, 0) == VK_SUCCESS,
              "Failed to bind image memory!")
}

void VulkanDevice::Move(VulkanDevice& other)
{
    instance = std::move(other.instance);
    physicalDevice = std::move(other.physicalDevice);
    logicalDevice = std::move(other.logicalDevice);

    SetVulkanDeviceInstance(this);
}
} // namespace Siege
