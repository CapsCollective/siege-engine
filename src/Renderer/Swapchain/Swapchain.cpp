#include "Swapchain.h"

namespace SnekVk
{
    SwapChain::SwapChain(VulkanDevice& device, VkExtent2D windowExtent) 
        : device{device}, windowExtent{windowExtent}
    {
        CreateSwapChain();
    }

    void SwapChain::CreateSwapChain()
    {
        SwapChainSupportDetails details = device.GetSwapChainSupport();

        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(details.formats, details.availableFormatCount);
        VkPresentModeKHR presentMode = ChoosePresentMode(details.presentModes, details.availablePresentModeCount);
        VkExtent2D extent = ChooseSwapExtent(details.capabilities);

        std::cout << "Extent: " << extent.width << "x" << extent.height << std::endl;

        u32 imageCount = details.capabilities.minImageCount + 1;

        if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount)
        {
            imageCount = details.capabilities.maxImageCount;
        }   
        std::cout << "Image Count: " << imageCount << std::endl;

        VkSwapchainCreateInfoKHR createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = device.Surface();
        
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        
        QueueFamilyIndices indices = device.findPhysicalQueueFamilies();
        u32 queueFamilyIndices[] = {indices.graphicsFamily, indices.presentFamily};

        if (indices.graphicsFamily != indices.presentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else 
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform = details.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        SNEK_ASSERT(vkCreateSwapchainKHR(device.Device(), &createInfo, nullptr, &swapChain) == VK_SUCCESS,
                "Failed to create Swapchain!");

        vkGetSwapchainImagesKHR(device.Device(), swapChain, &imageCount, nullptr);
        swapChainImages = new VkImage[imageCount];
        vkGetSwapchainImagesKHR(device.Device(), swapChain, &imageCount, swapChainImages);

        this->imageCount = imageCount;
        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent;
    }

    // Helpers

    VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(VkSurfaceFormatKHR* formats, size_t formatCount)
    {
        for (size_t i = 0; i < formatCount; i++)
        {
            VkSurfaceFormatKHR& availableFormat = formats[i];
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && 
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) return availableFormat;
        }
        
        return formats[0];
    }

    VkPresentModeKHR SwapChain::ChoosePresentMode(VkPresentModeKHR* presentModes, size_t presentModeCount)
    {
        for (size_t i = 0; i < presentModeCount; i++)
        {
            VkPresentModeKHR& availablePresentMode = presentModes[i];
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                std::cout << "Present Mode: Mailbox" << std::endl;
                return availablePresentMode;
            }
        }

        std::cout << "Present Mode: V-Sync" << std::endl;
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SwapChain::ChooseSwapExtent(VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<u32>::max()) return capabilities.currentExtent;
        else 
        {
            VkExtent2D actualExtent = windowExtent;
            actualExtent.width = std::max(
                capabilities.minImageExtent.width,
                std::min(capabilities.maxImageExtent.width, actualExtent.width)
            );
            actualExtent.height = std::max(
                capabilities.minImageExtent.height,
                std::min(capabilities.maxImageExtent.height, actualExtent.height)
            );
            return actualExtent;
        }
    }
}
