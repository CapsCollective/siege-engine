#include "VulkanDevice.h"
#include <iostream>
#include <cstring>
#include <array>
#include <set>

namespace SnekVk 
{
    std::array<const char*, 1> VulkanDevice::validationLayers = { "VK_LAYER_KHRONOS_validation" };

    std::array<const char*, 1> VulkanDevice::deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    SnekState VulkanDevice::CreateInstance() 
    {
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

        SNEK_ASSERT(HasGlfwInstanceExtensions(), "must have GLFW extensions to initialise Vulkan!");

        return SnekState::Success;
    }

    SnekState VulkanDevice::SetupDebugMessenger()
    {
        SnekState state = SnekState::Success;

        if (!enableValidationLayers) return state;

        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        SnekVK::PopulateDebugMessengerCreateInfo(createInfo);

        if (SnekVK::CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) state = SnekState::Failure;
        
        return state;
    }

    SnekState VulkanDevice::CreateSurface() 
    { 
        return window.CreateWindowSurface(instance, &surface) ? SnekState::Success : SnekState::Failure; 
    }

    SnekState VulkanDevice::PickPhysicalDevice()
    {
        SnekState state = SnekState::Success;
        u32 deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) return SnekState::Failure;

        VkPhysicalDevice devices[deviceCount];
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

        for (size_t i = 0; i < deviceCount; i++)
        {
            VkPhysicalDevice device = devices[i];
            if (IsDeviceSuitable(device))
            {
                VkPhysicalDeviceProperties properties;
                vkGetPhysicalDeviceProperties(device, &properties);
                std::cout << "FOUND DEVICE: " << properties.deviceName << std::endl;
                physicalDevice = device;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE) state = SnekState::Failure;
        
        return state;
    }

    SnekState VulkanDevice::CreateLogicalDevice()
    {
        QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

        std::set<u32> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};
        VkDeviceQueueCreateInfo queueCreateInfos[uniqueQueueFamilies.size()];

        float queuePriority = 1.0f;

        size_t index = 0; 
        for (auto& queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily; 
            queueCreateInfo.queueCount = 1; 
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos[index] = queueCreateInfo;
            index++;
        } 

        VkPhysicalDeviceFeatures deviceFeatures = {};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<u32>(uniqueQueueFamilies.size());
        createInfo.pQueueCreateInfos = queueCreateInfos;

        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<u32>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<u32>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else createInfo.enabledLayerCount = 0;

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
        {
            return SnekState::Failure;
        }

        vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily, 0, &presentQueue);

        return SnekState::Success;
    }

    bool VulkanDevice::IsDeviceSuitable(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices = FindQueueFamilies(device);

        bool extensionsSupported = CheckDeviceExtensionSupport(device);

        bool swapChainAdequate = false;

        if (extensionsSupported)
        {
            SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
            swapChainAdequate = swapChainSupport.hasPresentMode && swapChainSupport.hasSurfaceFormat;
            DestroySwapChainSupportDetails(swapChainSupport);
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        return IsComplete(indices) && extensionsSupported && swapChainAdequate &&
                supportedFeatures.samplerAnisotropy;
    }

    SwapChainSupportDetails VulkanDevice::QuerySwapChainSupport(VkPhysicalDevice device)
    {
        SwapChainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        u32 formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
 
        VkSurfaceFormatKHR* formats = new VkSurfaceFormatKHR;
        if (formatCount != 0)
        {
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats);
        }

        u32 presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        VkPresentModeKHR* presentModes = new VkPresentModeKHR;

        if (presentModeCount != 0)
        {
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                device,
                surface, 
                &presentModeCount,
                presentModes
            );
        }

        details.formats = formats;
        details.hasSurfaceFormat = formatCount > 0;
        details.presentModes = presentModes;
        details.hasPresentMode = presentModeCount > 0;

        return details;
    }

    QueueFamilyIndices VulkanDevice::FindQueueFamilies(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices;

        u32 queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        VkQueueFamilyProperties queueFamilies[queueFamilyCount];
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

        for (size_t i = 0; i < queueFamilyCount; i++)
        {
            auto& queueFamily = queueFamilies[i];

            // Check if device queues support graphics operatrions
            if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamily = i;
                indices.hasGraphicsFamily = true;
            }

            VkBool32 presentSupport = false;

            // Check if device supports operations to our surface (glfw)
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

            if (queueFamily.queueCount > 0 && presentSupport)
            {
                indices.presentFamily = i;
                indices.hasPresentFamily = true;
            }

            if (IsComplete(indices)) break;
        }
        return indices;
    }

    bool VulkanDevice::CheckDeviceExtensionSupport(VkPhysicalDevice device)
    {
        u32 extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        VkExtensionProperties availableExtensions[extensionCount];
        vkEnumerateDeviceExtensionProperties(
            device, 
            nullptr,
            &extensionCount,
            availableExtensions
        );

        bool hasRequiredExtensions = true;

        for (auto& requiredExtension : deviceExtensions)
        {
            bool hasExtension = false;
            for (size_t i = 0; i < extensionCount; i++)
            {
                const char* deviceExtension = availableExtensions[i].extensionName;
                if (strcmp(requiredExtension, deviceExtension) == 0)
                {
                    hasExtension = true;
                    break;
                }
            }
            if (!hasExtension)
            {
                hasRequiredExtensions = false;
                break;
            }
        }

        return hasRequiredExtensions;
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

    void VulkanDevice::DestroyVulkanDevice() 
    {
        vkDestroyDevice(device, nullptr);

        if (enableValidationLayers) SnekVK::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);

        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
    }
}
