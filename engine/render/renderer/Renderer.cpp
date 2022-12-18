//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#define VOLK_IMPLEMENTATION

#include "Renderer.h"

namespace Siege
{
HeapArray<VkCommandBuffer> Renderer::commandBuffers;
VulkanDevice* Renderer::deviceInstance = nullptr;

Renderer::Renderer(Window& window) : window {window}, swapChain {SwapChain(device)}
{
    device.SetWindow(&window);
    swapChain.SetWindowExtents(window.GetExtent());

    if (deviceInstance == nullptr) deviceInstance = &device;

    DescriptorPool::AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10);
    DescriptorPool::AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 10);
    DescriptorPool::AddPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 10);
    DescriptorPool::AddPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 10);

    DescriptorPool::BuildPool();

    Renderer3D::Initialise();
    Renderer2D::Initialise();

    CreateCommandBuffers();
}

Renderer::~Renderer()
{
    CC_LOG_INFO("Destroying renderer")
    DescriptorPool::DestroyPool();
    Renderer3D::DestroyRenderer3D();
}

void Renderer::CreateCommandBuffers()
{
    commandBuffers = HeapArray<VkCommandBuffer>(SwapChain::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = device.GetCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.Size());

    CC_ASSERT(vkAllocateCommandBuffers(device.Device(), &allocInfo, OUT commandBuffers.Data()) ==
                  VK_SUCCESS,
              "Failed to allocate command buffer");
}

void Renderer::DrawFrame()
{
    auto commandBuffer = GetCurrentCommandBuffer();

    Renderer2D::GlobalData global2DData = {projection};

    Renderer3D::Render(commandBuffer, projection);
    Renderer2D::Render(commandBuffer, global2DData);
}

void Renderer::RecreateSwapChain()
{
    ClearDeviceQueue();
    auto extent = window.GetExtent();
    while (extent.width == 0 || extent.height == 0)
    {
        extent = window.GetExtent();
        window.WaitEvents();
    }

    auto oldImageFormat = swapChain.GetImageFormat();
    auto oldDepthFormat = swapChain.GetDepthFormat();

    // Re-create swapchain
    swapChain.RecreateSwapchain();

    // Re-create the pipeline once the swapchain renderpass
    // becomes available again.
    if (!swapChain.CompareSwapFormats(oldImageFormat, oldDepthFormat))
    {
        Renderer3D::RecreateMaterials();
        Renderer2D::RecreateMaterials();
    }
}

bool Renderer::StartFrame()
{
    CC_ASSERT(!isFrameStarted, "Can't start a frame when a frame is already in progress!");

    auto result = swapChain.AcquireNextImage(&currentImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        RecreateSwapChain();
        return false;
    }

    CC_ASSERT(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR,
              "Failed to acquire swapchain image!");

    isFrameStarted = true;

    VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();

    VkCommandBufferBeginInfo beginInfo {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    CC_ASSERT(vkBeginCommandBuffer(OUT commandBuffer, &beginInfo) == VK_SUCCESS,
              "Failed to begin recording command buffer");

    BeginSwapChainRenderPass(commandBuffer);

    return true;
}

void Renderer::EndFrame()
{
    CC_ASSERT(isFrameStarted, "Can't end frame while frame is not in progress!");

    DrawFrame();

    VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();

    EndSwapChainRenderPass(commandBuffer);

    CC_ASSERT(vkEndCommandBuffer(OUT commandBuffer) == VK_SUCCESS,
              "Failed to record command buffer!");

    auto result = swapChain.SubmitCommandBuffers(&commandBuffer, &currentImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.WasResized())
    {
        window.ResetWindowResized();
        RecreateSwapChain();
    }
    else if (result != VK_SUCCESS)
    {
        CC_ASSERT(result == VK_SUCCESS, "Failed to submit command buffer for drawing!");
    }

    isFrameStarted = false;
    currentFrameIndex = (currentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;

    Renderer3D::Flush();
    Renderer2D::Flush();
}

void Renderer::BeginSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
    CC_ASSERT(isFrameStarted, "Can't start render pass while the frame hasn't started!");
    CC_ASSERT(commandBuffer == GetCurrentCommandBuffer(),
              "Can't begin a render pass on a command buffer from another frame!");

    uint32_t clearValueCount = 2;
    VkClearValue clearValues[clearValueCount];
    clearValues[0].color = clearValue;
    clearValues[1].depthStencil = {1.0f, 0};

    RenderPass::Begin(swapChain.GetRenderPass()->GetRenderPass(),
                      OUT commandBuffer,
                      swapChain.GetFrameBuffer(currentImageIndex),
                      {0, 0},
                      swapChain.GetSwapChainExtent(),
                      clearValues,
                      clearValueCount);

    VkViewport viewport {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapChain.GetSwapChainExtent().width);
    viewport.height = static_cast<float>(swapChain.GetSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor {{0, 0}, swapChain.GetSwapChainExtent()};

    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void Renderer::EndSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
    CC_ASSERT(isFrameStarted, "Can't end render pass while the frame hasn't started!");
    CC_ASSERT(commandBuffer == GetCurrentCommandBuffer(),
              "Can't begin a render pass on a command buffer from another frame!");

    RenderPass::End(commandBuffer);
}
} // namespace Siege
