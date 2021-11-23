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

        for (auto& binding : descriptorBindings)
        {
            vkDestroyDescriptorSetLayout(device->Device(), binding.layout, nullptr);
        }
        
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

        if (descriptorSets.Count() > 0) vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, descriptorSets.Count(), descriptorSets.Data(), descriptorOffsets.Count(), descriptorOffsets.Data());
    }

    void Material::RecreatePipeline()
    {
        // Clear our graphics pipeline before swapchain re-creation
        pipeline.ClearPipeline();

        BuildMaterial();
    }

    // BUG: If a descriptor is set for multiple shaders this entire process falls apart.
    // TODO: Find a way to resolve the bug above.
    // TODO: Find a way to sort bindings by number and shader stage
    void Material::SetDescriptors()
    {
        auto device = VulkanDevice::GetDeviceInstance();

        // Create a descriptor set for our object transforms.

        // A layout binding must be created for each resource used by a shader. 
        // If we have two uniforms at different bindings (binding 0, 1, 2...etc), then we need
        // a new layout binding for each one. 

        size_t propertiesCount = propertiesArray.Count();
        VkDescriptorSetLayoutBinding layoutBindings[propertiesCount];
        VkDescriptorBufferInfo bufferInfos[propertiesCount];
        
        descriptorSets.Allocate(propertiesArray.Count(), VK_NULL_HANDLE);

        std::cout << "Allocating descriptor set storage of " << propertiesCount << std::endl;

        for (size_t i = 0; i < propertiesCount; i++)
        {
            auto& property = propertiesArray.Get(i);

            layoutBindings[i] = Utils::Descriptor::CreateLayoutBinding(
                i, 
                1, 
                descriptorBindings[i].type,
                VK_SHADER_STAGE_VERTEX_BIT // TODO: Change this to respond to different stages
            );

            std::cout << "Creating a layout binding for binding " << i << std::endl;

            // Create all layouts
            auto& binding = descriptorBindings.Get(i);
            
            SNEK_ASSERT(Utils::Descriptor::CreateLayout(device->Device(), OUT binding.layout, &layoutBindings[i], 1),
            "Failed to create descriptor set!");

            auto& uniform = propertiesArray.Get(i);

            bufferInfos[i] = Utils::Descriptor::CreateBufferInfo(buffer.buffer, 0, uniform.size);
            descriptorOffsets.Append(Buffer::PadUniformBufferSize(property.offset));

            std::cout << "Allocating descriptor set for binding " << binding.binding << std::endl;
            Utils::Descriptor::AllocateSets(device->Device(), &binding.descriptorSet, descriptorPool, 1, &binding.layout);

            descriptorSets.Set(binding.binding, binding.descriptorSet);
        }

        std::cout << "Successfully created all required layouts!" << std::endl;

        std::cout << "Total descriptor sets: " <<  descriptorBindings.Count() << std::endl;

        VkWriteDescriptorSet writeDescriptorSets[descriptorBindings.Count()];
        
        for(size_t i = 0; i < descriptorBindings.Count(); i++) {
            auto& binding = descriptorBindings.Get(i);
            writeDescriptorSets[i] = Utils::Descriptor::CreateWriteSet(
                i, 
                descriptorSets[binding.binding], 
                1, 
                (SnekVk::Utils::Descriptor::Type)binding.type, // TODO: change this to respond to multiple types
                bufferInfos[i]
            );
        }    

        Utils::Descriptor::WriteSets(device->Device(), writeDescriptorSets, descriptorBindings.Count());
    }

    void Material::AddShader(Shader shader)
    {
        shaders.Append(shader);

        bufferSize += Buffer::PadUniformBufferSize(shader.GetUniformSize());

        vertexCount += shader.GetVertexBindings().count;

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
            bufferSize,
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            OUT buffer.buffer,
            OUT buffer.bufferMemory
        );

        VertexDescription::Binding bindings[vertexCount];
        PipelineConfig::ShaderConfig shaderConfigs[shaders.Count()];

        // TODO: Extract this into another function
        u64 offset = 0;

        for (size_t i = 0; i < shaders.Count(); i++)
        {
            auto shader = shaders.Get(i);
            auto vertices = shader.GetVertexBindings();

            for (size_t j = 0; j < vertices.count; j++)
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

            auto uniforms = shader.GetUniformStructs();

            for(size_t j = 0; j < uniforms.count; j++)
            {
                auto uniform = uniforms.data[j];

                propertiesArray.Append({uniform.id, (VkShaderStageFlags)shader.GetStage(), offset,  uniform.size, nullptr});

                if (!descriptorBindings.Exists(uniform.binding))
                {
                    std::cout << "Added new binding at position: " << uniform.binding << std::endl;
                    descriptorBindings.Activate(uniform.binding);
                    descriptorBindings.Get(uniform.binding).binding = uniform.binding;
                    descriptorBindings.Get(uniform.binding).type = (DescriptorType)uniform.type;
                }

                std::cout << "Added new uniform to binding: " << uniform.binding << std::endl;

                std::cout << "Added new property of size: " << uniform.size << " with buffer offset: " << offset << std::endl;
                offset += uniform.size;
            }
        }

        std::cout << "Total bindings: " << descriptorBindings.Count() << std::endl;

        SetDescriptors();

        VkDescriptorSetLayout layouts[descriptorBindings.Count()];
        for(size_t i = 0; i < descriptorBindings.Count(); i++)
        {
            layouts[i] = descriptorBindings[i].layout;
        }

        CreateLayout(layouts, descriptorBindings.Count());

        SNEK_ASSERT(pipelineLayout != nullptr, "Cannot create pipeline without a valid layout!");

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

    Material::MaterialBuilder& Material::MaterialBuilder::WithVertexShader(ShaderBuilder* vertexShader)
    {
        this->vertexShader = vertexShader;

        AddShader(vertexShader);

        return *this;
    }

    Material::MaterialBuilder& Material::MaterialBuilder::WithFragmentShader(ShaderBuilder* fragmentShader)
    {
        this->fragmentShader = fragmentShader;

        AddShader(fragmentShader);

        return *this;
    }

    void Material::MaterialBuilder::AddShader(ShaderBuilder* shader)
    {
        bufferSize += Buffer::PadUniformBufferSize(shader->GetUniformSize());

        auto& vertices = shader->GetVertexBindings();

        vertexCount += vertices.Count();
        
        shaderCount++;

        for(size_t i = 0; i < vertices.Count(); i++)
        {
            auto& binding = vertices.Get(i);
            auto& attributes = binding.attributes;

            if (attributes.Count() == 0) continue;

            bindings.Append(
                VertexDescription::CreateBinding(
                i, 
                binding.vertexStride, 
                VertexDescription::VERTEX, 
                attributes.Data(), 
                attributes.Count()
            ));
        }

        shaderConfigs.Append({ shader->GetPath(), shader->GetStage() });

        std::cout << "Added shader: " << shader->GetPath() << " with " << vertices.Count() << " vertex bindings" << std::endl;
        std::cout << "Total bindings: " << bindings.Count() << std::endl;
    }

    Material::MaterialBuilder& Material::MaterialBuilder::WithPolygonMode(PolygonMode mode) 
    {
        shaderSettings.mode = mode;

        return *this;
    }
}
