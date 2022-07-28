//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "LogicalDevice.h"

#include <set>

#include "Config.h"
#include "utils/Buffer.h"
#include "utils/CommandBuffer.h"
#include "utils/CommandPool.h"

#define GET_UNIQUE_QUEUES(createInfos, ...)                                        \
    std::set<uint32_t> uniqueQueueFamilies = {__VA_ARGS__};                        \
    float queuePriority = 1.0f;                                                    \
    size_t index = 0;                                                              \
    for (uint32_t queueFamily : uniqueQueueFamilies)                               \
    {                                                                              \
        createInfos[index] =                                                       \
            Vulkan::VulkanDevice::QueueCreateInfo(queueFamily, 1, &queuePriority); \
        index++;                                                                   \
    }

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

namespace Siege::Vulkan
{
LogicalDevice::LogicalDevice(const PhysicalDevice& physDevice, VkSurfaceKHR surface) :
    physicalDevice {physDevice}
{
    auto vkPhysicalDevice = physicalDevice.Device();

    auto extensions = Config::deviceExtensions;
    auto layers = Config::validationLayers;

    uint32_t graphicsIdx = VulkanDevice::Physical::GetGraphicsQueue(vkPhysicalDevice);
    uint32_t presentIdx = VulkanDevice::Physical::GetPresentQueue(vkPhysicalDevice, surface);

    const uint32_t maxQueues = 2;
    uint32_t count = maxQueues - (graphicsIdx == presentIdx);
    VkDeviceQueueCreateInfo queueCreateInfos[count];

    GET_UNIQUE_QUEUES(queueCreateInfos, graphicsIdx, presentIdx)

    CREATE_LOGICAL_DEVICE(
        vkPhysicalDevice,
        device,
        static_cast<uint32_t>(uniqueQueueFamilies.size()),
        queueCreateInfos,
        static_cast<uint32_t>(extensions.size()),
        extensions.data(),
        VulkanDevice::Physical::DeviceFeaturesBuilder().WithSamplerAnistropy(VK_TRUE).Build(),
        static_cast<uint32_t>(layers.size()),
        layers.data())

    graphicsQueue = VulkanDevice::GetQueue(device, graphicsIdx, 0);
    presentQueue = VulkanDevice::GetQueue(device, presentIdx, 0);

    volkLoadDevice(device);

    CreateCommandPool(vkPhysicalDevice);
}

LogicalDevice::LogicalDevice(LogicalDevice&& other)
{
    device = other.device;
    physicalDevice = other.physicalDevice;

    graphicsQueue = other.graphicsQueue;
    presentQueue = other.presentQueue;

    commandPool = other.commandPool;

    other.device = VK_NULL_HANDLE;
    other.graphicsQueue = VK_NULL_HANDLE;
    other.presentQueue = VK_NULL_HANDLE;
    other.commandPool = VK_NULL_HANDLE;
}

LogicalDevice::~LogicalDevice()
{
    if (device != VK_NULL_HANDLE) vkDestroyCommandPool(device, commandPool, nullptr);
    vkDestroyDevice(device, nullptr);

    device = VK_NULL_HANDLE;
    commandPool = VK_NULL_HANDLE;
}

LogicalDevice& LogicalDevice::operator=(LogicalDevice&& other)
{
    device = other.device;
    physicalDevice = other.physicalDevice;

    graphicsQueue = other.graphicsQueue;
    presentQueue = other.presentQueue;

    commandPool = other.commandPool;

    other.device = VK_NULL_HANDLE;
    other.graphicsQueue = VK_NULL_HANDLE;
    other.presentQueue = VK_NULL_HANDLE;
    other.commandPool = VK_NULL_HANDLE;

    return *this;
}

void LogicalDevice::CreateCommandPool(VkPhysicalDevice vkPhysicalDevice)
{
    commandPool = Vulkan::CommandPool::Builder()
                      .WithQueueFamily(VulkanDevice::Physical::GetGraphicsQueue(vkPhysicalDevice))
                      .WithFlag(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT)
                      .WithFlag(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)
                      .Build(device);
}

VkCommandBuffer LogicalDevice::BeginSingleTimeCommands()
{
    auto allocInfo = CommandBuffer::AllocInfo(commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);

    VkCommandBuffer commandBuffer = CommandBuffer::Allocate(device, allocInfo);

    CommandBuffer::BeginCommand(OUT commandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    return commandBuffer;
}

void LogicalDevice::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    CommandBuffer::EndCommand(commandBuffer);

    VulkanDevice::SubmitToQueue(graphicsQueue, 1, &commandBuffer);

    CommandBuffer::DestroyCommandBuffer(device, commandPool, 1, commandBuffer);
}

VkMemoryRequirements LogicalDevice::GetImageMemoryRequirements(VkImage image)
{
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, OUT & memRequirements);
    return memRequirements;
}

void LogicalDevice::CreateImage(const VkImageCreateInfo& imageInfo,
                                VkImage& image,
                                VkAllocationCallbacks* callbacks)
{
    CC_ASSERT(vkCreateImage(device, &imageInfo, callbacks, OUT & image) == VK_SUCCESS,
              "Failed to create FrameImages!")
}

void LogicalDevice::AllocateMemory(VkMemoryAllocateInfo allocInfo,
                                   VkDeviceMemory& memory,
                                   VkAllocationCallbacks* callbacks)
{
    CC_ASSERT(vkAllocateMemory(device, &allocInfo, callbacks, OUT & memory) == VK_SUCCESS,
              "Failed to allocate image memory!")
}

void LogicalDevice::BindImageMemory(VkImage image, VkDeviceMemory memory, VkDeviceSize offset)
{
    CC_ASSERT(vkBindImageMemory(device, OUT image, memory, offset) == VK_SUCCESS,
              "Failed to bind image memory!")
}

void LogicalDevice::CreateImageWithInfo(const VkImageCreateInfo& imageInfo,
                                        VkMemoryPropertyFlags targetProperties,
                                        VkImage& image,
                                        VkDeviceMemory& imageMemory)
{
    CreateImage(imageInfo, image);

    VkMemoryRequirements memRequirements = GetImageMemoryRequirements(image);

    auto memoryType =
        physicalDevice.FindMemoryType(memRequirements.memoryTypeBits, targetProperties);

    VkMemoryAllocateInfo allocInfo = Buffer::AllocateInfo(memRequirements.size, memoryType);

    AllocateMemory(allocInfo, imageMemory);

    BindImageMemory(image, imageMemory);
}
} // namespace Siege::Vulkan