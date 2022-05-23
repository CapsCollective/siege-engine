#include "Renderer.h"

namespace SnekVk
{
    VkCommandBuffer* Renderer::commandBuffers;

    Renderer::Renderer(SnekVk::Window& window) 
        : device{VulkanDevice(window)}, swapChain{SwapChain(device,  window.GetExtent())}
    {
        commandBuffers = new VkCommandBuffer[swapChain.GetImageCount()];
    }
    
    Renderer::~Renderer() 
    {
        vkDestroyPipelineLayout(device.Device(), pipelineLayout, nullptr);
    }

    void Renderer::SubmitModel(Model& model)
    {
        CreateCommandBuffers(graphicsPipeline, model);
    }

    void Renderer::CreateCommandBuffers(Pipeline& pipeline, Model& model)
    {
        u32 size = swapChain.GetImageCount();

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device.GetCommandPool();
        allocInfo.commandBufferCount = size;

        SNEK_ASSERT(vkAllocateCommandBuffers(device.Device(), &allocInfo, OUT commandBuffers) == VK_SUCCESS,
            "Failed to allocate command buffer");

        for (u32 i = 0; i < size; i++)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            SNEK_ASSERT(vkBeginCommandBuffer(OUT commandBuffers[i], &beginInfo) == VK_SUCCESS,
                "Failed to begin recording command buffer");
            
            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = swapChain.GetRenderPass();
            renderPassInfo.framebuffer = swapChain.GetFrameBuffer(i);
            
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = swapChain.GetSwapChainExtent();

            u32 clearValueCount = 2;
            VkClearValue clearValues[clearValueCount];
            clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};

            renderPassInfo.clearValueCount = clearValueCount;
            renderPassInfo.pClearValues = clearValues;
            
            vkCmdBeginRenderPass(OUT commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            pipeline.Bind(commandBuffers[i]);
            model.Bind(commandBuffers[i]);
            model.Draw(commandBuffers[i]);

            vkCmdEndRenderPass(OUT commandBuffers[i]);

            SNEK_ASSERT(vkEndCommandBuffer(OUT commandBuffers[i]) == VK_SUCCESS,
                "Failed to record command buffer!");
        }
    }

    void Renderer::CreatePipelineLayout()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0; 
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        SNEK_ASSERT(vkCreatePipelineLayout(device.Device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) == VK_SUCCESS, 
            "Failed to create pipeline layout!");
    }

    Pipeline Renderer::CreateGraphicsPipeline()
    {
        CreatePipelineLayout();
        auto pipelineConfig = Pipeline::DefaultPipelineConfig(swapChain.GetWidth(), swapChain.GetHeight());
        pipelineConfig.renderPass = swapChain.GetRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;

        return SnekVk::Pipeline(device, "./shaders/simpleShader.vert.spv", "./shaders/simpleShader.frag.spv", pipelineConfig);
    }

    void Renderer::DrawFrame()
    {
        u32 imageIndex;
        auto result = swapChain.AcquireNextImage(&imageIndex);

        SNEK_ASSERT(result == VK_SUCCESS && result != VK_SUBOPTIMAL_KHR, 
            "Failed to acquire swapchain image!");
        
        result = swapChain.SubmitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

        SNEK_ASSERT(result == VK_SUCCESS, "Failed to submit command buffer for drawing!");
    }
}
