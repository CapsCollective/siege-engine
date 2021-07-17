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

            VulkanDevice(Window& window):
                window(window) 
            {
                SNEK_ASSERT(CreateInstance() == SnekState::Success, "Failed to create Vulkan Instance!");
                std::cout << "SNEKVK: Created VULKAN Instance!" << std::endl;
                
                SNEK_ASSERT(SetupDebugMessenger() == SnekState::Success, "Failed to create Vulkan Instance!");
                if (enableValidationLayers) std::cout << "SNEKVK: Created DebugUtilsMessenger!" << std::endl;

                SNEK_ASSERT(CreateSurface() == SnekState::Success, "Failed to create window surface for glfw!");
                std::cout << "SNEKVK: Created window surface!" << std::endl;
            }

            ~VulkanDevice() {}

        private:

            static std::array<const char*, 1> validationLayers;

            static std::array<const char*, 1> deviceExtensions;

            Window& window;

            VkInstance instance;

            VkDebugUtilsMessengerEXT debugMessenger;

            VkSurfaceKHR surface;

            SnekState CreateInstance();

            SnekState SetupDebugMessenger();

            SnekState CreateSurface();

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
}