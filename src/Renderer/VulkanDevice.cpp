#include "VulkanDevice.h"
#include <iostream>
#include <cstring>

namespace SnekVk 
{

    const char* VulkanDevice::validationLayers[] = {"VK_LAYER_KHRONOS_validation"};
    const char* VulkanDevice::deviceExtensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    SnekState VulkanDevice::CreateInstance() 
    {
        if (enableValidationLayers && !checkValidationLayerSupport())
        {
            return SnekState::Failure;
        }
        return SnekState::Success;
    }

    bool VulkanDevice::checkValidationLayerSupport()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        VkLayerProperties availableLayers[layerCount];
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

        for (size_t i = 0; i < layerCount; i++)
        {
            bool layerFound = false;
            for (size_t j = 0; j < layerCount; j++)
            {
                if (strcmp(validationLayers[i], availableLayers[j].layerName) == 0)
                {
                    layerFound = true;
                    break;
                }    
            }
            if (!layerFound) return false;
        }
        return true;
    }
}
