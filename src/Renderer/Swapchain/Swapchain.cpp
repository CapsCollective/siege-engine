#include "Swapchain.h"

namespace SnekVk
{
    SwapChain::SwapChain(VulkanDevice& device, VkExtent2D windowExtent) 
        : device{device}, windowExtent{windowExtent}
    {
        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
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

    void SwapChain::CreateImageViews()
    {
        swapChainImageViews = new VkImageView[imageCount];

        for (size_t i = 0; i < imageCount; i++)
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = swapChainImageFormat;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            SNEK_ASSERT(vkCreateImageView(device.Device(), &createInfo, nullptr, &swapChainImageViews[i]) == VK_SUCCESS,
                    "Failed to create texture image view!");
        }
    }

    void SwapChain::CreateRenderPass()
    {
        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = FindDepthFormat();
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format = GetSwapChainImageFormat();
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.srcAccessMask = 0;
        dependency.srcStageMask = 
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstSubpass = 0;
        dependency.dstStageMask = 
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = 
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        u32 attachmentCount = 2;
        VkAttachmentDescription attachments[] = {colorAttachment, depthAttachment};
        VkRenderPassCreateInfo renderPassCreateInfo{};
        renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCreateInfo.attachmentCount = attachmentCount;
        renderPassCreateInfo.pAttachments = attachments;
        renderPassCreateInfo.subpassCount = 1;
        renderPassCreateInfo.pSubpasses = &subpass;
        renderPassCreateInfo.dependencyCount = 1;
        renderPassCreateInfo.pDependencies = &dependency;

        SNEK_ASSERT(vkCreateRenderPass(device.Device(), &renderPassCreateInfo, nullptr, &renderPass) == VK_SUCCESS,
            "Failed to create render pass!");
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

    VkFormat SwapChain::FindDepthFormat()
    {
        VkFormat formats[] = {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT};
        return device.FindSupportedFormat(
            formats, 
            3,
            VK_IMAGE_TILING_OPTIMAL, 
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }
}
