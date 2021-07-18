#pragma once

#include "../../Window/Window.h"
#include <array>
#include "Utils/DebugUtilsMessenger.h"
#include "Utils/Instance.h"
#include "Utils/PhysicalDevice.h"

namespace SnekVk
{
    class VulkanDevice 
    {
        public:

        #ifdef DEBUG
            const bool enableValidationLayers = true; 
        #else
            const bool enableValidationLayers = false;
        #endif

            // 'Structors

            VulkanDevice(Window& window);

            // Device is not movable or copyable
            VulkanDevice(const VulkanDevice &) = delete;
            void operator=(const VulkanDevice &) = delete;
            VulkanDevice(VulkanDevice &&) = delete;
            VulkanDevice &operator=(VulkanDevice &&) = delete;

            ~VulkanDevice() {}

            void DestroyVulkanDevice();

            // Getters

            VkCommandPool GetCommandPool() { return commandPool; }
            VkDevice GetDevice() { return device; }
            VkSurfaceKHR GetSurfaceKHR() { return surface; }
            VkQueue GetGraphicsQueue() { return graphicsQueue; }
            VkQueue GetPresentQueue() { return presentQueue; }

        private:

            static std::array<const char*, 1> validationLayers;

            static std::array<const char*, 1> deviceExtensions;

            Window& window;

            VkInstance instance;

            VkDebugUtilsMessengerEXT debugMessenger;

            VkSurfaceKHR surface;

            VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

            VkPhysicalDeviceProperties properties;

            VkDevice device;

            VkQueue graphicsQueue;

            VkQueue presentQueue;

            VkCommandPool commandPool;

            SnekState CreateInstance();

            SnekState SetupDebugMessenger();

            SnekState CreateSurface();

            SnekState PickPhysicalDevice();

            SnekState CreateLogicalDevice();

            SnekState CreateCommandPool();

            SnekVK::QueueFamilyIndices FindPhysicalQueueFamilies();

            SnekVK::SwapChainSupportDetails GetSwapChainSupport();
    };
}