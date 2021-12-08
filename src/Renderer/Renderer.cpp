#include "Renderer.h"

namespace SnekVk
{
    Utils::Array<VkCommandBuffer> Renderer::commandBuffers;
    VulkanDevice* Renderer::deviceInstance = nullptr;

    Renderer::Renderer(Window& window) : 
        window{window},
        swapChain{SwapChain(device)}
    {
        device.SetWindow(&window);
        swapChain.SetWindowExtents(window.GetExtent());

        if (deviceInstance == nullptr) deviceInstance = &device;

        bufferId = INTERN_STR("objectBuffer");
        lightId = INTERN_STR("lightDir");
        cameraDataId = INTERN_STR("cameraData");

        CreateCommandBuffers();
    }

    Renderer::~Renderer() 
    {
        std::cout << "Destroying renderer" << std::endl;
        Material::DestroyDescriptorPool();
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

    void Renderer::DrawModel(Model* model, const Model::Transform& transform)
    {
        models[modelCount] = model;
        // might need to see if there's a way to avoid a copy here.
        transforms[modelCount] = { transform.transform, transform.normalMatrix };
        SNEK_ASSERT(modelCount++ <= MAX_OBJECT_TRANSFORMS, 
                "LIMIT REACHED ON RENDERABLE MODELS");
    }

    void Renderer::DrawModel2D(Model* model, const Model::Transform2D& transform)
    {
        models2D[model2DCount] = model;
        // might need to see if there's a way to avoid a copy here.
        transforms2D[model2DCount] = { transform.transform };
        SNEK_ASSERT(model2DCount++ <= MAX_OBJECT_TRANSFORMS, 
                "LIMIT REACHED ON RENDERABLE MODELS");
    }

    void Renderer::DrawFrame()
    {
        if (modelCount == 0) return;

        auto commandBuffer = GetCurrentCommandBuffer();

        VkDeviceSize bufferSize = sizeof(transforms[0]) * MAX_OBJECT_TRANSFORMS;
        VkDeviceSize dirToLightBufferSize = sizeof(glm::vec3);
        VkDeviceSize cameraDataBufferSize = sizeof(glm::mat4);

        glm::vec3 dirToLight(1.0f, -3.0f, -1.0f);
        glm::mat4 cameraData = mainCamera->GetProjView();

        for (size_t i = 0; i < modelCount; i++)
        {
            auto model = models[i];

            if (currentMat != model->GetMaterial())
            {
                currentMat = model->GetMaterial();
                currentMat->SetUniformData(bufferId, bufferSize, transforms);
                currentMat->SetUniformData(lightId, dirToLightBufferSize, &dirToLight);
                currentMat->SetUniformData("cameraData", cameraDataBufferSize, &cameraData);
                currentMat->Bind(commandBuffer);
            } 

            if (currentModel != model)
            {
                currentModel = model;
                currentModel->Bind(commandBuffer);
            }

            model->Draw(commandBuffer, i);
        }

        VkDeviceSize bufferSize2D = sizeof(transforms2D[0]) * MAX_OBJECT_TRANSFORMS;

        for (size_t i = 0; i < model2DCount; i++)
        {
            auto model = models2D[i];

            if (currentMat != model->GetMaterial())
            {
                currentMat = model->GetMaterial();
                currentMat->SetUniformData(bufferId, bufferSize2D, transforms2D);
                currentMat->SetUniformData("cameraData", cameraDataBufferSize, &cameraData);
                currentMat->Bind(commandBuffer);
            } 

            if (currentModel != model)
            {
                currentModel = model;
                currentModel->Bind(commandBuffer);
            }

            model->Draw(commandBuffer, i);
        }

        currentModel = nullptr;
        currentMat = nullptr;
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
        if (!swapChain.CompareSwapFormats(oldImageFormat, oldDepthFormat)) 
        {
            currentMat->RecreatePipeline();
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
        
        return true;
    }

    void Renderer::EndFrame()
    {
        SNEK_ASSERT(isFrameStarted, "Can't end frame while frame is not in progress!");

        DrawFrame();

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

        // Reset the collections of models
        memset(transforms, 0, sizeof(transforms[0]) * modelCount);
        memset(models, 0, sizeof(models[0]) * modelCount);
        modelCount = 0;

        // TODO: Duplicated logic, might be good to create standalone container
        memset(transforms2D, 0, sizeof(transforms2D[0]) * model2DCount);
        memset(models, 0, sizeof(models2D[0]) * model2DCount);
        model2DCount = 0;
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
