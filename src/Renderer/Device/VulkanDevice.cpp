#include "VulkanDevice.h"
#include <iostream>
#include <cstring>
#include <array>
#include <set>

namespace SnekVk 
{
    std::array<const char*, 1> VulkanDevice::validationLayers = { "VK_LAYER_KHRONOS_validation" };

    std::array<const char*, 1> VulkanDevice::deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    VulkanDevice::VulkanDevice(Window& window):
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
    };

    SnekState VulkanDevice::CreateInstance() 
    {
        if (enableValidationLayers && !SnekVK::CheckValidationLayerSupport(validationLayers.data(), validationLayers.size())) 
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

        SnekVK::LayerAndExtensionInfo info = SnekVK::GetRequiredExtensions(enableValidationLayers);

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
        
        SnekVK::DestroyLayerAndExtensionInfo(info);

        SNEK_ASSERT(SnekVK::HasGlfwInstanceExtensions(enableValidationLayers), "must have GLFW extensions to initialise Vulkan!");

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
            if (SnekVK::IsDeviceSuitable(device, surface, deviceExtensions.data(), deviceExtensions.size()))
            {
                //vkGetPhysicalDeviceProperties(device, &properties);
                //std::cout << "SNEKVK: FOUND DEVICE: " << properties.deviceName << std::endl;
                physicalDevice = device;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE) state = SnekState::Failure;
        
        return state;
    }

    SnekState VulkanDevice::CreateLogicalDevice()
    {
        SnekVK::QueueFamilyIndices indices = SnekVK::FindQueueFamilies(physicalDevice, surface);

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

    SnekState VulkanDevice::CreateCommandPool()
    {
        SnekVK::QueueFamilyIndices queueFamilyIndices = FindPhysicalQueueFamilies();

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
        poolInfo.flags = 
            VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
        {
            return SnekState::Failure;
        }

        return SnekState::Success;
    }

    SnekVK::SwapChainSupportDetails VulkanDevice::GetSwapChainSupport() { return SnekVK::QuerySwapChainSupport(physicalDevice, surface); }

    SnekVK::QueueFamilyIndices VulkanDevice::FindPhysicalQueueFamilies() { return SnekVK::FindQueueFamilies(physicalDevice, surface); };

    void VulkanDevice::DestroyVulkanDevice() 
    {
        vkDestroyCommandPool(device, commandPool, nullptr);
        vkDestroyDevice(device, nullptr);

        if (enableValidationLayers) SnekVK::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);

        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
    }
}
