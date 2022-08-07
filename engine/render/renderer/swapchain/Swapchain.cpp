//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Swapchain.h"

#include "render/renderer/platform/vulkan/Context.h"
#include "render/renderer/platform/vulkan/utils/TypeAdaptor.h"
#include "render/renderer/platform/vulkan/utils/Device.h"

#include <utils/Logging.h>

namespace Siege
{
// TODO: Fix the warnings
SwapChain* SwapChain::instance = nullptr;

SwapChain::SwapChain(VkExtent2D windowExtent) : windowExtent {windowExtent}
{
    Init();
}

SwapChain::SwapChain() {}

SwapChain::~SwapChain()
{
    ClearSwapChain();
    ClearMemory();
}

// TODO: Is there a better way to handle swapchain creation?
void SwapChain::SetWindowExtents(VkExtent2D windowExtent)
{
    this->windowExtent = windowExtent;
    Init();
}

void SwapChain::ClearSwapChain(bool isRecreated)
{
    uint32_t imageCount = FrameImages::GetImageCount();

    auto device = Vulkan::Context::GetVkLogicalDevice();

    if (!isRecreated && swapChain != nullptr)
    {
        CC_LOG_INFO("Clearing Swapchain")
        vkDestroySwapchainKHR(device, GetSwapChain(), nullptr);
        swapChain = nullptr;
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(device, inFlightFences[i], nullptr);
    }
}

VkFormat SwapChain::GetSwapChainImageFormat()
{
    return Vulkan::Utils::ToVkFormat(swapchainImages.GetFormat());
}

void SwapChain::ClearMemory()
{
    // De-allocate arrays

    delete[] imagesInFlight;

    imagesInFlight = nullptr;
}

void SwapChain::RecreateSwapchain()
{
    CC_LOG_INFO("Re-creating Swapchain")
    // Destroy all Vulkan structs
    ClearSwapChain(true);

    // Re-create all Vulkan structs
    Init();
}

bool SwapChain::CompareSwapFormats(VkFormat oldImageFormat, VkFormat oldDepthFormat)
{
    return oldImageFormat == swapChainImageFormat && oldDepthFormat == swapChainDepthFormat;
}

void SwapChain::Init()
{
    CreateSwapChain();
    CreateImageViews();
    CreateRenderPass();
    CreateDepthResources();
    CreateFrameBuffers();
    CreateSyncObjects();

    if (instance == nullptr) instance = this;
}

// TODO: Clean this function
void SwapChain::CreateSwapChain()
{
    auto physicalDevice = Vulkan::Context::GetPhysicalDevice();
    auto surface = Vulkan::Context::GetInstance().GetSurface();
    auto device = Vulkan::Context::GetVkLogicalDevice();

    // Get our swapchain details
    auto formats = Vulkan::Device::Physical::GetSurfaceFormats(physicalDevice->GetDevice(),
                                                               surface);

    auto presentModes = Vulkan::Device::Physical::GetPresentModes(physicalDevice->GetDevice(),
                                                                  surface);

    auto capabilities =
        Vulkan::Device::Physical::GetSurfaceCapabilities(physicalDevice->GetDevice(), surface);

    // Get our supported colour format
    auto surfaceFormat =
        ChooseSwapSurfaceFormat(formats.Data(), static_cast<uint32_t>(formats.Size()));

    // Choose our presentation mode (the form of image buffering)
    auto presentMode =
        ChoosePresentMode(presentModes.Data(), static_cast<uint32_t>(presentModes.Size()));

    // The size of our images.
    VkExtent2D extent = ChooseSwapExtent(capabilities);

    CC_LOG_INFO("Swapchain created with image extents: [{}x{}]", extent.width, extent.height)

    // Get the image count we can support
    uint32_t imageCount = capabilities.minImageCount + 1;

    // Make sure we aren't exceeding the GPU's image count maximums
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
    {
        imageCount = capabilities.maxImageCount;
    }

    CC_LOG_INFO("Images supported by swapchain: {}", imageCount)

    // Now we populate the base swapchain creation struct
    VkSwapchainCreateInfoKHR createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = Vulkan::Context::GetInstance().GetSurface();

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t graphicsFamily = physicalDevice->GetGraphicsFamilyQueueIndex();
    uint32_t presentFamily = physicalDevice->GetPresentFamilyQueueIndex();

    uint32_t queueFamilyIndices[] = {graphicsFamily, presentFamily};

    // Sometimes the graphics and presentation queues are the same. We want to check for this
    // eventuality.
    if (graphicsFamily != presentFamily)
    {
        // We specify that there are two distinct queues that need to be used
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        // Indicates that there is only a single queue to be used
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    // Indicates any pre-transforms that need to occur to images (default is none)
    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = swapChain ? swapChain : VK_NULL_HANDLE;

    CC_ASSERT(vkCreateSwapchainKHR(device, &createInfo, nullptr, OUT & swapChain) == VK_SUCCESS,
              "Failed to create Swapchain!");

    swapchainImages = FrameImages(swapChain,
                                  {extent.width, extent.height, 1},
                                  Vulkan::Utils::ToImageFormat(surfaceFormat.format));

    swapChainExtent = extent;
}

void SwapChain::CreateImageViews()
{}

void SwapChain::CreateRenderPass()
{
    swapChainImageFormat = GetSwapChainImageFormat();
    swapChainDepthFormat = FindDepthFormat();
    renderPass = RenderPass(
        RenderPass::CreateConfig()
            .WithAttachment(Attachments::CreateColorAttachment(GetSwapChainImageFormat()))
            .WithAttachment(Attachments::CreateDepthAttachment(FindDepthFormat()))
            .WithSubPass(
                Attachments::CreateSubPass()
                    .WithColorReference(Attachments::CreateColorAttachmentReference(0))
                    .WithDepthReference(Attachments::CreateDepthStencilAttachmentReference(1))
                    .BuildGraphicsSubPass())
            .WithDependency(Attachments::CreateSubPassDependency()
                                .WithSrcSubPass(VK_SUBPASS_EXTERNAL)
                                .WithDstSubPass(0)
                                .WithSrcStageMask(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                                                  VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT)
                                .WithDstStageMask(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                                                  VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT)
                                .WithSrcAccessMask(0)
                                .WithDstAccessMask(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                                                   VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT)
                                .Build()));
}

void SwapChain::CreateDepthResources()
{
    // Start by getting our depth information
    VkExtent2D extent = GetSwapChainExtent();

    // Initialise our depth image information.
    depthImages = FrameImages({extent.width, extent.height, 1}, Vulkan::Utils::ToImageFormat(swapChainDepthFormat));
}

void SwapChain::CreateFrameBuffers()
{
    VkExtent2D extent = GetSwapChainExtent();

    Framebuffer::SetImageCount(FrameImages::GetImageCount());

    // Create a set of frame buffers to begin with.
    framebuffers = Framebuffer(Framebuffer::Config()
                                   .WithRenderPass(renderPass.GetRenderPass())
                                   .WithColorAttachments(&swapchainImages)
                                   .WithDepthAttachments(&depthImages)
                                   .WithImageDimensions(extent.width, extent.height)
                                   .WithLayers(1),
                               Vulkan::Context::GetVkLogicalDevice());
}

// TODO(Aryeh): See if this logic can be encapsulated in an object/s
void SwapChain::CreateSyncObjects()
{
    auto device = Vulkan::Context::GetVkLogicalDevice();
    uint32_t imageCount = FrameImages::GetImageCount();

    if (imageAvailableSemaphores == nullptr)
        imageAvailableSemaphores = new VkSemaphore[MAX_FRAMES_IN_FLIGHT];
    if (renderFinishedSemaphores == nullptr)
        renderFinishedSemaphores = new VkSemaphore[MAX_FRAMES_IN_FLIGHT];
    if (inFlightFences == nullptr) inFlightFences = new VkFence[MAX_FRAMES_IN_FLIGHT];
    if (imagesInFlight == nullptr) imagesInFlight = new VkFence[imageCount];

    // Set all images in flight to null
    for (size_t i = 0; i < imageCount; i++) imagesInFlight[i] = VK_NULL_HANDLE;

    // Create our semaphore and fence create info
    VkSemaphoreCreateInfo semaphoreInfo {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    // Create the synchronisation objects
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        CC_ASSERT(
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, OUT & imageAvailableSemaphores[i]) ==
                    VK_SUCCESS &&
                vkCreateSemaphore(device,
                                  &semaphoreInfo,
                                  nullptr,
                                  OUT & renderFinishedSemaphores[i]) == VK_SUCCESS &&
                vkCreateFence(device, &fenceInfo, nullptr, OUT & inFlightFences[i]) == VK_SUCCESS,
            "Failed to create synchronization objects fora  frame!");
    }
}

VkResult SwapChain::AcquireNextImage(uint32_t* imageIndex)
{
    auto device = Vulkan::Context::GetVkLogicalDevice();

    // Wait for the image of the current frame to become available
    vkWaitForFences(device,
                    1,
                    &inFlightFences[currentFrame],
                    VK_TRUE,
                    std::numeric_limits<uint64_t>::max());

    // Once available, Add it to our available images semaphor for usage
    return vkAcquireNextImageKHR(device,
                                 swapChain,
                                 std::numeric_limits<uint64_t>::max(),
                                 imageAvailableSemaphores[currentFrame],
                                 VK_NULL_HANDLE,
                                 imageIndex);
}

VkResult SwapChain::SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex)
{
    auto device = Vulkan::Context::GetVkLogicalDevice();

    uint32_t index = *imageIndex;

    // If the image being asked for is being used, we wait for it to become available
    if (imagesInFlight[index] != VK_NULL_HANDLE)
    {
        vkWaitForFences(device, 1, &imagesInFlight[index], VK_TRUE, UINT64_MAX);
    }

    // Get the frame's image and move it to our images in flight
    imagesInFlight[index] = inFlightFences[currentFrame];

    // With our submission, we specify a semaphore to wait on to grab data from and one to
    // signal when the rendering is complete.

    VkSubmitInfo submitInfo {};

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    // Specify the semaphores we want to wait for (the one for our current frame)
    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};

    // Set a stage that you need to wait for. In this case we wait until the color stage of the
    // pipeline is done (fragment stage)
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = buffers;

    // Specify some semaphores to be signalled when rendering is done
    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    // Reset the fence of this frame
    vkResetFences(device, 1, OUT & inFlightFences[currentFrame]);

    auto logicalDevice = Vulkan::Context::GetCurrentDevice();

    // Submit the command buffer to the graphics queue
    CC_ASSERT(vkQueueSubmit(logicalDevice->GetGraphicsQueue(),
                            1,
                            &submitInfo,
                            OUT inFlightFences[currentFrame]) == VK_SUCCESS,
              "Failed to submit draw command buffer");

    // Set up our presentation information and the semaphores to wait on
    VkPresentInfoKHR presentInfo {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    // Specify the swapchains we want to use (currently only using one)
    VkSwapchainKHR swapChains[] {swapChain};

    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    // Store the image we want to render
    presentInfo.pImageIndices = imageIndex;

    // Submit the presentation info to the present queue.
    auto result = vkQueuePresentKHR(logicalDevice->GetPresentQueue(), &presentInfo);

    // Set the frame to the next frame
    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

    // Return the result of the rendering process
    return result;
}

// Helpers

VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(VkSurfaceFormatKHR* formats,
                                                      size_t formatCount)
{
    // Ideally, we want to support colors in 4 dimensional vectors (R, G, B, A) in SRGB color space.
    for (size_t i = 0; i < formatCount; i++)
    {
        VkSurfaceFormatKHR availableFormat = formats[i];
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return formats[i];
    }
    // If we can't find what we want then we use whatever is available on the GPU.
    return formats[0];
}

VkPresentModeKHR SwapChain::ChoosePresentMode(VkPresentModeKHR* presentModes,
                                              size_t presentModeCount)
{
    for (size_t i = 0; i < presentModeCount; i++)
    {
        VkPresentModeKHR availablePresentMode = presentModes[i];

        // Ideally, we want to support triple buffering since it has the best
        // balance between performance and image quality
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            CC_LOG_INFO("Present Mode: Mailbox")
            return presentModes[i];
        }
    }

    // If triple buffering is not available then use v-sync
    CC_LOG_INFO("Present Mode: V-Sync")
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::ChooseSwapExtent(VkSurfaceCapabilitiesKHR& capabilities)
{
    // We want to make sure that the extents provided are within a reasonable range.
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        return capabilities.currentExtent;
    else
    {
        // Otherwise get the smaller extent between the window and the swapchain image
        // extents and return that.
        VkExtent2D actualExtent = windowExtent;
        actualExtent.width =
            std::max(capabilities.minImageExtent.width,
                     std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height =
            std::max(capabilities.minImageExtent.height,
                     std::min(capabilities.maxImageExtent.height, actualExtent.height));
        return actualExtent;
    }
}

VkFormat SwapChain::FindDepthFormat()
{
    return Vulkan::Utils::ToVkFormat(Vulkan::Context::Get().GetPhysicalDevice()->GetDepthFormat());
}
} // namespace Siege
