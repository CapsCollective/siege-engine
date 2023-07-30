//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Device.h"

#include <set>

static constexpr uint32_t UNFOUND_DEVICE_ITEM_IDX = -1;

namespace Siege::Vulkan
{
// Device

VkDeviceQueueCreateInfo Device::QueueCreateInfo(const uint32_t& familyIndex,
                                                const uint32_t count,
                                                const float* priority,
                                                VkDeviceQueueCreateFlags flags,
                                                void* pNext)
{
    return {VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, pNext, flags, familyIndex, count, priority};
}

VkQueue Device::GetQueue(VkDevice device, uint32_t id, uint32_t queueIndex)
{
    VkQueue queue;
    vkGetDeviceQueue(device, id, queueIndex, OUT & queue);
    return queue;
}

// Physical Device

uint32_t Device::Physical::deviceCount = 0;

uint32_t Device::Physical::GetGraphicsQueue(VkPhysicalDevice device)
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
    return UNFOUND_DEVICE_ITEM_IDX;
}

uint32_t Device::Physical::GetPresentQueue(VkPhysicalDevice device, VkSurfaceKHR surface)
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
    return UNFOUND_DEVICE_ITEM_IDX;
}

void Device::Physical::GetDevices(VkInstance instance, VkPhysicalDevice* devices)
{
    vkEnumeratePhysicalDevices(instance, &deviceCount, OUT devices);
}

VkPhysicalDevice Device::Physical::FindSuitableDevice(VkInstance instance,
                                                      VkSurfaceKHR surface,
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

bool Device::Physical::IsSuitable(VkPhysicalDevice device,
                                  VkSurfaceKHR surface,
                                  const char* const* deviceExtensions,
                                  size_t deviceExtensionCount)
{
    return HasRequiredQueues(device, surface) &&
           SupportsExtensions(device, deviceExtensions, deviceExtensionCount) &&
           (HasFormats(device, surface) && HasPresentModes(device, surface)) &&
           GetDeviceFeatures(device).samplerAnisotropy;
}

bool Device::Physical::HasRequiredQueues(VkPhysicalDevice device,
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

bool Device::Physical::GraphicsQueuePresent(const VkQueueFamilyProperties& queueFamily)
{
    return queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT;
}

bool Device::Physical::PresentQueuePresent(const VkQueueFamilyProperties& queueFamily)
{
    return queueFamily.queueCount > 0;
}

void Device::Physical::GetQueueFamilies(VkPhysicalDevice device,
                                        uint32_t& count,
                                        VkQueueFamilyProperties* queueFamilies)
{
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, OUT queueFamilies);
}

void Device::Physical::GetQueueCount(VkPhysicalDevice device, uint32_t& count)
{
    vkGetPhysicalDeviceQueueFamilyProperties(device, OUT & count, nullptr);
}

bool Device::Physical::IsSurfaceSupported(VkPhysicalDevice device,
                                          VkSurfaceKHR surface,
                                          const uint32_t& index)
{
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface, OUT & presentSupport);
    return presentSupport;
}

bool Device::Physical::SupportsExtensions(VkPhysicalDevice device,
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

uint32_t Device::Physical::GetSurfaceFormatCount(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    uint32_t count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, OUT & count, nullptr);
    return count;
}

void Device::Physical::GetSurfaceFormats(VkPhysicalDevice device,
                                         VkSurfaceKHR surface,
                                         uint32_t& count,
                                         VkSurfaceFormatKHR* formats)
{
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, OUT & count, formats);
}

VkSurfaceCapabilitiesKHR Device::Physical::GetSurfaceCapabilities(VkPhysicalDevice device,
                                                                  VkSurfaceKHR surface)
{
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, OUT & capabilities);
    return capabilities;
}

Siege::MHArray<VkSurfaceFormatKHR> Device::Physical::GetSurfaceFormats(VkPhysicalDevice device,
                                                                       VkSurfaceKHR surface)
{
    uint32_t formatCount = GetSurfaceFormatCount(device, surface);

    Siege::MHArray<VkSurfaceFormatKHR> formats(formatCount);

    Vulkan::Device::Physical::GetSurfaceFormats(device, surface, formatCount, formats.Data());

    return formats;
}

Siege::MHArray<VkPresentModeKHR> Device::Physical::GetPresentModes(VkPhysicalDevice device,
                                                                   VkSurfaceKHR surface)
{
    uint32_t presentModeCount = Vulkan::Device::Physical::GetPresentModeCount(device, surface);

    Siege::MHArray<VkPresentModeKHR> modes(presentModeCount);

    Vulkan::Device::Physical::GetPresentModes(device, surface, presentModeCount, modes.Data());

    return modes;
}

uint32_t Device::Physical::FindMemoryType(VkPhysicalDevice device,
                                          uint32_t typeFilter,
                                          VkMemoryPropertyFlags flags)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(device, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & flags) == flags)
        {
            return i;
        }
    }

    CC_ASSERT(false, "Failed to find suitable memory type!")
    return UNFOUND_DEVICE_ITEM_IDX;
}

VkFormat Device::Physical::FindSupportedFormat(VkPhysicalDevice device,
                                               const VkFormat* candidates,
                                               size_t formatCount,
                                               VkImageTiling tiling,
                                               VkFormatFeatureFlagBits features)
{
    for (size_t i = 0; i < formatCount; i++)
    {
        VkFormat format = candidates[i];

        VkFormatProperties props = Vulkan::Device::Physical::GetProperties(device, format);

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
    return {};
}

bool Device::Physical::HasFormats(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    return GetSurfaceFormatCount(device, surface) > 0;
}

bool Device::Physical::HasPresentModes(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    return GetPresentModeCount(device, surface) > 0;
}

uint32_t Device::Physical::GetPresentModeCount(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, OUT & presentModeCount, nullptr);
    return presentModeCount;
}

void Device::Physical::GetPresentModes(VkPhysicalDevice device,
                                       VkSurfaceKHR surface,
                                       uint32_t& count,
                                       VkPresentModeKHR* presentModes)
{
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, presentModes);
}

VkPhysicalDeviceFeatures Device::Physical::GetDeviceFeatures(VkPhysicalDevice device)
{
    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, OUT & supportedFeatures);
    return supportedFeatures;
}

VkPhysicalDeviceProperties Device::Physical::GetDeviceProperties(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, OUT & properties);
    return properties;
}

MHArray<VkQueueFamilyProperties> Device::Physical::GetDeviceQueueFamilyIndices(
    VkPhysicalDevice device)
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, OUT & queueFamilyCount, nullptr);

    VkQueueFamilyProperties queueFamilies[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, OUT queueFamilies);

    return MHArray<VkQueueFamilyProperties>(queueFamilies, queueFamilyCount);
}

int32_t Device::Physical::GetMatchingQueueFamilyIndex(VkPhysicalDevice device,
                                                      VkSurfaceKHR surface,
                                                      QueueCallback condition)
{
    auto indices = GetDeviceQueueFamilyIndices(device);

    for (size_t i = 0; i < indices.Size(); i++)
    {
        if (condition(device, indices[i], i, surface) == i) return i;
    }

    return 0;
}

VkFormatProperties Device::Physical::GetProperties(VkPhysicalDevice device, VkFormat format)
{
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(device, format, OUT & props);
    return props;
}

// Logical Device

VkDeviceCreateInfo Device::Logical::CreateInfo(uint32_t queueCreateInfoCount,
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
