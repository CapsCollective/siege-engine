//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "VulkanDevice.h"

#include <utils/Logging.h>

#include "../platform/vulkan/utils/CommandPool.h"
#include "../platform/vulkan/utils/Device.h"
#include "../platform/vulkan/utils/Instance.h"

namespace Siege
{

VulkanDevice* VulkanDevice::vulkanDeviceInstance = nullptr;

VulkanDevice::VulkanDevice()
{
    context.Init(Siege::Window::GetRequiredExtensions, Siege::Window::CreateWindowSurface);

    SetVulkanDeviceInstance(this);
}

VulkanDevice::~VulkanDevice() {}

size_t VulkanDevice::GetDeviceAlignment()
{
    auto properties = context.GetPhysicalDevice().GetProperties();
    return properties.limits.minUniformBufferOffsetAlignment;
}

VkFormat VulkanDevice::FindSupportedFormat(const VkFormat* candidates,
                                           size_t formatCount,
                                           VkImageTiling tiling,
                                           VkFormatFeatureFlags features)
{
    auto vkPhysicalDevice = context.GetPhysicalDevice().GetDevice();

    for (size_t i = 0; i < formatCount; i++)
    {
        VkFormat format = candidates[i];

        VkFormatProperties props = Vulkan::Device::Physical::GetProperties(vkPhysicalDevice, format);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
        {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
                 (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }
    CC_ASSERT(false, "Failed to find a supported format!")
}

SwapChainSupportDetails::SwapChainSupportDetails VulkanDevice::GetSwapChainSupport()
{
    auto surface = Surface();
    auto vkPhysicalDevice = context.GetPhysicalDevice().GetDevice();

    return SwapChainSupportDetails::QuerySupport(vkPhysicalDevice, surface);
}

uint32_t VulkanDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags newProperties)
{
    auto vkPhysicalDevice = context.GetPhysicalDevice().GetDevice();

    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & newProperties) == newProperties)
        {
            return i;
        }
    }

    CC_ASSERT(false, "Failed to find suitable memory type!")
}

QueueFamilyIndices::QueueFamilyIndices VulkanDevice::FindPhysicalQueueFamilies()
{
    auto surface = Surface();
    auto vkPhysicalDevice = context.GetPhysicalDevice().GetDevice();

    return QueueFamilyIndices::FindQueueFamilies(vkPhysicalDevice, surface);
}

VkCommandBuffer VulkanDevice::BeginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = GetCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(context.GetLogicalDevice().GetDevice(), &allocInfo, OUT & commandBuffer);

    VkCommandBufferBeginInfo beginInfo {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(OUT commandBuffer, &beginInfo);
    return commandBuffer;
}

void VulkanDevice::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    auto graphicsQueue = context.GetLogicalDevice().GetGraphicsQueue();

    VkSubmitInfo submitInfo {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    vkFreeCommandBuffers(context.GetLogicalDevice().GetDevice(), GetCommandPool(), 1, OUT & commandBuffer);
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
    CC_ASSERT(vkCreateImage(context.GetLogicalDevice().GetDevice(), &imageInfo, nullptr, OUT & image) == VK_SUCCESS,
              "Failed to create FrameImages!")

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(context.GetLogicalDevice().GetDevice(), image, OUT & memRequirements);

    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, targetProperties);

    CC_ASSERT(vkAllocateMemory(context.GetLogicalDevice().GetDevice(), &allocInfo, nullptr, OUT & imageMemory) == VK_SUCCESS,
              "Failed to allocate image memory!")

    CC_ASSERT(vkBindImageMemory(context.GetLogicalDevice().GetDevice(), OUT image, imageMemory, 0) == VK_SUCCESS,
              "Failed to bind image memory!")
}
} // namespace Siege
