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
        private:
            
            static VkCommandBuffer* commandBuffers;
            
            VulkanDevice device;
            SwapChain swapChain;

            VkPipelineLayout pipelineLayout;
            Pipeline graphicsPipeline{CreateGraphicsPipeline()};
    };
}
