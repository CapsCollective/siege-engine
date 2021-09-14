#pragma once

#include "Device/VulkanDevice.h"
#include "Swapchain/Swapchain.h"
#include "Pipeline/Pipeline.h"
#include "Model/Model.h"
#include "Camera/Camera.h"
#include "Material/Material.h"

namespace SnekVk 
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
                SNEK_ASSERT(!isFrameStarted, "Can't get frame index when frame is not in progress!")
                return currentFrameIndex;
            }

            float GetAspectRatio() const { return swapChain.ExtentAspectRatio(); }

            void SetMainCamera(Camera* camera) { mainCamera = camera; }
            
            bool IsFrameStarted() { return isFrameStarted; }

            VkCommandBuffer GetCurrentCommandBuffer() const 
            { 
                SNEK_ASSERT(isFrameStarted, "Can't get command buffer when frame is not in progress!");
                return commandBuffers[currentFrameIndex]; 
            }
            
            void DrawModel(Model* model, const Model::Transform& transform);

            void RegisterMaterial(Material* mat) { currentMat = mat; }

            bool StartFrame();
            void EndFrame();

            void ClearDeviceQueue() { vkDeviceWaitIdle(device.Device()); } 
            Pipeline CreateGraphicsPipeline();

            void SetClearValue(float r, float g, float b, float a) { clearValue = {r, g, b, a}; }
        private:

            static constexpr size_t MAX_OBJECT_TRANSFORMS = 10000;
            
            static VulkanDevice* deviceInstance;
            static Utils::Array<VkCommandBuffer> commandBuffers;
            
            VkClearColorValue clearValue {0, 0, 0, 1.f};

            void CreateCommandBuffers();

            void RecreateSwapChain();
            void FreeCommandBuffers();

            void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
            void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

            void DrawFrame();

            PipelineConfigInfo CreateDefaultPipelineConfig();

            SnekVk::Window& window;
            
            VulkanDevice device;
            SwapChain swapChain;

            u32 currentImageIndex;
            bool isFrameStarted{false};
            int currentFrameIndex{0};
            
            Material* currentMat{nullptr};
            Model* currentModel{nullptr};

            Model::Transform transforms[MAX_OBJECT_TRANSFORMS];
            Model* models[MAX_OBJECT_TRANSFORMS];
            size_t modelCount = 0;

            Camera* mainCamera;
    };
}
