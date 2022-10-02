//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Swapchain.h"

#include "Context.h"
#include "utils/Device.h"
#include "utils/Swapchain.h"
#include "utils/TypeAdaptor.h"

namespace Siege::Vulkan
{
Swapchain::Swapchain(const Utils::Extent2D& imageExtent, VkSwapchainKHR oldSwapchain) :
    windowExtent {imageExtent}
{
    CreateSwapChain(oldSwapchain);
    CreateRenderPass();
    CreateDepthResources();
    CreateFrameBuffers();
    CreateSyncObjects();
}

Swapchain::Swapchain(Swapchain&& other)
{
    Swap(other);
}

Swapchain& Swapchain::operator=(Swapchain&& other)
{
    Swap(other);
    return *this;
}

void Swapchain::CreateSwapChain(VkSwapchainKHR oldSwapchain)
{
    auto physicalDevice = Vulkan::Context::GetPhysicalDevice();
    auto surface = Vulkan::Context::GetSurface();

    auto capabilities =
        Vulkan::Device::Physical::GetSurfaceCapabilities(physicalDevice->GetDevice(), surface);

    // Get our supported colour format
    auto surfaceFormat = Vulkan::Utils::ChooseSwapSurfaceFormat(
        Vulkan::Device::Physical::GetSurfaceFormats(physicalDevice->GetDevice(), surface));

    // The size of our images.
    VkExtent2D extent =
        Vulkan::Utils::ChooseSwapExtent(capabilities, Utils::ToVkExtent2D(windowExtent));

    // Get the image count we can support
    uint32_t imageCount = capabilities.minImageCount + 1;

    // Make sure we aren't exceeding the GPU's image count maximums
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
    {
        imageCount = capabilities.maxImageCount;
    }

    swapchain = Utils::SwapchainBuilder()
                    .WithSurface(surface)
                    .WithMinImageCount(imageCount)
                    .WithSurfaceFormat(surfaceFormat)
                    .WithImageExtent(extent)
                    .WithQueueFamilyIndices(
                        {
                            physicalDevice->GetGraphicsFamilyQueueIndex(),
                            physicalDevice->GetPresentFamilyQueueIndex()
                        })
                    .WithPreTransform(capabilities.currentTransform)
                    .WithCompositeAlpha(VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
                    .WithPresentMode(Vulkan::Utils::ChoosePresentMode(
                        Vulkan::Device::Physical::GetPresentModes(physicalDevice->GetDevice(),
                                                                  surface)))
                    .IsClipped(VK_TRUE)
                    .FromOldSwapchain(oldSwapchain)
                    .Build();

    swapchainImages = FrameImages(swapchain,
                                  {extent.width, extent.height, 1},
                                  Vulkan::Utils::ToImageFormat(surfaceFormat.format));

    swapchainExtent = Utils::ToExtent2D(extent);

    CC_LOG_INFO("[SWAPCHAIN] Created with extent: [{}x{}]", extent.width, extent.height)
}

Swapchain::~Swapchain()
{
    auto device = Context::GetVkLogicalDevice();
    uint32_t imageCount = FrameImages::GetImageCount();

    if (device)
    {
        CC_LOG_INFO("Clearing Swapchain")
        vkDestroySwapchainKHR(device, swapchain, nullptr);
    }

    if (swapChainFrameBuffers)
    {
        for (size_t i = 0; i < imageCount; i++)
        {
            vkDestroyFramebuffer(device, swapChainFrameBuffers[i], nullptr);
            swapChainFrameBuffers[i] = VK_NULL_HANDLE;
        }
    }

    if (inFlightFences)
    {
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            vkDestroyFence(device, inFlightFences[i], nullptr);
        }
    }

    delete[] imagesInFlight;
    delete[] swapChainFrameBuffers;
    delete[] inFlightFences;

    imagesInFlight = VK_NULL_HANDLE;
    swapChainFrameBuffers = VK_NULL_HANDLE;
    inFlightFences = VK_NULL_HANDLE;
    swapchain = VK_NULL_HANDLE;
}

VkResult Swapchain::AcquireNextImage(uint32_t* imageIndex)
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
                                 swapchain,
                                 std::numeric_limits<uint64_t>::max(),
                                 imageAvailableSemaphores[currentFrame],
                                 VK_NULL_HANDLE,
                                 imageIndex);
}

VkResult Swapchain::SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex)
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

    // Set a stage that you need to wait for. In this case we wait until the colour stage of the
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
    VkSwapchainKHR swapChains[] {swapchain};

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

bool Swapchain::IsSameSwapFormat(Utils::ImageFormat oldImageFormat,
                                 Utils::DepthFormat oldDepthFormat)
{
    return oldImageFormat == swapChainImageFormat && oldDepthFormat == swapChainDepthFormat;
}

void Swapchain::CreateRenderPass()
{
    swapChainImageFormat = swapchainImages.GetFormat();
    swapChainDepthFormat = Vulkan::Context::Get().GetPhysicalDevice()->GetDepthFormat();
    RenderPass::Initialise(
        OUT renderPass,
        RenderPass::CreateConfig()
            .WithAttachment(
                Attachments::CreateColorAttachment(Vulkan::Utils::ToVkFormat(swapChainImageFormat)))
            .WithAttachment(
                Attachments::CreateDepthAttachment(Vulkan::Utils::ToVkFormat(swapChainDepthFormat)))
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

void Swapchain::CreateDepthResources()
{
    // Initialise our depth image information.
    auto format = static_cast<Utils::ImageFormat>(swapChainDepthFormat);
    depthImages = FrameImages({swapchainExtent.width, swapchainExtent.height, 1}, format);
}

void Swapchain::CreateFrameBuffers()
{
    uint32_t imageCount = FrameImages::GetImageCount();

    // Initialise the framebuffers storage
    if (swapChainFrameBuffers == nullptr) swapChainFrameBuffers = new VkFramebuffer[imageCount];

    // We need a separate frame buffer for each image that we want to draw
    for (size_t i = 0; i < imageCount; i++)
    {
        // We have two sets of image views we need to render images with
        uint32_t attachmentCount = 2;

        auto& colourImage = swapchainImages.GetVkImage(i);
        auto& depthImage = depthImages.GetVkImage(i);

        VkImageView attachments[] {colourImage.GetView(), depthImage.GetView()};

        // Get our extents
        Utils::Extent2D swapChainExtent = swapchainExtent;

        // Populate the creation struct
        VkFramebufferCreateInfo frameBufferInfo {};
        frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferInfo.renderPass = renderPass.GetRenderPass();
        frameBufferInfo.attachmentCount = attachmentCount;
        frameBufferInfo.pAttachments = attachments;
        frameBufferInfo.width = swapChainExtent.width;
        frameBufferInfo.height = swapChainExtent.height;
        frameBufferInfo.layers = 1;

        CC_ASSERT(vkCreateFramebuffer(Vulkan::Context::GetVkLogicalDevice(),
                                      &frameBufferInfo,
                                      nullptr,
                                      OUT & swapChainFrameBuffers[i]) == VK_SUCCESS,
                  "Failed to create framebuffer")
    }
}

void Swapchain::CreateSyncObjects()
{
    auto device = Vulkan::Context::GetVkLogicalDevice();
    uint32_t imageCount = FrameImages::GetImageCount();

    imageAvailableSemaphores = Semaphore(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores = Semaphore(MAX_FRAMES_IN_FLIGHT);

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
        CC_ASSERT(vkCreateFence(device, &fenceInfo, nullptr, OUT & inFlightFences[i]) == VK_SUCCESS,
            "Failed to create synchronization objects for a frame!");
    }
}

void Swapchain::Swap(Swapchain& other)
{
    auto tmpWindowExtent = windowExtent;
    auto tmpSwapchainExtent = swapchainExtent;
    auto tmpSwapchain = swapchain;
    auto tmpSwapchainImages = std::move(swapchainImages);
    auto tmpSwapchainImageFormat = swapChainImageFormat;
    auto tmpSwapchainDepthFormat = swapChainDepthFormat;
    auto tmpRenderPass = std::move(renderPass);
    auto tmpDepthImages = std::move(depthImages);
    auto tmpFrameBuffers = swapChainFrameBuffers;
    auto tmpInFlightFences = inFlightFences;
    auto tmpImagesInFlight = imagesInFlight;
    auto tmpCurrentFrame = currentFrame;
    auto tmpImageAvailableSemaphores2 = std::move(imageAvailableSemaphores);
    auto tmpRenderFinishedSemaphores2 = std::move(renderFinishedSemaphores);

    windowExtent = other.windowExtent;
    swapchain = other.swapchain;
    swapchainExtent = other.swapchainExtent;
    swapchainImages = std::move(other.swapchainImages);
    swapChainImageFormat = other.swapChainImageFormat;
    swapChainDepthFormat = other.swapChainDepthFormat;
    renderPass = std::move(other.renderPass);
    depthImages = std::move(other.depthImages);
    swapChainFrameBuffers = other.swapChainFrameBuffers;
    inFlightFences = other.inFlightFences;
    imagesInFlight = other.imagesInFlight;
    currentFrame = other.currentFrame;
    imageAvailableSemaphores = std::move(other.imageAvailableSemaphores);
    renderFinishedSemaphores = std::move(other.renderFinishedSemaphores);


    other.windowExtent = tmpWindowExtent;
    other.swapchainExtent = tmpSwapchainExtent;
    other.swapchain = tmpSwapchain;
    other.swapchainImages = std::move(tmpSwapchainImages);
    other.swapChainImageFormat = tmpSwapchainImageFormat;
    other.swapChainDepthFormat = tmpSwapchainDepthFormat;
    other.renderPass = std::move(tmpRenderPass);
    other.depthImages = std::move(tmpDepthImages);
    other.swapChainFrameBuffers = tmpFrameBuffers;
    other.inFlightFences = tmpInFlightFences;
    other.imagesInFlight = tmpImagesInFlight;
    other.currentFrame = tmpCurrentFrame;
    other.imageAvailableSemaphores = std::move(tmpImageAvailableSemaphores2);
    other.renderFinishedSemaphores = std::move(tmpRenderFinishedSemaphores2);
}
} // namespace Siege::Vulkan
