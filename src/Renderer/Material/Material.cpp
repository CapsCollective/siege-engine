#include "Material.h"
#include "../Mesh/Mesh.h"
#include "../Swapchain/Swapchain.h"

namespace SnekVk
{
    VkDescriptorPool Material::descriptorPool = {VK_NULL_HANDLE};

    Material::Material()
    {
        auto device = VulkanDevice::GetDeviceInstance();

        if (descriptorPool == VK_NULL_HANDLE) 
        {
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

            SNEK_ASSERT(vkCreateDescriptorPool(device->Device(), &poolCreateInfo, nullptr, &descriptorPool) == VK_SUCCESS,
                "Unable to create descriptor pool!");
        }
    }

    Material::~Material() 
    {
        auto device = VulkanDevice::GetDeviceInstance();

        vkDestroyDescriptorSetLayout(device->Device(), layout, nullptr);
        vkDestroyPipelineLayout(device->Device(), pipelineLayout, nullptr);
        Buffer::DestroyBuffer(buffer);
    }

    void Material::CreateLayout(
            VkDescriptorSetLayout* layouts, 
            u32 layoutCount, 
            VkPushConstantRange* pushConstants, 
            u32 pushConstantCount) 
    {
        auto device = VulkanDevice::GetDeviceInstance();

        PipelineConfig::CreatePipelineLayout(
            device->Device(), 
            OUT &pipelineLayout, 
            layouts, 
            layoutCount, 
            pushConstants, 
            pushConstantCount);
    }

    void Material::Bind(VkCommandBuffer commandBuffer)
    {
        pipeline.Bind(commandBuffer);
        if (descriptorSet) vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
    }

    void Material::RecreatePipeline()
    {
        // Clear our graphics pipeline before swapchain re-creation
            pipeline.ClearPipeline();

            BuildMaterial();
    }

    void Material::SetDescriptor(Descriptor descriptor)
    {
        auto device = VulkanDevice::GetDeviceInstance();

        Buffer::CreateBuffer(
            descriptor.size,
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            OUT buffer.buffer,
            OUT buffer.bufferMemory
        );

        // Create a descriptor set for our object transforms.

        VkDescriptorSetLayoutBinding bufferBinding {};
        bufferBinding.binding = 0;
        bufferBinding.descriptorCount = 1;
        bufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        bufferBinding.stageFlags = (VkShaderStageFlags)descriptor.stage;

        VkDescriptorSetLayoutCreateInfo layoutCreateInfo{};
        layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutCreateInfo.pNext = nullptr;

        layoutCreateInfo.bindingCount = 1;
        layoutCreateInfo.flags = 0;
        layoutCreateInfo.pBindings = &bufferBinding;

        SNEK_ASSERT(vkCreateDescriptorSetLayout(device->Device(), &layoutCreateInfo, nullptr, &layout) == VK_SUCCESS,
                "Failed to create descriptor set!");
        
        // Object transform allocate info
        VkDescriptorSetAllocateInfo allocateInfo {};
        allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocateInfo.pNext = nullptr;
        allocateInfo.descriptorPool = descriptorPool;
        allocateInfo.descriptorSetCount = 1;
        allocateInfo.pSetLayouts = &layout;
        
        vkAllocateDescriptorSets(device->Device(), &allocateInfo, &descriptorSet);

        VkDescriptorBufferInfo bufferInfo {};
        bufferInfo.buffer = buffer.buffer;
        bufferInfo.offset = 0;
        bufferInfo.range = descriptor.size;

        VkWriteDescriptorSet writeDescriptorSet {};
        writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet.pNext = nullptr;
        writeDescriptorSet.dstBinding = 0;
        writeDescriptorSet.dstSet = descriptorSet;
        writeDescriptorSet.descriptorCount = 1;
        writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        writeDescriptorSet.pBufferInfo = &bufferInfo;

        VkWriteDescriptorSet writeDescriptorSets[] = {writeDescriptorSet};

        vkUpdateDescriptorSets(device->Device(), 1, writeDescriptorSets, 0, nullptr);
    }

    void Material::AddVertexAttribute(u32 binding, u32 offset, VertexDescription::AttributeType type)
    {
        SNEK_ASSERT(vertexStorage.count < vertexStorage.MAX_COUNT, 
                std::string("Too many vertex attributes assigned. Max is ")
                .append(std::to_string(vertexStorage.MAX_COUNT)).c_str()); 
        
        SNEK_ASSERT(binding < vertexStorage.MAX_COUNT, 
                "Cannot assign attributes to more than 5 bindings!");
        
        u32 attributeCount = vertexStorage.data[binding].attributeCount;

        if (attributeCount == 0) vertexStorage.count++;

        vertexStorage.data[binding].attributes[attributeCount] = {offset, type};
        vertexStorage.data[binding].attributeCount++;
    }

    void Material::SetVertexInputSize(u32 binding, u32 stride)
    {
        SNEK_ASSERT(binding < vertexStorage.MAX_COUNT, 
                "Cannot assign attributes to more than 5 bindings!");
        
        vertexStorage.data[binding].vertexStride = stride;
    }

    void Material::AddShader(const char* filePath, PipelineConfig::PipelineStage stage)
    {
        SNEK_ASSERT(shaderStorage.count < shaderStorage.MAX_COUNT, 
                std::string("Too many shaders assigned. Max is ")
                .append(std::to_string(shaderStorage.MAX_COUNT)).c_str());
        
        shaderStorage.data[shaderStorage.count] = {filePath, stage};
        shaderStorage.count++;
    }

    void Material::BuildMaterial()
    {
        if (layout != VK_NULL_HANDLE) CreateLayout(&layout, 1);

        SNEK_ASSERT(pipelineLayout != nullptr, "Cannot create pipeline without a valid layout!");

        VertexDescription::Binding bindings[vertexStorage.count];

        //Need to find a way to configure vertex data into the material.

        for (u32 i = 0; i < vertexStorage.count; i++)
        {
            auto attributes = vertexStorage.data[i];

            if (attributes.attributeCount == 0) continue;

            bindings[i] = VertexDescription::CreateBinding(
                    i, 
                    attributes.vertexStride, 
                    VertexDescription::VERTEX, 
                    attributes.attributes, 
                    attributes.attributeCount);
        }

        auto pipelineConfig = Pipeline::DefaultPipelineConfig();
        pipelineConfig.rasterizationInfo.polygonMode = (VkPolygonMode)shaderSettings.mode;
        
        pipelineConfig.renderPass = SwapChain::GetInstance()->GetRenderPass()->GetRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        
        pipelineConfig.vertexData = VertexDescription::CreateDescriptions(vertexStorage.count, bindings);

        pipeline.RecreatePipeline(
            shaderStorage.data,
            static_cast<u32>(shaderStorage.count),
            pipelineConfig
        );
    }
}
