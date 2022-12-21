//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "VulkanDevice.h"

#include <utils/Logging.h>

// std headers
#include <cstring>
#include <set>

#ifdef __APPLE__
#define ADD_PORTABILITY_BIT(info) info.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#else
#define ADD_PORTABILITY_BIT(...)
#endif

namespace Siege
{

VulkanDevice* VulkanDevice::vulkanDeviceInstance = nullptr;

VulkanDevice::VulkanDevice(Window* window) : window {window}
{
    CC_ASSERT(volkInitialize() == VK_SUCCESS, "Unable to initialise Volk!");

    CreateInstance();
    SetupDebugMessenger();
    CreateSurface();
    PickPhysicalDevice();
    CreateLogicalDevice();
    CreateCommandPool();

    SetVulkanDeviceInstance(this);
}

VulkanDevice::VulkanDevice()
{
    SetVulkanDeviceInstance(this);
}

void VulkanDevice::SetWindow(Window* window)
{
    CC_ASSERT(volkInitialize() == VK_SUCCESS, "Unable to initialise Volk!");

    CC_ASSERT(window != nullptr, "Must provide a valid pointer to a window!");

    this->window = window;

    CreateInstance();
    SetupDebugMessenger();
    CreateSurface();
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

    if (enableValidationLayers)
    {
        DebugUtilsMessenger::DestroyMessenger(instance, debugMessenger, nullptr);
    }

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
}

void VulkanDevice::CreateInstance()
{
    if (enableValidationLayers)
    {
        CC_ASSERT(Extensions::CheckValidationLayerSupport(validationLayers.Data(),
                                                          validationLayers.Size()),
                  "Validation Layers are not supported!");
    }

    // Specify general app information.
    VkApplicationInfo appInfo {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Render Example";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Get all extensions required by our windowing system.
    auto extensions = Extensions::GetRequiredExtensions(enableValidationLayers);

    const char* rawExtensions[extensions.Size()];

    for (size_t i = 0; i < extensions.Size(); i++) rawExtensions[i] = extensions[i].Str();

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.Size());
    createInfo.ppEnabledExtensionNames = rawExtensions;

    ADD_PORTABILITY_BIT(createInfo)

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (enableValidationLayers)
    {
        const char* rawLayers[VALIDATION_LAYERS_COUNT];

        for (size_t i = 0; i < VALIDATION_LAYERS_COUNT; i++)
            rawLayers[i] = validationLayers[i].Str();

        // Only add the ability to report on validation layers if the feature is enabled.
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.Size());
        createInfo.ppEnabledLayerNames = rawLayers;

        DebugUtilsMessenger::PopulateCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    CC_ASSERT(vkCreateInstance(&createInfo, nullptr, OUT & instance) == VK_SUCCESS,
              "Unable to create Vulkan Instance!");

    Extensions::HasGflwRequiredInstanceExtensions(enableValidationLayers);

    volkLoadInstance(instance);
}

void VulkanDevice::CreateSurface()
{
    window->CreateWindowSurface(instance, OUT & surface);
}

void VulkanDevice::PickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, OUT & deviceCount, nullptr);

    CC_ASSERT(deviceCount > 0, "Failed to find GPUs with Vulkan Support!");

    CC_LOG_INFO("Device count: {}", deviceCount)

    VkPhysicalDevice devices[deviceCount];
    vkEnumeratePhysicalDevices(instance, &deviceCount, OUT devices);

    for (size_t i = 0; i < deviceCount; i++)
    {
        VkPhysicalDevice pDevice = devices[i];
        if (PhysicalDevice::IsSuitable(pDevice,
                                       surface,
                                       deviceExtensions.Data(),
                                       deviceExtensions.Size()))
        {
            physicalDevice = pDevice;
            break;
        }
    }

    CC_ASSERT(physicalDevice != VK_NULL_HANDLE, "Failed to find a suitable GPU!");

    vkGetPhysicalDeviceProperties(physicalDevice, OUT & properties);

    CC_LOG_INFO("Physical device found: {} with minumum buffer alignment of {}",
                properties.deviceName,
                properties.limits.minUniformBufferOffsetAlignment)
}

void VulkanDevice::CreateLogicalDevice()
{
    QueueFamilyIndices::QueueFamilyIndices indices =
        QueueFamilyIndices::FindQueueFamilies(physicalDevice, surface);

    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};
    VkDeviceQueueCreateInfo queueCreateInfos[uniqueQueueFamilies.size()];

    float queuePriority = 1.0f;

    size_t index = 0;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos[index] = queueCreateInfo;
        index++;
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(uniqueQueueFamilies.size());
    createInfo.pQueueCreateInfos = queueCreateInfos;

    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.Size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.Data();

    // might not really be necessary anymore because device specific validation layers
    // have been deprecated
    if (enableValidationLayers)
    {
        const char* rawLayers[VALIDATION_LAYERS_COUNT];

        for (size_t i = 0; i < VALIDATION_LAYERS_COUNT; i++)
            rawLayers[i] = validationLayers[i].Str();

        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.Size());
        createInfo.ppEnabledLayerNames = rawLayers;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    CC_ASSERT(vkCreateDevice(physicalDevice, &createInfo, nullptr, OUT & device) == VK_SUCCESS,
              "failed to create logical device!");

    vkGetDeviceQueue(device, indices.graphicsFamily, 0, OUT & graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily, 0, OUT & presentQueue);

    volkLoadDevice(device);
}

void VulkanDevice::CreateCommandPool()
{
    QueueFamilyIndices::QueueFamilyIndices queueFamilyIndices = FindPhysicalQueueFamilies();

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
    poolInfo.flags =
        VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    CC_ASSERT(vkCreateCommandPool(device, &poolInfo, nullptr, OUT & commandPool) == VK_SUCCESS,
              "Failed to create command pool!");
}

void VulkanDevice::SetupDebugMessenger()
{
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    DebugUtilsMessenger::PopulateCreateInfo(OUT createInfo);

    CC_ASSERT(DebugUtilsMessenger::CreateMessenger(instance,
                                                   &createInfo,
                                                   nullptr,
                                                   OUT & debugMessenger) == VK_SUCCESS,
              "Failed to create DebugUtilsMessenger!");
}

VkFormat VulkanDevice::FindSupportedFormat(const VkFormat* candidates,
                                           size_t formatCount,
                                           VkImageTiling tiling,
                                           VkFormatFeatureFlags features)
{
    for (size_t i = 0; i < formatCount; i++)
    {
        VkFormat format = candidates[i];

        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, OUT & props);

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
    CC_ASSERT(false, "Failed to find a supported format!");
}

uint32_t VulkanDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    CC_ASSERT(false, "Failed to find suitable memory type!");
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
                                       VkMemoryPropertyFlags properties,
                                       VkImage& image,
                                       VkDeviceMemory& imageMemory)
{
    CC_ASSERT(vkCreateImage(device, &imageInfo, nullptr, OUT & image) == VK_SUCCESS,
              "Failed to create FrameImages!");

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, OUT & memRequirements);

    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

    CC_ASSERT(vkAllocateMemory(device, &allocInfo, nullptr, OUT & imageMemory) == VK_SUCCESS,
              "Failed to allocate image memory!");

    CC_ASSERT(vkBindImageMemory(device, OUT image, imageMemory, 0) == VK_SUCCESS,
              "Failed to bind image memory!");
}
} // namespace Siege
