//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDERER_H
#define SIEGE_ENGINE_RENDERER_H

#include <utils/Logging.h>

#include "descriptor/DescriptorPool.h"
#include "lights/PointLight.h"
#include "material/Material.h"
#include "model/Model.h"
#include "pipeline/Pipeline.h"
#include "render/renderer/platform/vulkan/Context.h"
#include "render/window/Window.h"
#include "renderer/Renderer2D.h"
#include "renderer/Renderer3D.h"

namespace Siege
{
class Renderer
{
public:

    explicit Renderer(Window& window);

    ~Renderer();

    int GetCurrentFrameIndex() const
    {
        CC_ASSERT(!isFrameStarted, "Can't get frame index when frame is not in progress!");
        return currentFrameIndex;
    }

    float GetAspectRatio() const
    {
        return static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight());
    }

    void SetProjection(const Mat4& projectionMat, const Mat4& viewMat)
    {
        projection = {projectionMat, viewMat};
    }

    VkCommandBuffer GetCurrentCommandBuffer() const
    {
        CC_ASSERT(isFrameStarted, "Can't get command buffer when frame is not in progress!")
        return commandBuffers[currentFrameIndex];
    }

    bool StartFrame();
    void EndFrame();

    void ClearDeviceQueue()
    {
        vkDeviceWaitIdle(Context().GetVkLogicalDevice());
    }

    void SetClearValue(float r, float g, float b, float a)
    {
        clearValue = {{r, g, b, a}};
    }

    static Vulkan::Context& Context()
    {
        return Get()->context;
    }

    static Renderer* Get()
    {
        return instance;
    }

private:

    static Renderer* instance;
    static Utils::MHArray<VkCommandBuffer> commandBuffers;

    Vulkan::Context context;

    // Make this adjustable in the window, not the renderer.
    VkClearColorValue clearValue {{0.96f, 0.96f, 0.96f, 1.f}};

    void CreateCommandBuffers();

    void RecreateSwapChain();

    void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

    void DrawFrame();

    Window& window;

    uint32_t currentImageIndex;
    bool isFrameStarted {false};
    int currentFrameIndex {0};

    CameraData projection;
};
} // namespace Siege

#endif
