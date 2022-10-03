//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#define VOLK_IMPLEMENTATION

#include "Renderer.h"

#include "platform/vulkan/utils/Types.h"

namespace Siege
{
Renderer* Renderer::instance {nullptr};
Utils::MHArray<VkCommandBuffer> Renderer::commandBuffers;

Renderer::Renderer(Window& window) : window {window}
{
    if (instance == nullptr) instance = this;

    auto extent = window.GetExtent();

    context.Init({extent.width, extent.height},
                 Window::GetRequiredExtensions,
                 Window::CreateWindowSurface);

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
    commandBuffers = Utils::MHArray<VkCommandBuffer>(Vulkan::Swapchain::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = Vulkan::Context::GetCurrentDevice()->GetCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.Size());

    CC_ASSERT(vkAllocateCommandBuffers(Vulkan::Context::GetVkLogicalDevice(),
                                       &allocInfo,
                                       OUT commandBuffers.Data()) == VK_SUCCESS,
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

    auto& swapchain = context.GetSwapchain();

    auto oldImageFormat = swapchain.GetImageFormat();
    auto oldDepthFormat = swapchain.GetDepthFormat();

    context.RecreateSwapchain({extent.width, extent.height});

    // Re-create the pipeline once the swapchain renderpass
    // becomes available again.
    if (!swapchain.IsSameSwapFormat(oldImageFormat, oldDepthFormat))
    {
        Renderer3D::RecreateMaterials();
        Renderer2D::RecreateMaterials();
    }
}

bool Renderer::StartFrame()
{
    CC_ASSERT(!isFrameStarted, "Can't start a frame when a frame is already in progress!");

    auto& swapchain = context.GetSwapchain();

    auto result = swapchain.AcquireNextImage(&currentImageIndex);

    if (result == Vulkan::Utils::ERROR_OUT_OF_DATE)
    {
        RecreateSwapChain();
        return false;
    }

    CC_ASSERT(result == Vulkan::Utils::SUCCESS || result == Vulkan::Utils::SUBOPTIMAL,
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
    CC_ASSERT(isFrameStarted, "Can't end frame while frame is not in progress!")

    auto& swapchain = context.GetSwapchain();

    DrawFrame();

    VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();

    EndSwapChainRenderPass(commandBuffer);

    CC_ASSERT(vkEndCommandBuffer(OUT commandBuffer) == VK_SUCCESS,
              "Failed to record command buffer!");

    auto result = swapchain.SubmitCommandBuffers(&commandBuffer, &currentImageIndex);

    if (result == Vulkan::Utils::ERROR_RESIZED || window.WasResized())
    {
        window.ResetWindowResized();
        RecreateSwapChain();
    }

    isFrameStarted = false;
    currentFrameIndex = (currentFrameIndex + 1) % Vulkan::Swapchain::MAX_FRAMES_IN_FLIGHT;

    Renderer3D::Flush();
    Renderer2D::Flush();
}

void Renderer::BeginSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
    auto& swapchain = context.GetSwapchain();

    CC_ASSERT(isFrameStarted, "Can't start render pass while the frame hasn't started!");
    CC_ASSERT(commandBuffer == GetCurrentCommandBuffer(),
              "Can't begin a render pass on a command buffer from another frame!");

    auto swapExtent = swapchain.GetExtent();

    swapchain.BeginRenderPass(OUT commandBuffer, currentImageIndex, {{clearValue}, {{{1.f, 0.f}}}});

    VkViewport viewport {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapExtent.width);
    viewport.height = static_cast<float>(swapExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor {{0, 0}, {swapExtent.width, swapExtent.height}};

    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void Renderer::EndSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
    auto& swapchain = context.GetSwapchain();

    CC_ASSERT(isFrameStarted, "Can't end render pass while the frame hasn't started!");
    CC_ASSERT(commandBuffer == GetCurrentCommandBuffer(),
              "Can't end a render pass on a command buffer from another frame!");

    swapchain.EndRenderPass(commandBuffer);
}
} // namespace Siege
