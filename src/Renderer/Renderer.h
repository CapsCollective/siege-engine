#pragma once

#include "Device/VulkanDevice.h"
#include "Swapchain/Swapchain.h"
#include "Pipeline/Pipeline.h"
#include "Model/Model.h"

namespace SnekVk 
{
    class Renderer
    {
        public:
            Model CreateModel(Model::Vertex* vertices, u32 vertexCount);

            Renderer(Window& window);
            ~Renderer();

            void DestroyRenderer();

            static VulkanDevice& GetDevice() { return device; }
            SwapChain& GetSwapChain() { return swapChain; }
            VkRenderPass GetSwapChanRenderPass() { return swapChain.GetRenderPass()->GetRenderPass(); }

            int GetCurrentFrameIndex() const
            {
                SNEK_ASSERT(!isFrameStarted, "Can't get frame index when frame is not in progress!")
                return currentFrameIndex;
            }
            
            bool IsFrameStarted() { return isFrameStarted; }

            VkCommandBuffer GetCurrentCommandBuffer() const 
            { 
                SNEK_ASSERT(isFrameStarted, "Can't get command buffer when frame is not in progress!");
                return commandBuffers[currentFrameIndex]; 
            }
            
            void DrawModel(Model* model, const Model::PushConstantData& pushData);

            bool StartFrame();
            void EndFrame();

            void CreatePipelineLayout();
            void ClearDeviceQueue() { vkDeviceWaitIdle(device.Device()); } 
            Pipeline CreateGraphicsPipeline();

            void SetClearValue(float r, float g, float b, float a) { clearValue = {r, g, b, a}; }
        private:
            
            static Utils::Array<VkCommandBuffer> commandBuffers;
            
            VkClearColorValue clearValue {0, 0, 0, 1.f};

            void CreateCommandBuffers();
            void RecreateSwapChain();
            void FreeCommandBuffers();

            void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
            void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

            PipelineConfigInfo CreateDefaultPipelineConfig();

            SnekVk::Window& window;
            
            static VulkanDevice device;
            SwapChain swapChain;

            VkPipelineLayout pipelineLayout{nullptr};
            Pipeline graphicsPipeline;

            u32 currentImageIndex;
            bool isFrameStarted{false};
            int currentFrameIndex{0};
    };
}
