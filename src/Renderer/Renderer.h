#pragma once

#include "Device/VulkanDevice.h"
#include "Swapchain/Swapchain.h"
#include "Pipeline/Pipeline.h"
#include "Model/Model.h"

#include <vector>

namespace SnekVk 
{
    class Renderer
    {
        public:
            Model CreateModel(Model::Vertex* vertices, u32 vertexCount);

            Renderer(Window& window);
            ~Renderer();

            void DestroyRenderer();

            void SubmitModel(Model* model);
            
            void DrawFrame();

            static VulkanDevice& GetDevice() { return device; }
            SwapChain& GetSwapChain() { return swapChain; }

            void CreatePipelineLayout();
            void ClearDeviceQueue() { vkDeviceWaitIdle(device.Device()); } 
            Pipeline CreateGraphicsPipeline();

            void SetClearValue(float r, float g, float b, float a) { clearValue = {r, g, b, a}; }
        private:
            
            static Utils::Array<VkCommandBuffer> commandBuffers;
            
            VkClearColorValue clearValue {0, 0, 0, 1.f};

            void CreateCommandBuffers();

            void RecordCommandBuffer(int imageIndex);
            void RecreateSwapChain();
            void FreeCommandBuffers();
            PipelineConfigInfo CreateDefaultPipelineConfig();

            SnekVk::Window& window;
            
            static VulkanDevice device;
            SwapChain swapChain;

            std::vector<Model*> models;

            VkPipelineLayout pipelineLayout{nullptr};
            Pipeline graphicsPipeline;
    };
}
