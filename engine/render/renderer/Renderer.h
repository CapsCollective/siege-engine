#pragma once

#include "device/VulkanDevice.h"
#include "swapchain/Swapchain.h"
#include "pipeline/Pipeline.h"
#include "model/Model.h"
#include "camera/Camera.h"
#include "material/Material.h"
#include "lights/PointLight.h"
#include "renderer/Renderer3D.h"
#include "renderer/Renderer2D.h"
#include "descriptor/DescriptorPool.h"

namespace Siege
{
    class Renderer
    {
        public:

            Renderer(Window& window);
            ~Renderer();

            void DestroyRenderer();

            VulkanDevice& GetDevice() { return device; }
            SwapChain& GetSwapChain() { return swapChain; }
            VkRenderPass GetSwapChanRenderPass() { return swapChain.GetRenderPass()->GetRenderPass(); }

            int GetCurrentFrameIndex() const
            {
                CC_ASSERT(!isFrameStarted, "Can't get frame index when frame is not in progress!")
                return currentFrameIndex;
            }

            float GetAspectRatio() const { return swapChain.ExtentAspectRatio(); }

            void SetMainCamera(Camera* camera) { mainCamera = camera; }
            
            bool IsFrameStarted() { return isFrameStarted; }

            VkCommandBuffer GetCurrentCommandBuffer() const 
            { 
                CC_ASSERT(isFrameStarted, "Can't get command buffer when frame is not in progress!");
                return commandBuffers[currentFrameIndex]; 
            }

            bool StartFrame();
            void EndFrame();

            void ClearDeviceQueue() { vkDeviceWaitIdle(device.Device()); } 
            Pipeline CreateGraphicsPipeline();

            void SetClearValue(float r, float g, float b, float a) { clearValue = {r, g, b, a}; }
        private:
            static constexpr size_t MAX_OBJECT_TRANSFORMS = 1000;
            
            static VulkanDevice* deviceInstance;
            static Utils::Array<VkCommandBuffer> commandBuffers;
            
            VkClearColorValue clearValue {0, 0, 0, 1.f};

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
            bool isFrameStarted{false};
            int currentFrameIndex{0};

            Camera* mainCamera;
    };
}
