//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_DEVICE_H
#define SIEGE_ENGINE_DEVICE_H

#include <utils/Logging.h>
#include <utils/collections/HeapArray.h>

#include "volk/volk.h"

#if ENABLE_VALIDATION_LAYERS == 1
#define CREATE_DEVICE(physicalDevice,                                              \
                      logicalDevice,                                               \
                      queueCount,                                                  \
                      queueCreateInfos,                                            \
                      extensionCount,                                              \
                      extensions,                                                  \
                      features,                                                    \
                      ...)                                                         \
    auto deviceFeatures = features;                                                \
    auto createInfo = Siege::Vulkan::Device::Logical::CreateInfo(queueCount,       \
                                                                 queueCreateInfos, \
                                                                 extensionCount,   \
                                                                 extensions,       \
                                                                 &deviceFeatures,  \
                                                                 __VA_ARGS__);     \
    Siege::Vulkan::Device::Logical::Create(physicalDevice, &createInfo, &logicalDevice);
#else
#define CREATE_DEVICE(physicalDevice,                                              \
                      logicalDevice,                                               \
                      queueCount,                                                  \
                      queueCreateInfos,                                            \
                      extensionCount,                                              \
                      extensions,                                                  \
                      features,                                                    \
                      ...)                                                         \
    auto deviceFeatures = features;                                                \
    auto createInfo = Siege::Vulkan::Device::Logical::CreateInfo(queueCount,       \
                                                                 queueCreateInfos, \
                                                                 extensionCount,   \
                                                                 extensions,       \
                                                                 &deviceFeatures); \
    Siege::Vulkan::Device::Logical::Create(physicalDevice, &createInfo, &logicalDevice);
#endif

namespace Siege::Vulkan
{
class Device
{
public:

    typedef int32_t (*QueueCallback)(VkPhysicalDevice,
                                     VkQueueFamilyProperties,
                                     size_t,
                                     VkSurfaceKHR);

    static VkDeviceQueueCreateInfo QueueCreateInfo(const uint32_t& familyIndex,
                                                   const uint32_t count,
                                                   const float* priority,
                                                   VkDeviceQueueCreateFlags flags = 0,
                                                   void* pNext = nullptr);

    static VkQueue GetQueue(VkDevice device, uint32_t id, uint32_t queueIndex);

    class Physical
    {
    public:

        struct RequiredQueues
        {
            bool graphicsQueueRequired {true};
            bool presentQueueRequired {true};

            // Add more queues when needed ...
        };

        static uint32_t deviceCount;

        static void GetDevices(VkInstance instance, VkPhysicalDevice* devices);

        static VkPhysicalDevice FindSuitableDevice(VkInstance instance,
                                                   VkSurfaceKHR surface,
                                                   const char* const* deviceExtensions,
                                                   size_t deviceExtensionCount);
        static bool IsSuitable(VkPhysicalDevice device,
                               VkSurfaceKHR surface,
                               const char* const* deviceExtensions,
                               size_t deviceExtensionCount);

        static bool GraphicsQueuePresent(const VkQueueFamilyProperties& queueFamily);
        static bool PresentQueuePresent(const VkQueueFamilyProperties& queueFamily);

        static uint32_t GetGraphicsQueue(VkPhysicalDevice device);
        static uint32_t GetPresentQueue(VkPhysicalDevice device, VkSurfaceKHR surface);

        static void GetQueueFamilies(VkPhysicalDevice device,
                                     uint32_t& count,
                                     VkQueueFamilyProperties* queueFamilies);

        static void GetQueueCount(VkPhysicalDevice device, uint32_t& count);

        static bool HasRequiredQueues(VkPhysicalDevice device,
                                      VkSurfaceKHR surface,
                                      RequiredQueues required = {true, true});

        static bool IsSurfaceSupported(VkPhysicalDevice device,
                                       VkSurfaceKHR surface,
                                       const uint32_t& index);

        static bool SupportsExtensions(VkPhysicalDevice device,
                                       const char* const* deviceExtensions,
                                       size_t deviceExtensionCount);

        static uint32_t GetSurfaceFormatCount(VkPhysicalDevice device, VkSurfaceKHR surface);

        static void GetSurfaceFormats(VkPhysicalDevice device,
                                      VkSurfaceKHR surface,
                                      uint32_t& count,
                                      VkSurfaceFormatKHR* formats);

        static VkSurfaceCapabilitiesKHR GetSurfaceCapabilities(VkPhysicalDevice device,
                                                               VkSurfaceKHR surface);

        static Siege::MHArray<VkSurfaceFormatKHR> GetSurfaceFormats(VkPhysicalDevice device,
                                                                    VkSurfaceKHR surface);

        static Siege::MHArray<VkPresentModeKHR> GetPresentModes(VkPhysicalDevice device,
                                                                VkSurfaceKHR surface);

        static uint32_t FindMemoryType(VkPhysicalDevice device,
                                       uint32_t typeFilter,
                                       VkMemoryPropertyFlags flags);

        static VkFormat FindSupportedFormat(VkPhysicalDevice device,
                                            const VkFormat* candidates,
                                            size_t formatCount,
                                            VkImageTiling tiling,
                                            VkFormatFeatureFlagBits features);

        static bool HasFormats(VkPhysicalDevice device, VkSurfaceKHR surface);

        static bool HasPresentModes(VkPhysicalDevice device, VkSurfaceKHR surface);

        static uint32_t GetPresentModeCount(VkPhysicalDevice device, VkSurfaceKHR surface);

        static void GetPresentModes(VkPhysicalDevice device,
                                    VkSurfaceKHR surface,
                                    uint32_t& count,
                                    VkPresentModeKHR* presentModes);

        static VkPhysicalDeviceFeatures GetDeviceFeatures(VkPhysicalDevice device);

        static VkPhysicalDeviceProperties GetDeviceProperties(VkPhysicalDevice device);

        static Siege::MHArray<VkQueueFamilyProperties> GetDeviceQueueFamilyIndices(
            VkPhysicalDevice device);

        static int32_t GetMatchingQueueFamilyIndex(VkPhysicalDevice device,
                                                   VkSurfaceKHR surface,
                                                   QueueCallback condition);

        struct DeviceFeaturesBuilder
        {
            DeviceFeaturesBuilder& WithSamplerAnistropy(const VkBool32& anistropy)
            {
                samplerAnistropy = anistropy;
                return *this;
            }

            VkPhysicalDeviceFeatures Build()
            {
                VkPhysicalDeviceFeatures features {};
                features.samplerAnisotropy = samplerAnistropy;
                return features;
            }

            VkBool32 samplerAnistropy = VK_FALSE;
        };

        static VkFormatProperties GetProperties(VkPhysicalDevice device, VkFormat format);
    };

    class Logical
    {
    public:

        static VkDeviceCreateInfo CreateInfo(uint32_t queueCreateInfoCount,
                                             const VkDeviceQueueCreateInfo* pQueueCreateInfos,
                                             uint32_t enabledExtensionCount,
                                             const char* const* ppEnabledExtensionNames,
                                             const VkPhysicalDeviceFeatures* pEnabledFeatures,
                                             uint32_t enabledLayerCount = 0,
                                             const char* const* ppEnabledLayerNames = nullptr,
                                             VkDeviceCreateFlags flags = 0,
                                             const void* pNext = nullptr);

        static inline void Create(VkPhysicalDevice physicalDevice,
                                  VkDeviceCreateInfo* createInfo,
                                  VkDevice* device,
                                  VkAllocationCallbacks* callbacks = nullptr)
        {
            CC_ASSERT(
                vkCreateDevice(physicalDevice, createInfo, callbacks, OUT device) == VK_SUCCESS,
                "failed to create logical device!")
        }

        static inline void WaitIdle(VkDevice device)
        {
            vkDeviceWaitIdle(device);
        }
    };
};

} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_DEVICE_H
