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

    void Material::SetDescriptors(Utils::StackArray<Utils::StackArray<Shader::Uniform<const void*>, MAX_PROPERTIES_COUNT>, 5>& uniformBindings)
    {
        auto device = VulkanDevice::GetDeviceInstance();

        // Create a descriptor set for our object transforms.

        // A layout binding must be created for each resource used by a shader. 
        // If we have two uniforms at different bindings (binding 0, 1, 2...etc), then we need
        // a new layout binding for each one. 

        size_t propertiesCount = propertiesArray.Count();
        VkDescriptorSetLayoutBinding layoutBindings[propertiesCount];
        
        descriptorSets.Allocate(propertiesArray.Count(), VK_NULL_HANDLE);
        layouts.Allocate(propertiesCount, VK_NULL_HANDLE);

        std::cout << "Allocating descriptor set storage of " << propertiesCount << std::endl;

        for (size_t i = 0; i < propertiesCount; i++)
        {
            layoutBindings[i] = Utils::Descriptor::CreateLayoutBinding(
                i, 
                1, 
                descriptorBindings[i].type,
                VK_SHADER_STAGE_VERTEX_BIT // TODO: Change this
            );

            std::cout << "Creating a layout binding for binding " << i << std::endl;

            // Create all layouts
            auto& binding = descriptorBindings.Get(i);
            
            SNEK_ASSERT(Utils::Descriptor::CreateLayout(device->Device(), OUT binding.layout, &layoutBindings[i], 1),
            "Failed to create descriptor set!");
        }

        std::cout << "Successfully created all required layouts!" << std::endl;

        size_t descriptorCount = 0;
        for (auto& binding : descriptorBindings)
        {
            auto& uniformArray = uniformBindings.Get(binding.binding);

            binding.descriptorCount = uniformArray.Count();
            
            std::cout << "Allocating " << binding.descriptorCount << " descriptor sets for binding " << binding.binding << std::endl;
            Utils::Descriptor::AllocateSets(device->Device(), &binding.descriptorSet, descriptorPool, uniformArray.Count(), &binding.layout);
            
            descriptorCount += uniformArray.Count();
            std::cout << "Binding count: " << binding.descriptorCount << std::endl;
            
            // TODO: Add offsets to an offsets array at the top level. 
            descriptorSets.Set(binding.binding, binding.descriptorSet);
        }

        std::cout << "Total descriptor sets: " <<  descriptorCount << std::endl;

        VkWriteDescriptorSet writeDescriptorSets[descriptorCount];
        size_t writtenDescriptors = 0;

        size_t offset = 0;
        for (auto& binding : descriptorBindings)
        {
            auto& uniformArray = uniformBindings.Get(binding.binding);

            auto& uniform = uniformArray.Get(0);

            auto bufferInfo = Utils::Descriptor::CreateBufferInfo(buffer.buffer, 0, uniform.size);
            descriptorOffsets.Append(Buffer::PadUniformBufferSize(offset));

            std::cout << "Offset for binding " << binding.binding << " is set to " << offset << "/" << bufferSize << std::endl;

            offset += uniform.size;

            std::cout << "Total size of all uniforms is " << offset << "/" << bufferSize << std::endl;

            writeDescriptorSets[writtenDescriptors] = Utils::Descriptor::CreateWriteSet(
                uniform.binding, 
                descriptorSets[binding.binding], 
                1, 
                (SnekVk::Utils::Descriptor::Type)binding.type, // TODO: change this to respond to multiple types
                bufferInfo
            );

            writtenDescriptors++;
        }     

        Utils::Descriptor::WriteSets(device->Device(), writeDescriptorSets, descriptorCount);
    }

    void Material::AddShader(Shader shader)
    {
        shaders.Append(shader);

        bufferSize += Buffer::PadUniformBufferSize(shader.GetUniformSize());

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

        // TODO: Extract this into another function
        u64 offset = 0;

        Utils::StackArray<Utils::StackArray<Shader::Uniform<const void*>, MAX_PROPERTIES_COUNT>, 5> uniformBindings;
        for (auto& shader : shaders)
        {
            auto uniforms = shader.GetUniformStructs();

            // TODO: Create a function that accepts an array of uniforms and
            // creates a descriptor set for each one. 

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

                if(!uniformBindings.Exists(uniform.binding)) 
                {
                    std::cout << "Added new binding at position: " << uniform.binding << std::endl;
                    uniformBindings.Activate(uniform.binding);
                } 
                
                auto& bindingList = uniformBindings.Get(uniform.binding);

                bindingList.Append(uniform);

                std::cout << "Added new uniform to binding: " << uniform.binding << std::endl;
                std::cout << "There are now " << bindingList.Count() << " sets in binding " << uniform.binding << std::endl;

                std::cout << "Added new property of size: " << uniform.size << " with buffer offset: " << offset << std::endl;
                offset += uniform.size;
            }
        }

        std::cout << "Total bindings: " << uniformBindings.Count() << std::endl;

        SetDescriptors(uniformBindings);

        VkDescriptorSetLayout layouts[descriptorBindings.Count()];
        for(size_t i = 0; i < descriptorBindings.Count(); i++)
        {
            layouts[i] = descriptorBindings[i].layout;
        }

        CreateLayout(layouts, descriptorBindings.Count());

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
