//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#pragma once

#include "camera/Camera.h"
#include "descriptor/DescriptorPool.h"
#include "device/VulkanDevice.h"
#include "lights/PointLight.h"
#include "material/Material.h"
#include "model/Model.h"
#include "pipeline/Pipeline.h"
#include "renderer/Renderer2D.h"
#include "renderer/Renderer3D.h"
#include "swapchain/Swapchain.h"

namespace Siege
{
class Renderer
{
public:

    explicit Renderer(Window& window);

    ~Renderer();

    VulkanDevice& GetDevice()
    {
        return device;
    }
    SwapChain& GetSwapChain()
    {
        return swapChain;
    }
    VkRenderPass GetSwapChanRenderPass()
    {
        return swapChain.GetRenderPass()->GetRenderPass();
    }

    int GetCurrentFrameIndex() const
    {
        CC_ASSERT(!isFrameStarted, "Can't get frame index when frame is not in progress!")
        return currentFrameIndex;
    }

    float GetAspectRatio() const
    {
        return swapChain.ExtentAspectRatio();
    }

    void SetMainCamera(Camera* camera)
    {
        mainCamera = camera;
    }

    bool IsFrameStarted() const
    {
        return isFrameStarted;
    }

    VkCommandBuffer GetCurrentCommandBuffer() const
    {
        CC_ASSERT(isFrameStarted, "Can't get command buffer when frame is not in progress!");
        return commandBuffers[currentFrameIndex];
    }

    bool StartFrame();
    void EndFrame();

    void ClearDeviceQueue()
    {
        vkDeviceWaitIdle(device.Device());
    }

    void SetClearValue(float r, float g, float b, float a)
    {
        clearValue = {{r, g, b, a}};
    }

private:

    static constexpr size_t MAX_OBJECT_TRANSFORMS = 1000;

    static VulkanDevice* deviceInstance;
    static Utils::Array<VkCommandBuffer> commandBuffers;

    VkClearColorValue clearValue {{0, 0, 0, 1.f}};

    void CreateCommandBuffers();

    void RecreateSwapChain();
    void FreeCommandBuffers();

    void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

    void DrawFrame();

    Siege::Window& window;

    VulkanDevice device;
    SwapChain swapChain;

    u32 currentImageIndex;
    bool isFrameStarted {false};
    int currentFrameIndex {0};

    Camera* mainCamera;
};
} // namespace Siege
