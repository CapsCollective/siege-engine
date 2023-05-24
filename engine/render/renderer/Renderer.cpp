//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#define VOLK_IMPLEMENTATION

#include "Renderer.h"

#include "platform/vulkan/Font.h"
#include "platform/vulkan/utils/Types.h"
#include "statics/Statics.h"

namespace Siege
{
Renderer* Renderer::instance {nullptr};
Vulkan::CommandBuffer Renderer::commandBuffers;
uint32_t Renderer::currentFrameIndex = 0;

Renderer::Renderer(Window& window) : window {window}
{
    Statics::Initialise();

    if (instance == nullptr) instance = this;

    auto extent = window.GetExtent();

    context.Init({extent.width, extent.height},
                 Window::GetRequiredExtensions,
                 Window::CreateWindowSurface);

    DescriptorPool::AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1024);
    DescriptorPool::AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1024);
    DescriptorPool::AddPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1024);
    DescriptorPool::AddPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1024);
    DescriptorPool::AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1024);

    DescriptorPool::BuildPool();

    Renderer3D::Initialise();
    renderer2D.Initialise("globalData");

    commandBuffers = Vulkan::CommandBuffer(Vulkan::Swapchain::MAX_FRAMES_IN_FLIGHT);
}

Renderer::~Renderer()
{
    CC_LOG_INFO("Destroying renderer")
    DescriptorPool::DestroyPool();
    Renderer3D::DestroyRenderer3D();
    Statics::Free();
}

void Renderer::DrawFrame()
{
    Renderer3D::Render(currentFrameIndex, commandBuffers, projection);
    renderer2D.Render(commandBuffers, sizeof(uiCamera), &uiCamera, currentFrameIndex);
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
    }
}

bool Renderer::StartFrame()
{
    CC_ASSERT(!isFrameStarted, "Can't start a frame when a frame is already in progress!")

    auto& swapchain = context.GetSwapchain();

    auto result = swapchain.AcquireNextImage(&currentImageIndex);

    if (result == Vulkan::Utils::ERROR_OUT_OF_DATE)
    {
        RecreateSwapChain();
        return false;
    }

    CC_ASSERT(result == Vulkan::Utils::SUCCESS || result == Vulkan::Utils::SUBOPTIMAL,
              "Failed to acquire swapchain image!")

    isFrameStarted = true;

    commandBuffers.Begin(currentFrameIndex);

    BeginSwapChainRenderPass();

    return true;
}

void Renderer::DrawQuad(const Vec2 position,
                        const Vec2 scale,
                        const IColour colour,
                        float rotation,
                        const uint8_t zIndex,
                        Vulkan::Texture2D* texture)
{
    renderer2D.DrawQuad(position, scale, colour, rotation, zIndex, texture);
}

void Renderer::DrawText2D(const char* const text,
                          Vulkan::Font& font,
                          const Vec2 position,
                          const Vec2 scale,
                          float rotation,
                          const IColour colour,
                          const uint8_t zIndex)
{
    renderer2D.DrawText2D(text, position, scale, font, colour);
}

void Renderer::DrawGrid2D(float spacing, const Vec3& lineColouring, float scale,  float lineWidth, float fadeFactor, float cellMultiple)
{
    renderer2D.DrawGrid2D(spacing, lineColouring, scale, lineWidth, fadeFactor, cellMultiple);
}

void Renderer::EndFrame()
{
    Renderer3D::Update();
    renderer2D.Update();

    CC_ASSERT(isFrameStarted, "Can't end frame while frame is not in progress!")

    auto& swapchain = context.GetSwapchain();

    DrawFrame();

    EndSwapChainRenderPass();

    commandBuffers.End();

    commandBuffers.SetActiveBufferIndex(currentFrameIndex);

    auto result = swapchain.SubmitCommandBuffers(commandBuffers, currentImageIndex);

    if (result == Vulkan::Utils::ERROR_RESIZED || window.WasResized())
    {
        window.ResetWindowResized();
        RecreateSwapChain();
    }

    isFrameStarted = false;
    currentFrameIndex = (currentFrameIndex + 1) % Vulkan::Swapchain::MAX_FRAMES_IN_FLIGHT;

    Renderer3D::Flush();
    renderer2D.Flush();
}

void Renderer::BeginSwapChainRenderPass()
{
    auto& swapchain = context.GetSwapchain();

    CC_ASSERT(isFrameStarted, "Can't start render pass while the frame hasn't started!")

    auto swapExtent = swapchain.GetExtent();

    swapchain.BeginRenderPass(commandBuffers, currentImageIndex, {{clearValue}, {{{1.f, 0.f}}}});

    VkViewport viewport {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapExtent.width);
    viewport.height = static_cast<float>(swapExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor {{0, 0}, {swapExtent.width, swapExtent.height}};

    vkCmdSetViewport(commandBuffers.GetActiveCommandBuffer(), 0, 1, &viewport);
    vkCmdSetScissor(commandBuffers.GetActiveCommandBuffer(), 0, 1, &scissor);
}

void Renderer::EndSwapChainRenderPass()
{
    auto& swapchain = context.GetSwapchain();

    CC_ASSERT(isFrameStarted, "Can't end render pass while the frame hasn't started!")

    swapchain.EndRenderPass(commandBuffers);
}
} // namespace Siege
