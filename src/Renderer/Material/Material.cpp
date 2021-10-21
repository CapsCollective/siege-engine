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
        u32 uniformOffset = 0;
        if (descriptorSet) vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 1, &uniformOffset);
    }

    void Material::RecreatePipeline()
    {
        // Clear our graphics pipeline before swapchain re-creation
        pipeline.ClearPipeline();

        BuildMaterial();
    }

    void Material::SetDescriptor(Shader::Uniform<const void*>& uniform, VkShaderStageFlags stage, u64 offset)
    {
        // TODO: refactor this. 
        // Function should do the following:
        // 1. Allocate a buffer which stores the full size of all stored properties. 
        // 2. Create appropriate bindings and layouts.
        // 3. Write data to the sets.
        auto device = VulkanDevice::GetDeviceInstance();
        
        // TODO: Will need to find a way to find common uniforms across multiple shaders
        // will likely need to group them by stages

        // Create a descriptor set for our object transforms.

        // A layout binding must be created for each resorce used by a shader. 
        // If we have two uniforms at different bindings (binding 0, 1, 2...etc), then we need
        // a new layout binding for each one. 

        // TODO: iterate over all uniforms in the material and create a binding for each one. 
        // TODO: add all bindings to an array. All common bindings should be grouped together. 
        auto bufferBinding = Utils::Descriptor::CreateLayoutBinding(
            uniform.binding, 
            1, 
            Utils::Descriptor::STORAGE_DYNAMIC | Utils::Descriptor::UNIFORM_DYNAMIC, 
            stage
        );
        
        // TODO: Once all bindings are together, place them into an array and create the layout. 
        SNEK_ASSERT(Utils::Descriptor::CreateLayout(device->Device(), OUT layout, &bufferBinding, 1),
                "Failed to create descriptor set!");
        
        Utils::Descriptor::AllocateSets(device->Device(), descriptorSet, descriptorPool, 1, &layout);

        // TODO: Create an offset for each uniform in question.
        auto bufferInfo = Utils::Descriptor::CreateBufferInfo(buffer.buffer, offset, uniform.size);

        // TODO: Write a set for each bufferInfo.
        auto writeDescriptorSet = Utils::Descriptor::CreateWriteSet(
            uniform.binding, 
            descriptorSet, 
            1, 
            Utils::Descriptor::STORAGE_DYNAMIC, 
            bufferInfo
        );

        VkWriteDescriptorSet writeDescriptorSets[] = { writeDescriptorSet };

        // TODO: Write all the sets
        Utils::Descriptor::WriteSets(device->Device(), writeDescriptorSets, 1);
    }

    void Material::AddShader(Shader shader)
    {
        shaders.Append(shader);

        bufferSize += shader.GetUniformSize();

        std::cout << "Adding shader: " << shader.GetPath() << " with size: " << shader.GetUniformSize() << std::endl;
    }

    void Material::AddShaders(std::initializer_list<Shader> shaders)
    {
        for (auto& shader : shaders) AddShader(shader);
    }

    void Material::SetUniformData(VkDeviceSize dataSize, const void* data)
    {
        Buffer::CopyData(buffer, dataSize, data);
    }

    void Material::SetUniformData(Utils::StringId id, VkDeviceSize dataSize, const void* data)
    {
        // TODO: change this to use a dictionary
        for(auto& property : propertiesArray)
        {
            if (id == property.id) Buffer::CopyData(buffer, dataSize, data, property.offset);
        }
    }

    void Material::BuildMaterials(std::initializer_list<Material*> materials)
    {
        for (auto material : materials) material->BuildMaterial();
    }

    // TODO: Break this up into multiple smaller functions
    void Material::BuildMaterial()
    {
        // Allocate buffer which can store all the data we need
        Buffer::CreateBuffer(
            Buffer::PadUniformBufferSize(bufferSize),
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            OUT buffer.buffer,
            OUT buffer.bufferMemory
        );

        // TODO: Extract this into another function
        u64 offset = 0;

        for (auto& shader : shaders)
        {
            auto uniforms = shader.GetUniformStructs();

            for(size_t j = 0; j < uniforms.count; j++)
            {
                auto uniform = uniforms.data[j];

                SetDescriptor(uniform, (VkShaderStageFlags)shader.GetStage(), offset);

                propertiesArray.Append({uniform.id, offset, uniform.size, nullptr});
                std::cout << "Added new property of size: " << uniform.size << " with buffer offset: " << offset << std::endl;
                offset += uniform.size;
            }
        }

        if (layout != VK_NULL_HANDLE) CreateLayout(&layout, 1);

        SNEK_ASSERT(pipelineLayout != nullptr, "Cannot create pipeline without a valid layout!");

        size_t vertexCount = 0;

        // TODO: Precompute this value
        for (auto& shader: shaders)
        {
            vertexCount += shader.GetVertexBindings().count;
        }

        VertexDescription::Binding bindings[vertexCount];
        PipelineConfig::ShaderConfig shaderConfigs[shaders.Count()];

        for(size_t i = 0; i < shaders.Count(); i++)
        {
            auto shader = shaders[i];
            auto vertices = shader.GetVertexBindings();
            
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

            shaderConfigs[i] = { shader.GetPath(), shader.GetStage() };
        }

        auto pipelineConfig = Pipeline::DefaultPipelineConfig();
        pipelineConfig.rasterizationInfo.polygonMode = (VkPolygonMode)shaderSettings.mode;
        
        pipelineConfig.renderPass = SwapChain::GetInstance()->GetRenderPass()->GetRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        
        pipelineConfig.vertexData = VertexDescription::CreateDescriptions(vertexCount, bindings);

        pipeline.RecreatePipeline(
            shaderConfigs,
            static_cast<u32>(shaders.Count()),
            pipelineConfig
        );
    }
}
