#include "Swapchain.h"

#include <array>

namespace SnekVk
{
    SwapChain::SwapChain(VulkanDevice& device, VkExtent2D windowExtent) 
        : device{device}, windowExtent{windowExtent}
    {
        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
        CreateDepthResources();
        CreateFrameBuffers();
        CreateSyncObjects();
    }

    SwapChain::~SwapChain() 
    {
        u32 imageCount = GetImageCount(); 
        for (u32 i = 0; i < imageCount; i++)
        {
            vkDestroyImageView(device.Device(), swapChainImageViews[i], nullptr);
        }

        delete [] swapChainImageViews;

        if (GetSwapChain() != nullptr)
        {
            vkDestroySwapchainKHR(device.Device(), GetSwapChain(), nullptr);
            swapChain = nullptr;
        }

        for (size_t i = 0; i < imageCount; i++)
        {
            vkDestroyImageView(device.Device(), depthImageViews[i], nullptr);
            vkDestroyImage(device.Device(), depthImages[i], nullptr);
            vkFreeMemory(device.Device(), depthImageMemorys[i], nullptr);
        }

        delete [] depthImageViews;
        delete [] depthImages;
        delete [] depthImageMemorys;

        for (size_t i = 0; i < imageCount; i++)
        {
            vkDestroyFramebuffer(device.Device(), swapChainFrameBuffers[i], nullptr);
        }

        delete [] swapChainFrameBuffers;

        vkDestroyRenderPass(device.Device(), renderPass, nullptr);
        
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            vkDestroySemaphore(device.Device(), renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(device.Device(), imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(device.Device(), inFlightFences[i], nullptr);
        }

        delete [] imagesInFlight;
    }

    void SwapChain::CreateSwapChain()
    {
        SwapChainSupportDetails::SwapChainSupportDetails details = device.GetSwapChainSupport();

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
        
        QueueFamilyIndices::QueueFamilyIndices indices = device.FindPhysicalQueueFamilies();
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

        SNEK_ASSERT(vkCreateSwapchainKHR(device.Device(), &createInfo, nullptr, OUT &swapChain) == VK_SUCCESS,
                "Failed to create Swapchain!");

        vkGetSwapchainImagesKHR(device.Device(), swapChain, OUT &imageCount, nullptr);
        swapChainImages = new VkImage[imageCount];
        vkGetSwapchainImagesKHR(device.Device(), swapChain, &imageCount, OUT swapChainImages);

        this->imageCount = imageCount;
        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent;

        SwapChainSupportDetails::DestroySwapChainSupportDetails(details);
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

            SNEK_ASSERT(vkCreateImageView(device.Device(), &createInfo, nullptr, OUT &swapChainImageViews[i]) == VK_SUCCESS,
                    "Failed to create texture image view!");
        }

        std::cout << "Created ImageViews" << std::endl;
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

        SNEK_ASSERT(vkCreateRenderPass(device.Device(), &renderPassCreateInfo, nullptr, OUT &renderPass) == VK_SUCCESS,
            "Failed to create render pass!");
    }

    void SwapChain::CreateDepthResources()
    {
        VkFormat format = FindDepthFormat();
        VkExtent2D swapChainExtent = GetSwapChainExtent();

        depthImages = new VkImage[imageCount];
        depthImageMemorys = new VkDeviceMemory[imageCount];
        depthImageViews = new VkImageView[imageCount];

        for (size_t i = 0; i < imageCount; i++)
        {
            VkImageCreateInfo imageInfo{};
            imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageInfo.imageType = VK_IMAGE_TYPE_2D;
            imageInfo.extent.width = swapChainExtent.width;
            imageInfo.extent.height = swapChainExtent.height;
            imageInfo.extent.depth = 1;
            imageInfo.mipLevels = 1;
            imageInfo.arrayLayers = 1;
            imageInfo.format = format;
            imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            imageInfo.flags = 0;

            device.CreateImageWithInfo(
                imageInfo, 
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                OUT depthImages[i], 
                OUT depthImageMemorys[i]);

            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = depthImages[i];
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = format;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            SNEK_ASSERT(vkCreateImageView(device.Device(), &viewInfo, nullptr, OUT &depthImageViews[i]) == VK_SUCCESS,
                "Failed to create texture image view!");
        }
    }

    void SwapChain::CreateFrameBuffers()
    {
        swapChainFrameBuffers = new VkFramebuffer[imageCount];

        for(size_t i = 0; i < imageCount; i++)
        {
            u32 attachmentCount = 2;

            VkImageView attachments[] {swapChainImageViews[i], depthImageViews[i]};

            VkExtent2D swapChainExtent = GetSwapChainExtent();

            VkFramebufferCreateInfo frameBufferInfo{};
            frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            frameBufferInfo.renderPass = renderPass;
            frameBufferInfo.attachmentCount = attachmentCount;
            frameBufferInfo.pAttachments = attachments;
            frameBufferInfo.width = swapChainExtent.width;
            frameBufferInfo.height = swapChainExtent.height;
            frameBufferInfo.layers = 1;

            SNEK_ASSERT(vkCreateFramebuffer(device.Device(), &frameBufferInfo, nullptr, OUT &swapChainFrameBuffers[i]) == VK_SUCCESS,
                "Failed to create framebuffer");
        }
    }

    void SwapChain::CreateSyncObjects()
    {
        imageAvailableSemaphores = new VkSemaphore[MAX_FRAMES_IN_FLIGHT];
        renderFinishedSemaphores = new VkSemaphore[MAX_FRAMES_IN_FLIGHT];
        inFlightFences = new VkFence[MAX_FRAMES_IN_FLIGHT];
        imagesInFlight = new VkFence[imageCount];

        for (size_t i = 0; i < imageCount; i++) imagesInFlight[i] = VK_NULL_HANDLE;

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            SNEK_ASSERT(
                vkCreateSemaphore(device.Device(), &semaphoreInfo, nullptr, OUT &imageAvailableSemaphores[i]) == VK_SUCCESS &&
                vkCreateSemaphore(device.Device(), &semaphoreInfo, nullptr, OUT &renderFinishedSemaphores[i]) == VK_SUCCESS &&
                vkCreateFence(device.Device(), &fenceInfo, nullptr, OUT &inFlightFences[i]) == VK_SUCCESS, 
                "Failed to create synchronization objects fora  frame!");
        }
    }

    VkResult SwapChain::AcquireNextImage(u32* imageIndex)
    {
        vkWaitForFences(
            device.Device(), 
            1, 
            &inFlightFences[currentFrame], 
            VK_TRUE, 
            std::numeric_limits<u64>::max());

        return vkAcquireNextImageKHR(
            device.Device(),
            swapChain, 
            std::numeric_limits<u64>::max(),
            imageAvailableSemaphores[currentFrame],
            VK_NULL_HANDLE,
            imageIndex
        ); 
    }

    VkResult SwapChain::SubmitCommandBuffers(const VkCommandBuffer* buffers, u32* imageIndex)
    {
        u32 index = *imageIndex;

        if (imagesInFlight[*imageIndex] != VK_NULL_HANDLE)
        {
            vkWaitForFences(device.Device(), 1, &imagesInFlight[index], VK_TRUE, UINT64_MAX);
        }

        imagesInFlight[index] = inFlightFences[currentFrame];

        VkSubmitInfo submitInfo{};
        
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = buffers;

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;
        
        vkResetFences(device.Device(), 1, OUT &inFlightFences[currentFrame]);

        SNEK_ASSERT(vkQueueSubmit(device.GraphicsQueue(), 1, &submitInfo, OUT inFlightFences[currentFrame]) == VK_SUCCESS,
            "Failed to submit draw command buffer");

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] {swapChain};

        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = imageIndex;

        auto result = vkQueuePresentKHR(device.PresentQueue(), &presentInfo);

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

        return result;
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
