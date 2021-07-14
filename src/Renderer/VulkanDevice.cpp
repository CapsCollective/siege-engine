#include "VulkanDevice.h"
#include <iostream>
#include <cstring>
#include <array>

namespace SnekVk 
{
    std::array<const char*, 1> VulkanDevice::validationLayers = { "VK_LAYER_KHRONOS_validation" };

    std::array<const char*, 1> VulkanDevice::deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    // local callback functions
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }

    SnekState VulkanDevice::CreateInstance() 
    {
        std::cout << CheckValidationLayerSupport() << std::endl;
        if (enableValidationLayers && !CheckValidationLayerSupport()) return SnekState::Failure;
        
        // Create app info
        VkApplicationInfo appInfo {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "SnekVk";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "no engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_2;

        VkInstanceCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        LayerAndExtensionInfo info = GetRequiredExtensions();

        createInfo.ppEnabledExtensionNames = info.names;
        createInfo.enabledExtensionCount = info.count;

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo; 
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            PopulateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
        } 
        else
        {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) 
            return SnekState::Failure; 
        
        return SnekState::Success;
    }

    bool VulkanDevice::CheckValidationLayerSupport()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        VkLayerProperties availableLayers[layerCount];
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

        for (auto& validationLayer : validationLayers)
        {
            bool layerFound = false;
            for (size_t i = 0; i < layerCount; i++)
            {
                if (strcmp(validationLayer, availableLayers[i].layerName) == 0)
                {
                    layerFound = true;
                    break;
                }    
            }
            if (!layerFound) return false;
        }

        return true;
    }

    LayerAndExtensionInfo VulkanDevice::GetRequiredExtensions() 
    {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        uint32_t totalExtensions = enableValidationLayers ? (glfwExtensionCount + 1) : glfwExtensionCount;

        const char** extensions = new const char*;

        for (size_t i = 0; i < glfwExtensionCount; i++)
        {
            extensions[i] = glfwExtensions[i];
        }

        if (enableValidationLayers) extensions[totalExtensions - 1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

        return { extensions, totalExtensions };
    }

    void VulkanDevice::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
    {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
        createInfo.pUserData = nullptr;
    }
}
