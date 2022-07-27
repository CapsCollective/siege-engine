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
    CreateInstance(window);

    PickPhysicalDevice();
    CreateLogicalDevice();

    SetVulkanDeviceInstance(this);
}

VulkanDevice::VulkanDevice()
{
    SetVulkanDeviceInstance(this);
}

void VulkanDevice::SetWindow(Window* newWindow)
{
    CC_ASSERT(volkInitialize() == VK_SUCCESS, "Unable to initialise Volk!")

    CC_ASSERT(newWindow != nullptr, "Must provide a valid pointer to a window!")

    CreateInstance(newWindow);

    PickPhysicalDevice();
    CreateLogicalDevice();

    SetVulkanDeviceInstance(this);
}

VulkanDevice::~VulkanDevice()
{}

void VulkanDevice::CreateInstance(Window* window)
{
    auto surfaceExtensions = window->GetRequiredExtensions();
    instance = Vulkan::VulkanInstance(Window::CreateWindowSurface,
                                      surfaceExtensions.Data(),
                                      surfaceExtensions.Size());
}

void VulkanDevice::PickPhysicalDevice()
{
    uint32_t extensions = deviceExtensions.size();

    const char* rawExtensions[extensions];
    GET_RAW(deviceExtensions.data(), rawExtensions, extensions)

    physicalDevice = Vulkan::PhysicalDevice(instance.VkInstance(), instance.Surface(), rawExtensions, extensions);
}

void VulkanDevice::CreateLogicalDevice()
{
    const size_t extensionSize = deviceExtensions.size();
    const size_t layerSize = VALIDATION_LAYERS_COUNT;

    const char* rawExtensions[extensionSize];
    const char* rawLayers[layerSize];

    GET_RAW(deviceExtensions, rawExtensions, extensionSize)
    GET_RAW(validationLayers, rawLayers, layerSize)

    logicalDevice = Vulkan::LogicalDevice(physicalDevice,
                                          instance.Surface(),
                                          rawExtensions,
                                          extensionSize,
                                          rawLayers,
                                          layerSize);
}

VkFormat VulkanDevice::FindSupportedFormat(const VkFormat* candidates,
                                           size_t formatCount,
                                           VkImageTiling tiling,
                                           VkFormatFeatureFlags features)
{
    auto vkPhysicalDevice = physicalDevice.GetDevice();
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

uint32_t VulkanDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags newProperties)
{
    auto vkPhysicalDevice = physicalDevice.GetDevice();

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

VkCommandBuffer VulkanDevice::BeginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = logicalDevice.CommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(logicalDevice.Device(), &allocInfo, OUT & commandBuffer);

    VkCommandBufferBeginInfo beginInfo {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(OUT commandBuffer, &beginInfo);
    return commandBuffer;
}

void VulkanDevice::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(logicalDevice.GraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(logicalDevice.GraphicsQueue());

    vkFreeCommandBuffers(logicalDevice.Device(), logicalDevice.CommandPool(), 1, OUT & commandBuffer);
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

    CC_ASSERT(vkAllocateMemory(logicalDevice.Device(), &allocInfo, nullptr, OUT & imageMemory) == VK_SUCCESS,
              "Failed to allocate image memory!")

    CC_ASSERT(vkBindImageMemory(logicalDevice.Device(), OUT image, imageMemory, 0) == VK_SUCCESS,
              "Failed to bind image memory!")
}
} // namespace Siege
