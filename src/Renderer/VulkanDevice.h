#pragma once

#include "../Window/Window.h"
#include "Renderer.h" 
#include <vulkan/vulkan.h>

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
                CreateInstance();
            }

            ~VulkanDevice() {}

            SnekState CreateInstance();

            bool checkValidationLayerSupport();

        private:

            static const char* validationLayers[];
            static const char* deviceExtensions[];

            Window& window;
    };
}