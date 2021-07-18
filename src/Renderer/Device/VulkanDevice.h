#pragma once

#include "../../Window/Window.h"
#include "../Renderer.h" 
#include <array>
#include "DebugUtilsMessenger.h"

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

            VulkanDevice(Window& window):
                window(window) 
            {
                SNEK_ASSERT(CreateInstance() == SnekState::Success, "Failed to create Vulkan Instance!");
                std::cout << "SNEKVK: Created VULKAN Instance!" << std::endl;
                
                SNEK_ASSERT(SetupDebugMessenger() == SnekState::Success, "Failed to create Vulkan Instance!");
                if (enableValidationLayers) std::cout << "SNEKVK: Created DebugUtilsMessenger!" << std::endl;

                SNEK_ASSERT(CreateSurface() == SnekState::Success, "Failed to create window surface for glfw!");
                std::cout << "SNEKVK: Created window surface!" << std::endl;

                SNEK_ASSERT(PickPhysicalDevice() == SnekState::Success, "Failed to find a suitable physical device!");
                std::cout << "SNEKVK: Found a suitable physical device!" << std::endl;

                SNEK_ASSERT(CreateLogicalDevice() == SnekState::Success, "Failed to create a logical device!");
                std::cout << "SNEKVK: Successfully created logical device!" << std::endl;

                SNEK_ASSERT(CreateCommandPool() == SnekState::Success, "Failed to create command pool!");
                std::cout << "SNEKVK: Successfully created command pool!" << std::endl;
            }

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

            bool IsDeviceSuitable(VkPhysicalDevice device);

            struct QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

            struct QueueFamilyIndices FindPhysicalQueueFamilies();

            bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

            struct SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

            struct SwapChainSupportDetails GetSwapChainSupport();

            bool CheckValidationLayerSupport();

            struct LayerAndExtensionInfo GetRequiredExtensions();

            bool HasGlfwInstanceExtensions();
    };

    // Utilities 
    // TODO: Move to utils file.
    struct LayerAndExtensionInfo 
    {
        const char** names;
        uint32_t count;
    };

    void DestroyLayerAndExtensionInfo(LayerAndExtensionInfo& info)
    {
        delete [] info.names;
    };

    struct QueueFamilyIndices 
    {
        u32 graphicsFamily;
        u32 presentFamily;

        bool hasGraphicsFamily = false;
        bool hasPresentFamily = false;
    };

    bool IsComplete(const QueueFamilyIndices& indices) { return indices.hasGraphicsFamily && indices.hasPresentFamily; };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        
        VkSurfaceFormatKHR* formats;
        bool hasSurfaceFormat = false;
        
        VkPresentModeKHR* presentModes;
        bool hasPresentMode = false; 
    };

    void DestroySwapChainSupportDetails(SwapChainSupportDetails& swapChainSupport)
    {
        delete swapChainSupport.formats;
        delete swapChainSupport.presentModes;
    };
}