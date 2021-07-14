#pragma once

#include "../Window/Window.h"
#include "Renderer.h" 
#include <vulkan/vulkan.h>
#include <array>
#include <cassert>
#include <iostream>

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

            VulkanDevice(Window& window):
                window(window) 
            {
                assert(CreateInstance() == SnekState::Success && "Failed to create Vulkan Instance!");
                std::cout << "SNEKVK: Created VULKAN Instance!" << std::endl;
            }

            ~VulkanDevice() {}

            SnekState CreateInstance();

            bool CheckValidationLayerSupport();

            struct LayerAndExtensionInfo GetRequiredExtensions();

        private:

            static std::array<const char*, 1> validationLayers;

            static std::array<const char*, 1> deviceExtensions;

            Window& window;

            VkInstance instance;

            void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    };

    struct LayerAndExtensionInfo 
    {
        const char** names;
        uint32_t count;
    };
}