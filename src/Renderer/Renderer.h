#pragma once

#include "Device/VulkanDevice.h"
#include "Swapchain/Swapchain.h"
#include "Pipeline/Pipeline.h"

namespace SnekVk 
{

    class Renderer
    {
        public:
            Renderer(SwapChain* swapChain);
            ~Renderer();

            void CreateCommandBuffers(VulkanDevice& device, Pipeline& pipeline);
            void DrawFrame();
        private:
            
            static VkCommandBuffer* commandBuffers;
            SwapChain* swapChain;
    };
}
