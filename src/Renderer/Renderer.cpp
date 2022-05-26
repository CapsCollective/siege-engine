#include "Renderer.h"

namespace SnekVk
{
    Utils::Array<VkCommandBuffer> Renderer::commandBuffers;
    VulkanDevice Renderer::device;

    Renderer::Renderer(Window& window) : 
        window{window},
        swapChain{SwapChain(device)},
        graphicsPipeline{Pipeline(device)}
    {
        device.SetWindow(&window);
        swapChain.SetWindowExtents(window.GetExtent());
        
        CreatePipelineLayout();
        graphicsPipeline.RecreatePipeline(
            "shaders/simpleShader.vert.spv",
            "shaders/simpleShader.frag.spv",
            CreateDefaultPipelineConfig()
        );

        CreateCommandBuffers();
    }

    Renderer::~Renderer() 
    {
        std::cout << "Destroying renderer" << std::endl;
        vkDestroyPipelineLayout(device.Device(), pipelineLayout, nullptr);
        std::cout << "Destroying Pipeline" << std::endl;
        graphicsPipeline.ClearPipeline();
        std::cout << "Destroying SwapChain" << std::endl;
        swapChain.DestroySwapChain();
        std::cout << "Destroying Device" << std::endl;
        device.DestroyDevice();
    }

    Model Renderer::CreateModel(Model::Vertex* vertices, u32 vertexCount)
    {
        return Model(device, vertices, vertexCount);
    }

    void Renderer::CreateCommandBuffers()
    {
        commandBuffers = Utils::Array<VkCommandBuffer>(SwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device.GetCommandPool();
        allocInfo.commandBufferCount = static_cast<u32>(commandBuffers.Size());

        SNEK_ASSERT(vkAllocateCommandBuffers(device.Device(), &allocInfo, OUT commandBuffers.Data()) == VK_SUCCESS,
            "Failed to allocate command buffer");
    }

    void Renderer::DrawModel(Model* model, glm::mat4 transform, glm::vec3 color)
    {
        auto commandBuffer = GetCurrentCommandBuffer();

        model->Bind(commandBuffer);

        Model::PushConstantData pushData = { transform, color };

        vkCmdPushConstants(
            commandBuffer,
            pipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(Model::PushConstantData),
            &pushData
        );

        model->Draw(commandBuffer);
    }

    void Renderer::RecreateSwapChain()
    {
        ClearDeviceQueue();
        auto extent = window.GetExtent();
        while(extent.width == 0 || extent.height == 0)
        {
            extent = window.GetExtent();
            window.WaitEvents();
        }

        auto oldImageFormat = swapChain.GetImageFormat();
        auto oldDepthFormat = swapChain.GetDepthFormat();

        // Re-create swapchain
        swapChain.RecreateSwapchain();

        // Re-create the pipeline once the swapchain renderpass 
        // becomes available again.
        if (!swapChain.CompareSwapFormats(oldImageFormat, oldDepthFormat)) {
            // Clear our graphics pipeline before swapchain re-creation
            graphicsPipeline.ClearPipeline();

            // NOTE: We could possibly avoid this if we check for render pass compatibility.
            // If the new renderpass is compatible with the old, then we can actually keep the
            // the same graphics pipeline.
            graphicsPipeline.RecreatePipeline(
                    "shaders/simpleShader.vert.spv",
                    "shaders/simpleShader.frag.spv",
                    CreateDefaultPipelineConfig()
            );
        }
    }

    void Renderer::FreeCommandBuffers()
    {
        vkFreeCommandBuffers(
            device.Device(), 
            device.GetCommandPool(), 
            swapChain.GetImageCount(), 
            commandBuffers.Data());
    }

    PipelineConfigInfo Renderer::CreateDefaultPipelineConfig()
    {
        auto pipelineConfig = Pipeline::DefaultPipelineConfig();
        pipelineConfig.renderPass = swapChain.GetRenderPass()->GetRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;

        return pipelineConfig;
    }

    void Renderer::CreatePipelineLayout()
    {
        VkPushConstantRange range{};
        range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        range.offset = 0; 
        range.size = sizeof(Model::PushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0; 
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &range;

        SNEK_ASSERT(vkCreatePipelineLayout(device.Device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) == VK_SUCCESS, 
            "Failed to create pipeline layout!");
    }

    Pipeline Renderer::CreateGraphicsPipeline()
    {
        CreatePipelineLayout();

        SNEK_ASSERT(pipelineLayout != nullptr, "Cannot create pipeline without a valid layout!");

        return SnekVk::Pipeline(
            device, 
            "shaders/simpleShader.vert.spv", 
            "shaders/simpleShader.frag.spv", 
            CreateDefaultPipelineConfig()
        );
    }

    bool Renderer::StartFrame()
    {
        SNEK_ASSERT(!isFrameStarted, "Can't start a frame when a frame is already in progress!");

        auto result = swapChain.AcquireNextImage(&currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) 
        {
            RecreateSwapChain();
            return false;
        }

        SNEK_ASSERT(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR, 
            "Failed to acquire swapchain image!");

        isFrameStarted = true;

        VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        SNEK_ASSERT(vkBeginCommandBuffer(OUT commandBuffer, &beginInfo) == VK_SUCCESS,
            "Failed to begin recording command buffer");
        
        
        BeginSwapChainRenderPass(commandBuffer);

        graphicsPipeline.Bind(commandBuffer);
        
        return true;
    }

    void Renderer::EndFrame()
    {
        SNEK_ASSERT(isFrameStarted, "Can't end frame while frame is not in progress!");

        VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();

        EndSwapChainRenderPass(commandBuffer);

        SNEK_ASSERT(vkEndCommandBuffer(OUT commandBuffer) == VK_SUCCESS,
            "Failed to record command buffer!");

        auto result = swapChain.SubmitCommandBuffers(&commandBuffer, &currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
            window.WasResized())
        {
            window.ResetWindowResized();
            RecreateSwapChain();
        } else if (result != VK_SUCCESS) 
        {
            SNEK_ASSERT(result == VK_SUCCESS, "Failed to submit command buffer for drawing!");
        }

        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT; 
    }

    void Renderer::BeginSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        SNEK_ASSERT(isFrameStarted, "Can't start render pass while the frame hasn't started!");
        SNEK_ASSERT(commandBuffer == GetCurrentCommandBuffer(), "Can't begin a render pass on a command buffer from another frame!");

        u32 clearValueCount = 2;
            VkClearValue clearValues[clearValueCount];
            clearValues[0].color = clearValue;
            clearValues[1].depthStencil = {1.0f, 0};

        RenderPass::Begin(swapChain.GetRenderPass()->GetRenderPass(),
                          OUT commandBuffer,
                          swapChain.GetFrameBuffer(currentImageIndex),
                          {0,0},
                          swapChain.GetSwapChainExtent(),
                          clearValues,
                          clearValueCount);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain.GetSwapChainExtent().width);
        viewport.height = static_cast<float>(swapChain.GetSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor {{0,0}, swapChain.GetSwapChainExtent()};

        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void Renderer::EndSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        SNEK_ASSERT(isFrameStarted, "Can't end render pass while the frame hasn't started!");
        SNEK_ASSERT(commandBuffer == GetCurrentCommandBuffer(), "Can't begin a render pass on a command buffer from another frame!");
        
        RenderPass::End(commandBuffer);
    }
}
