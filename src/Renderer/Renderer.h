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
            Renderer(SnekVk::Window& window);
            ~Renderer();

            void SubmitModel(Model& model);
            void CreateCommandBuffers(Pipeline& pipeline, Model& model);
            void DrawFrame();

            VulkanDevice& GetDevice() { return device; }
            SwapChain& GetSwapChain() { return swapChain; }

            void CreatePipelineLayout();
            void ClearDeviceQueue() { vkDeviceWaitIdle(device.Device()); } 
            Pipeline CreateGraphicsPipeline();

            void SetClearValue(float r, float g, float b, float a) { clearValue = {r, g, b, a}; }
        private:
            
            static VkCommandBuffer* commandBuffers;
            VkClearColorValue clearValue {0, 0, 0, 1.f};
            
            VulkanDevice device;
            SwapChain swapChain;

            VkPipelineLayout pipelineLayout;
            Pipeline graphicsPipeline{CreateGraphicsPipeline()};
    };
}
