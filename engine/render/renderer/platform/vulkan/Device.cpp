//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Device.h"

#include "render/renderer/platform/vulkan/utils/Buffer.h"

namespace Siege
{

Device* Device::deviceInstance = nullptr;

Device::Device(Window* window)
{
    auto surfaceExtensions = window->GetRequiredExtensions();

    instance = Vulkan::VulkanInstance(Window::CreateWindowSurface,
                                      surfaceExtensions.Data(),
                                      surfaceExtensions.Size());

    physicalDevice = Vulkan::PhysicalDevice(instance);
    logicalDevice = Vulkan::LogicalDevice(physicalDevice, instance.Surface());

    SetDeviceInstance(this);
}

Device::Device()
{
    SetDeviceInstance(this);
}

Device::Device(Device&& other)
{
    Move(other);
}

Device& Device::operator=(Device&& other)
{
    Move(other);

    return *this;
}

void Device::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    Vulkan::Buffer::CopyWholeSingleBuffer(size, commandBuffer, srcBuffer, dstBuffer);

    EndSingleTimeCommands(commandBuffer);
}

void Device::CopyBufferToImage(VkBuffer buffer,
                               VkImage image,
                               uint32_t width,
                               uint32_t height,
                               uint32_t layerCount)
{
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    Vulkan::Buffer::CopyWholeSingleBufferToImage(
        commandBuffer,
        buffer,
        Vulkan::Buffer::ImageCopyBuilder()
            .WithSubResources(
                Vulkan::Buffer::ImageSubResourcesBuilder().WithLayerCount(layerCount).Build())
            .WithExtent3D({width, height, 1})
            .Build(),
        image);

    EndSingleTimeCommands(commandBuffer);
}

void Device::CreateImageWithInfo(const VkImageCreateInfo& imageInfo,
                                 VkMemoryPropertyFlags targetProperties,
                                 VkImage& image,
                                 VkDeviceMemory& imageMemory)
{
    logicalDevice.CreateImageWithInfo(imageInfo, targetProperties, image, imageMemory);
}

void Device::Move(Device& other)
{
    instance = std::move(other.instance);
    physicalDevice = std::move(other.physicalDevice);
    logicalDevice = std::move(other.logicalDevice);

    SetDeviceInstance(this);
}
} // namespace Siege
