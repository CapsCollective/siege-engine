//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "LogicalDevice.h"

#include "Config.h"
#include "utils/CommandPool.h"
#include "utils/Device.h"

// std headers
#include <set>

#if ENABLE_VALIDATION_LAYERS == 1
#define CREATE_LOGICAL_DEVICE(physicalDevice,   \
                              logicalDevice,    \
                              queueCount,       \
                              queueCreateInfos, \
                              extensionCount,   \
                              extensions,       \
                              features,         \
                              ...)              \
    CREATE_DEVICE(physicalDevice,               \
                  logicalDevice,                \
                  queueCount,                   \
                  queueCreateInfos,             \
                  extensionCount,               \
                  extensions,                   \
                  features,                     \
                  __VA_ARGS__)
#else
#define CREATE_LOGICAL_DEVICE(physicalDevice,   \
                              logicalDevice,    \
                              queueCount,       \
                              queueCreateInfos, \
                              extensionCount,   \
                              extensions,       \
                              features,         \
                              ...)              \
    CREATE_DEVICE(physicalDevice,               \
                  logicalDevice,                \
                  queueCount,                   \
                  queueCreateInfos,             \
                  extensionCount,               \
                  extensions,                   \
                  features)
#endif

#define GET_UNIQUE_QUEUES(createInfos, ...)                                                   \
    std::set<uint32_t> uniqueQueueFamilies = {__VA_ARGS__};                                   \
    float queuePriority = 1.0f;                                                               \
    size_t index = 0;                                                                         \
    for (uint32_t queueFamily : uniqueQueueFamilies)                                          \
    {                                                                                         \
        createInfos[index] = Vulkan::Device::QueueCreateInfo(queueFamily, 1, &queuePriority); \
        index++;                                                                              \
    }

namespace Siege::Vulkan
{
LogicalDevice::LogicalDevice(const Instance& instance, PhysicalDevice& physDevice)
{
    physicalDevice = &physDevice;
    auto vkPhysicalDevice = physDevice.GetDevice();

    uint32_t graphicsIdx = Vulkan::Device::Physical::GetGraphicsQueue(vkPhysicalDevice);
    uint32_t presentIdx =
        Vulkan::Device::Physical::GetPresentQueue(vkPhysicalDevice, instance.GetSurface());

    const uint32_t maxQueues = 2;
    uint32_t count = maxQueues - (graphicsIdx == presentIdx);
    VkDeviceQueueCreateInfo queueCreateInfos[count];

    GET_UNIQUE_QUEUES(queueCreateInfos, graphicsIdx, presentIdx)

    auto deviceExtensions = Vulkan::Config::deviceExtensions;
    auto layers = Vulkan::Config::validationLayers;

    CREATE_LOGICAL_DEVICE(
        vkPhysicalDevice,
        device,
        static_cast<uint32_t>(uniqueQueueFamilies.size()),
        queueCreateInfos,
        static_cast<uint32_t>(deviceExtensions.Size()),
        deviceExtensions.Data(),
        Vulkan::Device::Physical::DeviceFeaturesBuilder().WithSamplerAnistropy(VK_TRUE).Build(),
        static_cast<uint32_t>(layers.Size()),
        layers.Data())

    graphicsQueue = Vulkan::Device::GetQueue(device, graphicsIdx, 0);
    presentQueue = Vulkan::Device::GetQueue(device, presentIdx, 0);

    volkLoadDevice(device);

    commandPool = Vulkan::CommandPool::Builder()
                      .WithQueueFamily(Vulkan::Device::Physical::GetGraphicsQueue(vkPhysicalDevice))
                      .WithFlag(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT)
                      .WithFlag(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)
                      .Build(device);
}

LogicalDevice::LogicalDevice(LogicalDevice&& other)
{
    Move(other);
}

LogicalDevice::~LogicalDevice()
{
    if (device) vkDestroyCommandPool(device, commandPool, nullptr);
    vkDestroyDevice(device, nullptr);

    commandPool = VK_NULL_HANDLE;
    device = VK_NULL_HANDLE;
}

LogicalDevice& LogicalDevice::operator=(LogicalDevice&& other)
{
    Move(other);
    return *this;
}

VkCommandBuffer LogicalDevice::GetCommandBuffer()
{
    VkCommandBufferAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;

    CC_ASSERT(vkAllocateCommandBuffers(device, &allocInfo, OUT & commandBuffer) == VK_SUCCESS,
              "Failed to allocate command buffer!")

    return commandBuffer;
}

VkCommandBuffer LogicalDevice::BeginSingleTimeCommand(VkCommandBuffer commandBuffer)
{
    VkCommandBufferBeginInfo beginInfo {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    CC_ASSERT(vkBeginCommandBuffer(OUT commandBuffer, &beginInfo) == VK_SUCCESS,
              "Failed to begin recording command buffer!")

    return commandBuffer;
}

void LogicalDevice::EndSingleTimeCommand(VkCommandBuffer commandBuffer)
{
    FlushCommandBuffer(commandBuffer);
    SubmitToGraphicsQueue(commandBuffer);
    FreeCommandBuffer(commandBuffer);
}

void LogicalDevice::FlushCommandBuffer(VkCommandBuffer commandBuffer)
{
    CC_ASSERT(commandBuffer != VK_NULL_HANDLE, "commandBuffer must be a valid value!")

    vkEndCommandBuffer(commandBuffer);
}

void LogicalDevice::SubmitToGraphicsQueue(VkCommandBuffer commandBuffer)
{
    SubmitToQueue(commandBuffer, graphicsQueue);
}

void LogicalDevice::SubmitToQueue(VkCommandBuffer commandBuffer, VkQueue queue)
{
    VkSubmitInfo submitInfo {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = 0;
    VkFence fence;

    CC_ASSERT(vkCreateFence(device, &fenceCreateInfo, nullptr, &fence) == VK_SUCCESS,
              "Failed to create Fence!");

    CC_ASSERT(vkQueueSubmit(queue, 1, &submitInfo, fence) == VK_SUCCESS,
              "Failed to submit to queue!")

    CC_ASSERT(vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX) == VK_SUCCESS,
              "Failed to wait for successful execution of queue submission!")

    vkDestroyFence(device, fence, nullptr);
}

void LogicalDevice::FreeCommandBuffer(VkCommandBuffer commandBuffer)
{
    vkFreeCommandBuffers(device, GetCommandPool(), 1, OUT & commandBuffer);
}

void LogicalDevice::AllocateImageMemory(VkImage& image,
                                        VkDeviceMemory& imageMemory,
                                        VkMemoryPropertyFlags targetProperties)
{
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, OUT & memRequirements);

    AllocateMemory(memRequirements, imageMemory, targetProperties);
}

void LogicalDevice::AllocateMemory(VkMemoryRequirements& memRequirements,
                                   VkDeviceMemory& memory,
                                   VkMemoryPropertyFlags targetProperties)
{
    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Device::Physical::FindMemoryType(physicalDevice->GetDevice(),
                                                                 memRequirements.memoryTypeBits,
                                                                 targetProperties);

    CC_ASSERT(vkAllocateMemory(device, &allocInfo, nullptr, OUT & memory) == VK_SUCCESS,
              "Failed to allocate image memory!")
}

void LogicalDevice::CreateImage(const VkImageCreateInfo& createInfo, VkImage& image)
{
    CC_ASSERT(vkCreateImage(device, &createInfo, nullptr, OUT & image) == VK_SUCCESS,
              "Failed to create FrameImages!")
}

void LogicalDevice::BindImageMemory(VkImage& image,
                                    VkDeviceMemory& imageMemory,
                                    const VkDeviceSize& offset)
{
    CC_ASSERT(vkBindImageMemory(device, OUT image, imageMemory, offset) == VK_SUCCESS,
              "Failed to bind image memory!")
}

void LogicalDevice::CreateImageWithInfo(const VkImageCreateInfo& createInfo,
                                        VkImage& image,
                                        VkDeviceMemory& imageMemory,
                                        VkMemoryPropertyFlags targetProperties)
{
    CreateImage(createInfo, image);
    AllocateImageMemory(image, imageMemory, targetProperties);
    BindImageMemory(image, imageMemory);
}

void LogicalDevice::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    auto commandBuffer = BeginSingleTimeCommand(GetCommandBuffer());

    VkBufferCopy copyRegion {};
    copyRegion.srcOffset = 0; // Optional
    copyRegion.dstOffset = 0; // Optional
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    EndSingleTimeCommand(commandBuffer);
}

void LogicalDevice::CopyBufferToImage(VkBuffer buffer,
                                      VkImage image,
                                      uint32_t width,
                                      uint32_t height,
                                      uint32_t layerCount)
{
    auto commandBuffer = BeginSingleTimeCommand(GetCommandBuffer());

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

    EndSingleTimeCommand(commandBuffer);
}

void LogicalDevice::Move(LogicalDevice& other)
{
    device = other.device;
    graphicsQueue = other.graphicsQueue;
    presentQueue = other.presentQueue;
    commandPool = other.commandPool;
    physicalDevice = other.physicalDevice;

    other.device = VK_NULL_HANDLE;
    other.graphicsQueue = VK_NULL_HANDLE;
    other.presentQueue = VK_NULL_HANDLE;
    other.commandPool = VK_NULL_HANDLE;
    other.physicalDevice = nullptr;
}
} // namespace Siege::Vulkan