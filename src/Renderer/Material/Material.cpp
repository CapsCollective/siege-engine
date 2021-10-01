#include "Material.h"
#include "../Mesh/Mesh.h"
#include "../Swapchain/Swapchain.h"
#include "../Utils/Descriptor.h"

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
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 10},
                {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 10},
                {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 10}
            };

            VkDescriptorPoolCreateInfo poolCreateInfo {};
            poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolCreateInfo.flags = 0;
            poolCreateInfo.maxSets = 10;
            poolCreateInfo.poolSizeCount = 4;
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

        auto bufferBinding = Utils::Descriptor::CreateLayoutBinding(
            0, 
            1, 
            Utils::Descriptor::STORAGE, 
            (VkShaderStageFlags)descriptor.stage
        );

        SNEK_ASSERT(Utils::Descriptor::CreateLayout(device->Device(), OUT layout, &bufferBinding, 1),
                "Failed to create descriptor set!");
        
        Utils::Descriptor::AllocateSets(device->Device(), descriptorSet, descriptorPool, 1, &layout);

        auto bufferInfo = Utils::Descriptor::CreateBufferInfo(buffer.buffer, 0, descriptor.size);

        // TODO: Set this up to create bindings for different sets. 
        // Will likely need to create a container for descriptors that 
        // can sort them based on the uniforms defined by the shader.

        // something like:
        // struct DescriptorStorage {
        //     ...   
        // }

        // Will then need to add the descriptor to the buffer at a specific offset.

        auto writeDescriptorSet = Utils::Descriptor::CreateWriteSet(
            0, 
            descriptorSet, 
            1, 
            Utils::Descriptor::STORAGE, 
            bufferInfo
        );

        VkWriteDescriptorSet writeDescriptorSets[] = { writeDescriptorSet };

        Utils::Descriptor::WriteSets(device->Device(), writeDescriptorSets, 1);
    }

    void Material::AddShader(Shader shader)
    {
        SNEK_ASSERT(shaders.count < shaders.MAX_COUNT, 
            std::string("Too many shaders assigned. Max is ")
            .append(std::to_string(shaders.MAX_COUNT)).c_str());

        auto uniforms = shader.GetUniformStructs();

        for(size_t i = 0; i < uniforms.count; i++)
        {
            auto uniform = uniforms.data[i];

            SetDescriptor({shader.GetStage(), uniform.size });

            if (properties.count < properties.MAX_COUNT)
            {
                size_t count = properties.count;
                properties.data[count] = {uniform.id, count, uniform.size, nullptr};
                std::cout << "Added new property of size: " << uniform.size << " with buffer offset: " << count << std::endl;
                properties.count++;
            }
        }

        shaders.data[shaders.count] = shader;
        shaders.count++;
    }

    void Material::AddShaders(std::initializer_list<Shader> shaders)
    {
        for (auto& shader : shaders) AddShader(shader);
    }

    void Material::SetUniformData(VkDeviceSize dataSize, const void* data)
    {
        Buffer::CopyData(buffer, dataSize, data);
    }

    void Material::BuildMaterials(std::initializer_list<Material*> materials)
    {
        for (auto material : materials) material->BuildMaterial();
    }

    void Material::BuildMaterial()
    {
        if (layout != VK_NULL_HANDLE) CreateLayout(&layout, 1);

        SNEK_ASSERT(pipelineLayout != nullptr, "Cannot create pipeline without a valid layout!");

        size_t vertexCount = 0;

        for(size_t i = 0; i < shaders.count; i++)
        {
            auto vertices = shaders.data[i].GetVertexBindings();

            vertexCount += vertices.count;
        }

        VertexDescription::Binding bindings[vertexCount];
        PipelineConfig::ShaderConfig shaderConfigs[shaders.count];

        for(size_t i = 0; i < shaders.count; i++)
        {
            auto vertices = shaders.data[i].GetVertexBindings();
            
            for (u32 j = 0; j < vertices.count; j++)
            {
                auto attributes = vertices.data[j];

                if (attributes.attributeCount == 0) continue;

                bindings[j] = VertexDescription::CreateBinding(
                        j, 
                        attributes.vertexStride, 
                        VertexDescription::VERTEX, 
                        attributes.attributes, 
                        attributes.attributeCount);
            }

            shaderConfigs[i] = { shaders.data[i].GetPath(), shaders.data[i].GetStage() };
        }

        auto pipelineConfig = Pipeline::DefaultPipelineConfig();
        pipelineConfig.rasterizationInfo.polygonMode = (VkPolygonMode)shaderSettings.mode;
        
        pipelineConfig.renderPass = SwapChain::GetInstance()->GetRenderPass()->GetRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        
        pipelineConfig.vertexData = VertexDescription::CreateDescriptions(vertexCount, bindings);

        pipeline.RecreatePipeline(
            shaderConfigs,
            static_cast<u32>(shaders.count),
            pipelineConfig
        );
    }
}
