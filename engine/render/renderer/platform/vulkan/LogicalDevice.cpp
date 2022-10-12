//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "LogicalDevice.h"

#include "Config.h"
#include "utils/Buffer.h"
#include "utils/CommandBuffer.h"
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
LogicalDevice::LogicalDevice(const Surface& surface, PhysicalDevice& physDevice)
{
    physicalDevice = &physDevice;
    auto vkPhysicalDevice = physDevice.GetDevice();

    uint32_t graphicsIdx = Vulkan::Device::Physical::GetGraphicsQueue(vkPhysicalDevice);
    uint32_t presentIdx = Vulkan::Device::Physical::GetPresentQueue(vkPhysicalDevice, surface);

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
    return Utils::CommandBuffer::AllocateCommandBuffer(device, commandPool);
}

VkCommandBuffer LogicalDevice::BeginSingleTimeCommand(VkCommandBuffer commandBuffer)
{
    Utils::CommandBuffer::BeginSingleTimeCommand(commandBuffer);

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
              "Failed to create Fence!")

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

void LogicalDevice::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint64_t size)
{
    auto commandBuffer = BeginSingleTimeCommand(GetCommandBuffer());

    auto copyRegion = Utils::Buffer::CopyRegion(size);
    Utils::Buffer::CopyBuffer(commandBuffer, srcBuffer, dstBuffer, copyRegion, 1);

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
