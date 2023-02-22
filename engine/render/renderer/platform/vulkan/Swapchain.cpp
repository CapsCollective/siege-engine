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
#include "utils/SubmitCommands.h"
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

    swapchain =
        Utils::SwapchainBuilder()
            .WithSurface(surface)
            .WithMinImageCount(imageCount)
            .WithSurfaceFormat(surfaceFormat)
            .WithImageExtent(extent)
            .WithQueueFamilyIndices({physicalDevice->GetGraphicsFamilyQueueIndex(),
                                     physicalDevice->GetPresentFamilyQueueIndex()})
            .WithPreTransform(capabilities.currentTransform)
            .WithCompositeAlpha(VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
            .WithPresentMode(Vulkan::Utils::ChoosePresentMode(
                Vulkan::Device::Physical::GetPresentModes(physicalDevice->GetDevice(), surface)))
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

    if (device == nullptr) return;

    uint32_t imageCount = FrameImages::GetImageCount();

    CC_LOG_INFO("Clearing Swapchain")
    vkDestroySwapchainKHR(device, swapchain, nullptr);

    if (swapChainFrameBuffers)
    {
        for (size_t i = 0; i < imageCount; i++)
        {
            vkDestroyFramebuffer(device, swapChainFrameBuffers[i], nullptr);
            swapChainFrameBuffers[i] = VK_NULL_HANDLE;
        }
    }

    delete[] swapChainFrameBuffers;

    swapChainFrameBuffers = VK_NULL_HANDLE;
    swapchain = VK_NULL_HANDLE;
}

Utils::Result Swapchain::AcquireNextImage(uint32_t* imageIndex)
{
    auto device = Vulkan::Context::GetVkLogicalDevice();

    // Wait for the image of the current frame to become available
    inFlightFences.Wait(currentFrame);

    // Once available, Add it to our available images semaphor for usage
    return static_cast<Utils::Result>(vkAcquireNextImageKHR(device,
                                                            swapchain,
                                                            std::numeric_limits<uint64_t>::max(),
                                                            imageAvailableSemaphores[currentFrame],
                                                            VK_NULL_HANDLE,
                                                            imageIndex));
}

void Swapchain::BeginRenderPass(VkCommandBuffer commandBuffer,
                                uint32_t imageIndex,
                                std::initializer_list<VkClearValue> clearValues)
{
    RenderPass::Begin(renderPass.GetRenderPass(),
                      OUT commandBuffer,
                      swapChainFrameBuffers[imageIndex],
                      {0, 0},
                      {swapchainExtent.width, swapchainExtent.height},
                      std::data(clearValues),
                      clearValues.size());
}

void Swapchain::EndRenderPass(VkCommandBuffer commandBuffer)
{
    vkCmdEndRenderPass(OUT commandBuffer);
}

Utils::Result Swapchain::SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex)
{
    uint32_t index = *imageIndex;
    uint32_t frameIdx = currentFrame;

    // If the image being asked for is being used, we wait for it to become available
    imagesInFlight.Wait(index);

    // Get the frame's image and move it to our images in flight
    imagesInFlight.Set(index, inFlightFences[currentFrame]);

    inFlightFences.Reset(currentFrame);

    Utils::SubmitGraphicsCommand()
        .ToQueue(Vulkan::Context::GetCurrentDevice()->GetGraphicsQueue())
        .WaitOnSemaphores({imageAvailableSemaphores[currentFrame]})
        .WithPipelineStages({VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT})
        .WithCommandBuffers({buffers[0]})
        .SignalSemaphores({renderFinishedSemaphores[currentFrame]})
        .Submit(inFlightFences[currentFrame]);

    currentFrame = ((currentFrame + 1) < MAX_FRAMES_IN_FLIGHT) * (currentFrame + 1);

    // Return the result of the rendering process
    return Utils::SubmitPresentCommand()
        .ToQueue(Vulkan::Context::GetCurrentDevice()->GetPresentQueue())
        .SignalSemaphores({renderFinishedSemaphores[frameIdx]})
        .ForSwapchains({swapchain})
        .WithImageIndices({index})
        .Submit();
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
    uint32_t imageCount = FrameImages::GetImageCount();

    imageAvailableSemaphores = Semaphore(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores = Semaphore(MAX_FRAMES_IN_FLIGHT);

    inFlightFences = Fence(MAX_FRAMES_IN_FLIGHT);
    imagesInFlight = Fence(imageCount, Fence::FENCE_NULL);
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
    auto tmpInFlightFences = std::move(inFlightFences);
    auto tmpImagesInFlight = std::move(imagesInFlight);
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
    inFlightFences = std::move(other.inFlightFences);
    imagesInFlight = std::move(other.imagesInFlight);
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
    other.inFlightFences = std::move(tmpInFlightFences);
    other.imagesInFlight = std::move(tmpImagesInFlight);
    other.currentFrame = tmpCurrentFrame;
    other.imageAvailableSemaphores = std::move(tmpImageAvailableSemaphores2);
    other.renderFinishedSemaphores = std::move(tmpRenderFinishedSemaphores2);
}
} // namespace Siege::Vulkan