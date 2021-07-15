#include "VulkanDevice.h"
#include <iostream>
#include <cstring>
#include <array>

namespace SnekVk 
{
    std::array<const char*, 1> VulkanDevice::validationLayers = { "VK_LAYER_KHRONOS_validation" };

    std::array<const char*, 1> VulkanDevice::deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    SnekState VulkanDevice::CreateInstance() 
    {
        SNEK_ASSERT(1 < 0, "ONE IS NOT LESS THAN 0!");
        if (enableValidationLayers && !CheckValidationLayerSupport()) 
            return SnekState::Failure;
        
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

            SnekVK::PopulateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
        } 
        else
        {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) 
            return SnekState::Failure; 
        
        DestroyLayerAndExtensionInfo(info);

        SNEK_ASSERT(HasGlfwInstanceExtensions(), "must have GLFW extensions to process Vulkan!");

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
        u32 glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        u32 totalExtensions = enableValidationLayers ? (glfwExtensionCount + 1) : glfwExtensionCount;

        const char** extensions = new const char*;

        for (size_t i = 0; i < glfwExtensionCount; i++)
        {
            extensions[i] = glfwExtensions[i];
        }

        if (enableValidationLayers) extensions[totalExtensions - 1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

        return { extensions, totalExtensions };
    }

    bool VulkanDevice::HasGlfwInstanceExtensions() 
    {
        bool hasExtensions = true;
        u32 extensionCount = 0;

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        VkExtensionProperties extensions[extensionCount];
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions);

        std::cout << "Available extensions: " << std::endl;
        for (size_t i = 0; i < extensionCount; i++)
        {
            const char* extension = extensions[i].extensionName;
            std::cout << "\t" << extension << std::endl;
        }

        std::cout << "Required extensions: " << std::endl;
        LayerAndExtensionInfo requiredExtensions = GetRequiredExtensions();

        for (size_t i = 0; i < requiredExtensions.count; i++)
        {
            const char* required = requiredExtensions.names[i];
            std::cout << "\t" << required << std::endl;
            bool found = false;
            
            for (size_t j = 0; j < extensionCount; j++)
            {
                const char* extension = extensions[j].extensionName;
                found = strcmp(extension, required) == 0;
                if (found) break;
            }

            if (!found)
            {
                hasExtensions = false;
                break;
            } 
        }

        DestroyLayerAndExtensionInfo(requiredExtensions);
        return hasExtensions;
    }
}
