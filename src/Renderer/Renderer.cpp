#include "Renderer.h"

namespace SnekVk
{
    Utils::Array<VkCommandBuffer> Renderer::commandBuffers;
    VulkanDevice* Renderer::deviceInstance = nullptr;

    Renderer::Renderer(Window& window) : 
        window{window},
        swapChain{SwapChain(device)},
        graphicsPipeline{Pipeline(device)}
    {
        device.SetWindow(&window);
        swapChain.SetWindowExtents(window.GetExtent());

        if (deviceInstance == nullptr) deviceInstance = &device;

        CreateDescriptors();
        
        CreatePipelineLayout();

        PipelineConfig::ShaderConfig shaders[] = 
        {
            { "shaders/simpleShader.vert.spv", PipelineConfig::PipelineStage::VERTEX },
            { "shaders/simpleShader.frag.spv", PipelineConfig::PipelineStage::FRAGMENT }
        };

        graphicsPipeline.RecreatePipeline(
            shaders,
            2,
            CreateDefaultPipelineConfig()
        );

        CreateCommandBuffers();
    }

    Renderer::~Renderer() 
    {
        std::cout << "Destroying renderer" << std::endl;
        vkDestroyDescriptorSetLayout(device.Device(), globalLayout, nullptr);
        vkDestroyDescriptorSetLayout(device.Device(), objectLayout, nullptr);
        vkDestroyDescriptorPool(device.Device(), descriptorPool, nullptr);
        vkDestroyPipelineLayout(device.Device(), pipelineLayout, nullptr);
        Buffer::DestroyBuffer(uniformCamBuffer);
        Buffer::DestroyBuffer(objectTransformsBuffer);
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

    void Renderer::CreateDescriptors()
    {
        // Create the camera data uniform buffer
        Buffer::CreateBuffer(
            sizeof(Camera::GPUCameraData),
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
            // Ensures that CPU and GPU memory are consistent across both devices.
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            OUT uniformCamBuffer.buffer,
            OUT uniformCamBuffer.bufferMemory
        );

        Buffer::CreateBuffer(
            sizeof(Model::Transform) * MAX_OBJECT_TRANSFORMS,
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
            // Ensures that CPU and GPU memory are consistent across both devices.
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            OUT objectTransformsBuffer.buffer,
            OUT objectTransformsBuffer.bufferMemory
        );

        // Create a descriptor set for camera data

        VkDescriptorSetLayoutBinding camBufferBinding {};
        camBufferBinding.binding = 0;
        camBufferBinding.descriptorCount = 1;
        camBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        camBufferBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutCreateInfo layoutCreateInfo{};
        layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutCreateInfo.pNext = nullptr;

        layoutCreateInfo.bindingCount = 1;
        layoutCreateInfo.flags = 0;
        layoutCreateInfo.pBindings = &camBufferBinding;

        SNEK_ASSERT(vkCreateDescriptorSetLayout(device.Device(), &layoutCreateInfo, nullptr, &globalLayout) == VK_SUCCESS,
                "Failed to create descriptor set!");
        
        // Create a descriptor set for our object transforms.

        VkDescriptorSetLayoutBinding objectBufferBinding {};
        objectBufferBinding.binding = 0;
        objectBufferBinding.descriptorCount = 1;
        objectBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        objectBufferBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutCreateInfo objectLayoutCreateInfo{};
        objectLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        objectLayoutCreateInfo.pNext = nullptr;

        objectLayoutCreateInfo.bindingCount = 1;
        objectLayoutCreateInfo.flags = 0;
        objectLayoutCreateInfo.pBindings = &objectBufferBinding;
        
        SNEK_ASSERT(vkCreateDescriptorSetLayout(device.Device(), &objectLayoutCreateInfo, nullptr, &objectLayout) == VK_SUCCESS,
                "Failed to create descriptor set!");
        
        VkDescriptorPoolSize poolSizes[] = {
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 10}
        };

        VkDescriptorPoolCreateInfo poolCreateInfo {};
        poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolCreateInfo.flags = 0;
        poolCreateInfo.maxSets = 10;
        poolCreateInfo.poolSizeCount = 2;
        poolCreateInfo.pPoolSizes = poolSizes;

        SNEK_ASSERT(vkCreateDescriptorPool(device.Device(), &poolCreateInfo, nullptr, &descriptorPool) == VK_SUCCESS,
            "Unable to create descriptor pool!");
        
        // Camera data allocate info
        VkDescriptorSetAllocateInfo allocateInfo {};
        allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocateInfo.pNext = nullptr;
        allocateInfo.descriptorPool = descriptorPool;
        allocateInfo.descriptorSetCount = 1;
        allocateInfo.pSetLayouts = &globalLayout;
        
        vkAllocateDescriptorSets(device.Device(), &allocateInfo, &globalDescriptor);

        // Object transform allocate info
        VkDescriptorSetAllocateInfo allocateInfo2 {};
        allocateInfo2.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocateInfo2.pNext = nullptr;
        allocateInfo2.descriptorPool = descriptorPool;
        allocateInfo2.descriptorSetCount = 1;
        allocateInfo2.pSetLayouts = &objectLayout;
        
        vkAllocateDescriptorSets(device.Device(), &allocateInfo2, &objectDescriptor);

        VkDescriptorBufferInfo bufferInfo {};
        bufferInfo.buffer = uniformCamBuffer.buffer;
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(Camera::GPUCameraData);

        VkDescriptorBufferInfo objBufferInfo {};
        objBufferInfo.buffer = objectTransformsBuffer.buffer;
        objBufferInfo.offset = 0;
        objBufferInfo.range = sizeof(Model::Transform) * MAX_OBJECT_TRANSFORMS;

        VkWriteDescriptorSet writeDescriptorSet {};
        writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet.pNext = nullptr;
        writeDescriptorSet.dstBinding = 0;
        writeDescriptorSet.dstSet = globalDescriptor;
        writeDescriptorSet.descriptorCount = 1;
        writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeDescriptorSet.pBufferInfo = &bufferInfo;

        VkWriteDescriptorSet writeDescriptorSet2 {};
        writeDescriptorSet2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet2.pNext = nullptr;
        writeDescriptorSet2.dstBinding = 0;
        writeDescriptorSet2.dstSet = objectDescriptor;
        writeDescriptorSet2.descriptorCount = 1;
        writeDescriptorSet2.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        writeDescriptorSet2.pBufferInfo = &objBufferInfo;

        VkWriteDescriptorSet writeDescriptorSets[] = {writeDescriptorSet, writeDescriptorSet2};

        vkUpdateDescriptorSets(device.Device(), 2, writeDescriptorSets, 0,nullptr);
    }

    void Renderer::DrawModel(Model* model, const Model::Transform& transform)
    {
        models[modelCount] = model;
        transforms[modelCount] = transform;
        SNEK_ASSERT(modelCount++ <= MAX_OBJECT_TRANSFORMS, 
                "LIMIT REACHED ON RENDERABLE MODELS");
    }

    void Renderer::DrawFrame()
    {
        if (modelCount == 0) return;

        auto commandBuffer = GetCurrentCommandBuffer();

        VkDeviceSize bufferSize = sizeof(Model::Transform) * MAX_OBJECT_TRANSFORMS;

        // TODO - profile this as we're unsure whether one big copy is better than a few small ones
        Buffer::CopyData<Model::Transform>(objectTransformsBuffer, bufferSize, transforms);

        for (size_t i = 0; i < modelCount; i++)
        {
            models[i]->Bind(commandBuffer);
            
            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 1, 1, &objectDescriptor, 0, nullptr);

            // PUSH CONSTANTS ARE NO LONGER IN USE
            // vkCmdPushConstants(
            //     commandBuffer,
            //     pipelineLayout,
            //     VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            //     0,
            //     sizeof(Model::Transform),
            //     &transforms[i]
            // );

            models[i]->Draw(commandBuffer, i);
        }
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

            PipelineConfig::ShaderConfig shaders[] = 
            {
                { "shaders/simpleShader.vert.spv", PipelineConfig::PipelineStage::VERTEX },
                { "shaders/simpleShader.frag.spv", PipelineConfig::PipelineStage::FRAGMENT }
            };

            graphicsPipeline.RecreatePipeline(
                shaders,
                2,
                CreateDefaultPipelineConfig());
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
        pipelineConfig.vertexData = GetDescriptionData();

        return pipelineConfig;
    }

    void Renderer::CreatePipelineLayout()
    {
        VkPushConstantRange range = PipelineConfig::CreatePushConstantRange(
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 
                0, 
                sizeof(Model::Transform));

        VkDescriptorSetLayout layouts[] = { globalLayout, objectLayout};

        PipelineConfig::CreatePipelineLayout(device.Device(), OUT &pipelineLayout, layouts, 2, &range, 1);
    }

    Pipeline Renderer::CreateGraphicsPipeline()
    {
        CreatePipelineLayout();

        SNEK_ASSERT(pipelineLayout != nullptr, "Cannot create pipeline without a valid layout!");

        PipelineConfig::ShaderConfig shaders[] = 
        {
            { "shaders/simpleShader.vert.spv", PipelineConfig::PipelineStage::VERTEX },
            { "shaders/simpleShader.frag.spv", PipelineConfig::PipelineStage::FRAGMENT }
        };

        return SnekVk::Pipeline(
            device, 
            shaders,
            2,
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

        // Might need to find a better way to structure this section.
        // We should be able to only re-bind a pipeline when a new one is 
        // provided.
        // This can probably be bundled with a material system when ready.
        graphicsPipeline.Bind(commandBuffer);

        auto camData = mainCamera->GetCameraData();

        Buffer::CopyData<Camera::GPUCameraData>(uniformCamBuffer, sizeof(Camera::GPUCameraData), &camData);
        
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &globalDescriptor, 0, nullptr);
        
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
        memset(transforms, 0, sizeof(Model::Transform) * modelCount);
        memset(models, 0, sizeof(Model*) * modelCount);
        modelCount = 0;
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
