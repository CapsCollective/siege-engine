//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Device.h"

#include <set>

namespace Siege::Vulkan
{
// Device

VkDeviceQueueCreateInfo VulkanDevice::QueueCreateInfo(const uint32_t& familyIndex,
                                                      const uint32_t count,
                                                      const float* priority,
                                                      VkDeviceQueueCreateFlags flags,
                                                      void* pNext)
{
    return {VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, pNext, flags, familyIndex, count, priority};
}

VkQueue VulkanDevice::GetQueue(VkDevice device, uint32_t id, uint32_t queueIndex)
{
    VkQueue queue;
    vkGetDeviceQueue(device, id, queueIndex, OUT & queue);
    return queue;
}

void VulkanDevice::SubmitToQueue(VkQueue& queue,
                                 uint32_t bufferCount,
                                 VkCommandBuffer* buffers,
                                 uint32_t waitSemaphoreCount,
                                 const VkSemaphore* pWaitSemaphores,
                                 const VkPipelineStageFlags* pWaitDstStageMask,
                                 uint32_t signalSemaphoreCount,
                                 const VkSemaphore* pSignalSemaphores,
                                 const void* pNext)
{
    VkSubmitInfo submitInfo {VK_STRUCTURE_TYPE_SUBMIT_INFO,
                             pNext,
                             waitSemaphoreCount,
                             pWaitSemaphores,
                             pWaitDstStageMask,
                             bufferCount,
                             buffers,
                             signalSemaphoreCount,
                             pSignalSemaphores};

    vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(queue);
}

// Physical Device

uint32_t VulkanDevice::Physical::deviceCount = 0;

uint32_t VulkanDevice::Physical::GetGraphicsQueue(VkPhysicalDevice device)
{
    uint32_t queueFamilyCount = 0;
    GetQueueCount(device, OUT queueFamilyCount);
    VkQueueFamilyProperties queueFamilies[queueFamilyCount];
    GetQueueFamilies(device, queueFamilyCount, OUT queueFamilies);

    for (size_t i = 0; i < queueFamilyCount; i++)
    {
        if (GraphicsQueuePresent(queueFamilies[i])) return i;
    }

    CC_ASSERT(false, "No Graphics queue found for device!")
}

uint32_t VulkanDevice::Physical::GetPresentQueue(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    uint32_t queueFamilyCount = 0;
    GetQueueCount(device, OUT queueFamilyCount);
    VkQueueFamilyProperties queueFamilies[queueFamilyCount];
    GetQueueFamilies(device, queueFamilyCount, OUT queueFamilies);

    for (size_t i = 0; i < queueFamilyCount; i++)
    {
        if (PresentQueuePresent(queueFamilies[i]) && IsSurfaceSupported(device, surface, i))
        {
            return i;
        }
    }

    CC_ASSERT(false, "No Present queue found for device!")
}

void VulkanDevice::Physical::GetDevices(VkInstance instance, VkPhysicalDevice* devices)
{
    vkEnumeratePhysicalDevices(instance, &deviceCount, OUT devices);
}

VkPhysicalDevice VulkanDevice::Physical::FindSuitableDevice(VkInstance instance,
                                                            VkSurfaceKHR& surface,
                                                            const char* const* deviceExtensions,
                                                            size_t deviceExtensionCount)
{
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    CC_ASSERT(deviceCount > 0, "Failed to find GPUs with Vulkan Support!")

    CC_LOG_INFO("Device count: {}", deviceCount)

    VkPhysicalDevice device {VK_NULL_HANDLE};

    VkPhysicalDevice devices[deviceCount];
    GetDevices(instance, devices);

    for (size_t i = 0; i < deviceCount; i++)
    {
        VkPhysicalDevice newDevice = devices[i];
        if (IsSuitable(newDevice, surface, deviceExtensions, deviceExtensionCount))
        {
            device = newDevice;
            break;
        }
    }

    return device;
}

bool VulkanDevice::Physical::IsSuitable(VkPhysicalDevice device,
                                        VkSurfaceKHR surface,
                                        const char* const* deviceExtensions,
                                        size_t deviceExtensionCount)
{
    return HasRequiredQueues(device, surface) &&
           SupportsExtensions(device, deviceExtensions, deviceExtensionCount) &&
           (HasFormats(device, surface) && HasPresentModes(device, surface)) &&
           GetDeviceFeatures(device).samplerAnisotropy;
}

bool VulkanDevice::Physical::HasRequiredQueues(VkPhysicalDevice device,
                                               VkSurfaceKHR surface,
                                               RequiredQueues required)
{
    uint32_t queueFamilyCount = 0;
    GetQueueCount(device, OUT queueFamilyCount);
    VkQueueFamilyProperties queueFamilies[queueFamilyCount];
    GetQueueFamilies(device, queueFamilyCount, OUT queueFamilies);

    bool graphicsRequired = required.graphicsQueueRequired;
    bool presentRequired = required.presentQueueRequired;

    bool graphicsQueuePresent = false;
    bool presentQueuePresent = false;

    bool queuesExist = false;

    for (size_t i = 0; i < queueFamilyCount; i++)
    {
        auto queueFamily = queueFamilies[i];

        graphicsQueuePresent = static_cast<bool>(
            graphicsRequired * (graphicsQueuePresent + GraphicsQueuePresent(queueFamily)));

        presentQueuePresent =
            static_cast<bool>((presentRequired * IsSurfaceSupported(device, surface, i)) *
                              (presentQueuePresent + PresentQueuePresent(queueFamily)));

        queuesExist =
            (graphicsQueuePresent == presentRequired) && (presentQueuePresent == presentRequired);

        if (queuesExist) break;
    }

    return queuesExist;
}

bool VulkanDevice::Physical::GraphicsQueuePresent(const VkQueueFamilyProperties& queueFamily)
{
    return queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT;
}

bool VulkanDevice::Physical::PresentQueuePresent(const VkQueueFamilyProperties& queueFamily)
{
    return queueFamily.queueCount > 0;
}

void VulkanDevice::Physical::GetQueueFamilies(VkPhysicalDevice device,
                                              uint32_t& count,
                                              VkQueueFamilyProperties* queueFamilies)
{
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, OUT queueFamilies);
}

void VulkanDevice::Physical::GetQueueCount(VkPhysicalDevice device, uint32_t& count)
{
    vkGetPhysicalDeviceQueueFamilyProperties(device, OUT & count, nullptr);
}

bool VulkanDevice::Physical::IsSurfaceSupported(VkPhysicalDevice device,
                                                VkSurfaceKHR surface,
                                                const uint32_t& index)
{
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface, OUT & presentSupport);
    return presentSupport;
}

bool VulkanDevice::Physical::SupportsExtensions(VkPhysicalDevice device,
                                                const char* const* deviceExtensions,
                                                size_t deviceExtensionCount)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, OUT & extensionCount, nullptr);

    VkExtensionProperties availableExtensions[extensionCount];
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, OUT availableExtensions);

    // We use a set here so that we can store only unique values.
    std::set<String> requiredExtensions;

    for (size_t i = 0; i < deviceExtensionCount; i++)
    {
        requiredExtensions.insert(deviceExtensions[i]);
    }

    for (size_t i = 0; i < extensionCount; i++)
    {
        VkExtensionProperties extension = availableExtensions[i];
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

void VulkanDevice::Physical::GetSurfaceFormatCount(VkPhysicalDevice device,
                                                   VkSurfaceKHR surface,
                                                   uint32_t& count)
{
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, OUT & count, nullptr);
}

void VulkanDevice::Physical::GetSurfaceFormats(VkPhysicalDevice device,
                                               VkSurfaceKHR surface,
                                               uint32_t& count,
                                               VkSurfaceFormatKHR* formats)
{
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, OUT & count, formats);
}

VkSurfaceCapabilitiesKHR VulkanDevice::Physical::GetSurfaceCapabilities(VkPhysicalDevice device,
                                                                        VkSurfaceKHR surface)
{
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, OUT & capabilities);
    return capabilities;
}

bool VulkanDevice::Physical::HasFormats(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    uint32_t formatCount = 0;
    GetSurfaceFormatCount(device, surface, formatCount);
    return formatCount > 0;
}

bool VulkanDevice::Physical::HasPresentModes(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    return GetPresentModeCount(device, surface) > 0;
}

uint32_t VulkanDevice::Physical::GetPresentModeCount(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, OUT & presentModeCount, nullptr);
    return presentModeCount;
}

void VulkanDevice::Physical::GetPresentModes(VkPhysicalDevice device,
                                             VkSurfaceKHR surface,
                                             uint32_t& count,
                                             VkPresentModeKHR* presentModes)
{
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, presentModes);
}

VkPhysicalDeviceFeatures VulkanDevice::Physical::GetDeviceFeatures(VkPhysicalDevice device)
{
    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, OUT & supportedFeatures);
    return supportedFeatures;
}

VkPhysicalDeviceProperties VulkanDevice::Physical::GetDeviceProperties(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, OUT & properties);
    return properties;
}

VkFormatProperties VulkanDevice::Physical::GetProperties(VkPhysicalDevice device, VkFormat format)
{
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(device, format, OUT & props);
    return props;
}

// Logical Device

VkDeviceCreateInfo VulkanDevice::Logical::CreateInfo(
    uint32_t queueCreateInfoCount,
    const VkDeviceQueueCreateInfo* pQueueCreateInfos,
    uint32_t enabledExtensionCount,
    const char* const* ppEnabledExtensionNames,
    const VkPhysicalDeviceFeatures* pEnabledFeatures,
    uint32_t enabledLayerCount,
    const char* const* ppEnabledLayerNames,
    VkDeviceCreateFlags flags,
    const void* pNext)
{
    return {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            pNext,
            flags,
            queueCreateInfoCount,
            pQueueCreateInfos,
            enabledLayerCount,
            ppEnabledLayerNames,
            enabledExtensionCount,
            ppEnabledExtensionNames,
            pEnabledFeatures};
}

} // namespace Siege::Vulkan