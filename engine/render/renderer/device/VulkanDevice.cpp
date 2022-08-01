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

// std headers
#include <set>

#define GET_UNIQUE_QUEUES(createInfos, ...)                                                   \
    std::set<uint32_t> uniqueQueueFamilies = {__VA_ARGS__};                                   \
    float queuePriority = 1.0f;                                                               \
    size_t index = 0;                                                                         \
    for (uint32_t queueFamily : uniqueQueueFamilies)                                          \
    {                                                                                         \
        createInfos[index] = Vulkan::Device::QueueCreateInfo(queueFamily, 1, &queuePriority); \
        index++;                                                                              \
    }

#define GET_RAW(collection, name, count) \
    for (size_t i = 0; i < count; i++) name[i] = collection[i].Str();

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

#define CREATE_DEBUG(debugInfo)                               \
    DebugUtilsMessenger::PopulateCreateInfo(debugCreateInfo); \
    auto pDebugCreateInfo = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;

#define SETUP_UTILS_MESSENGER SetupDebugMessenger();

#define CREATE_VULKAN_INSTANCE(instance,                       \
                               appInfo,                        \
                               extensions,                     \
                               extensionCount,                 \
                               layers,                         \
                               layerCount,                     \
                               flags)                          \
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;        \
                                                               \
    CREATE_DEBUG_MESSENGER(debugCreateInfo)                    \
    instance = Vulkan::Instance::CreateInstance(appInfo,       \
                                                extensionSize, \
                                                extensions,    \
                                                layerSize,     \
                                                rawLayers,     \
                                                flags)
#else

#define CREATE_VULKAN_INSTANCE(instance,                       \
                               appInfo,                        \
                               extensions,                     \
                               extensionCount,                 \
                               layers,                         \
                               layerCount,                     \
                               flags)                          \
    instance = Vulkan::Instance::CreateInstance(appInfo,       \
                                                extensionSize, \
                                                extensions,    \
                                                layerSize,     \
                                                rawLayers,     \
                                                flags)
#define SETUP_UTILS_MESSENGER

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

namespace Siege
{

VulkanDevice* VulkanDevice::vulkanDeviceInstance = nullptr;

VulkanDevice::VulkanDevice()
{
    Siege::Vulkan::Context::Init(Siege::Window::GetRequiredExtensions,
                                 Siege::Window::CreateWindowSurface);

    PickPhysicalDevice();
    CreateLogicalDevice();
    CreateCommandPool();

    SetVulkanDeviceInstance(this);
}

VulkanDevice::~VulkanDevice()
{
    // When the device goes out of scope, all vulkan structs must be
    // de-allocated in reverse order of how they were created.

    vkDestroyCommandPool(device, commandPool, nullptr);
    vkDestroyDevice(device, nullptr);

    Vulkan::Context::Destroy();
}

void VulkanDevice::PickPhysicalDevice()
{
    auto deviceExtensions = Vulkan::Config::deviceExtensions;

    auto instance = Vulkan::Context::GetVkInstance();
    auto surface = Vulkan::Context::GetInstance().GetSurface();

    physicalDevice = Vulkan::Device::Physical::FindSuitableDevice(instance,
                                                                  surface,
                                                                  deviceExtensions.Data(),
                                                                  deviceExtensions.Size());

    CC_ASSERT(physicalDevice != VK_NULL_HANDLE, "Failed to find a suitable GPU!")

    properties = Vulkan::Device::Physical::GetDeviceProperties(physicalDevice);

    CC_LOG_INFO("Physical device found: {} with minumum buffer alignment of {}",
                properties.deviceName,
                properties.limits.minUniformBufferOffsetAlignment)
}

void VulkanDevice::CreateLogicalDevice()
{
    uint32_t graphicsIdx = Vulkan::Device::Physical::GetGraphicsQueue(physicalDevice);
    uint32_t presentIdx = Vulkan::Device::Physical::GetPresentQueue(physicalDevice, Surface());

    const uint32_t maxQueues = 2;
    uint32_t count = maxQueues - (graphicsIdx == presentIdx);
    VkDeviceQueueCreateInfo queueCreateInfos[count];

    GET_UNIQUE_QUEUES(queueCreateInfos, graphicsIdx, presentIdx)

    auto deviceExtensions = Vulkan::Config::deviceExtensions;
    auto layers = Vulkan::Config::validationLayers;

    CREATE_LOGICAL_DEVICE(
        physicalDevice,
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
}

void VulkanDevice::CreateCommandPool()
{
    commandPool = Vulkan::CommandPool::Builder()
                      .WithQueueFamily(Vulkan::Device::Physical::GetGraphicsQueue(physicalDevice))
                      .WithFlag(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT)
                      .WithFlag(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)
                      .Build(device);
}

VkFormat VulkanDevice::FindSupportedFormat(const VkFormat* candidates,
                                           size_t formatCount,
                                           VkImageTiling tiling,
                                           VkFormatFeatureFlags features)
{
    for (size_t i = 0; i < formatCount; i++)
    {
        VkFormat format = candidates[i];

        VkFormatProperties props = Vulkan::Device::Physical::GetProperties(physicalDevice, format);

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
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
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
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, OUT & commandBuffer);

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

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    vkFreeCommandBuffers(device, commandPool, 1, OUT & commandBuffer);
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
    CC_ASSERT(vkCreateImage(device, &imageInfo, nullptr, OUT & image) == VK_SUCCESS,
              "Failed to create FrameImages!")

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, OUT & memRequirements);

    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, targetProperties);

    CC_ASSERT(vkAllocateMemory(device, &allocInfo, nullptr, OUT & imageMemory) == VK_SUCCESS,
              "Failed to allocate image memory!")

    CC_ASSERT(vkBindImageMemory(device, OUT image, imageMemory, 0) == VK_SUCCESS,
              "Failed to bind image memory!")
}
} // namespace Siege
