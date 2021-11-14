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
        // An array of all offsets for these descriptor sets
        // TODO: Need to do the following: 
        // TODO: 1. Iterate over all bindings
        // TODO: 2. Extract all descriptorSets into a single array.
        // TODO: 3. Extract all dynamic offsets into a single array.
        // TODO: 4. Plug both into this function

        if (descriptorSets.Count() > 0) vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, descriptorSets.Count(), descriptorSets.Data(), descriptorOffsets.Count(), descriptorOffsets.Data());
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
        
        Utils::Descriptor::AllocateSets(device->Device(), &descriptorSet, descriptorPool, 1, &layout);

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

    void Material::SetDescriptors(Utils::StackArray<Utils::StackArray<Shader::Uniform<const void*>, MAX_PROPERTIES_COUNT>, 5>& uniformBindings)
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

        // A layout binding must be created for each resou rce used by a shader. 
        // If we have two uniforms at different bindings (binding 0, 1, 2...etc), then we need
        // a new layout binding for each one. 

        VkDescriptorSetLayoutBinding layoutBindings[uniformBindings.Count()];
        
        for (size_t i = 0; i < uniformBindings.Count(); i++)
        {
            auto& uniformArray = uniformBindings.Get(i);

            layoutBindings[i] = Utils::Descriptor::CreateLayoutBinding(
                i, 
                uniformArray.Count(), 
                Utils::Descriptor::STORAGE_DYNAMIC | Utils::Descriptor::UNIFORM_DYNAMIC, 
                VK_SHADER_STAGE_VERTEX_BIT // TODO: Change this
            );

            descriptorSets.Allocate(uniformArray.Count(), VK_NULL_HANDLE);

            std::cout << "Allocating descriptor set storage of " << uniformArray.Count() << std::endl;

            std::cout << "Creating a layout binding for binding " << i << " with " << uniformArray.Count() << " uniforms" << std::endl;
        }

        layouts.Allocate(uniformBindings.Count(), VK_NULL_HANDLE);

        for (size_t i = 0; i < uniformBindings.Count(); i++)
        {
            auto& binding = descriptorBindings.Get(i);
            SNEK_ASSERT(Utils::Descriptor::CreateLayout(device->Device(), OUT binding.layout, &layoutBindings[i], 1),
            "Failed to create descriptor set!");
        }

        std::cout << "Successfully created all required layouts!" << std::endl;

        // TODO: Need to find a way to package all relevant descriptor sets into a single array per
        // binding

        size_t descriptorCount = 0;
        for (auto& binding : descriptorBindings)
        {
            auto& uniformArray = uniformBindings.Get(binding.binding);

            binding.descriptorCount = uniformArray.Count();
            
            std::cout << "Allocating " << binding.descriptorCount << " descriptor sets for binding " << binding.binding << std::endl;
            Utils::Descriptor::AllocateSets(device->Device(), binding.descriptorSets, descriptorPool, uniformArray.Count(), &binding.layout);
            
            descriptorCount += uniformArray.Count();
            std::cout << "Binding count: " << binding.descriptorCount << std::endl;
            
            // TODO: Add offsets to an offsets array at the top level. 
            descriptorSets.Set(binding.binding, binding.descriptorSets[0]);
        }

        std::cout << "Total descriptor sets: " <<  descriptorCount << std::endl;

        VkWriteDescriptorSet writeDescriptorSets[descriptorCount];
        size_t writtenDescriptors = 0;

        size_t offset = 0;
        size_t totalSize = 0;
        for (auto& binding : descriptorBindings)
        {
            auto& uniformArray = uniformBindings.Get(binding.binding);

            auto& uniform = uniformArray.Get(0);

            std::cout << "Offset: " << offset << std::endl;
            std::cout << "Buffer Size: " << bufferSize << std::endl;
            std::cout << "Uniform Size: " << uniform.size << std::endl;

            auto bufferInfo = Utils::Descriptor::CreateBufferInfo(buffer.buffer, offset, uniform.size);
            descriptorOffsets.Append(Buffer::PadUniformBufferSize(offset));

            std::cout << "Offset for binding " << binding.binding << " is set to " << offset << "/" << bufferSize << std::endl;
            // TODO: Add this offset to an offsets array. 
            offset += uniform.size;
            totalSize += uniform.size;

            std::cout << "Total size of all uniforms is " << totalSize << "/" << bufferSize << std::endl;

            writeDescriptorSets[writtenDescriptors] = Utils::Descriptor::CreateWriteSet(
                uniform.binding, 
                descriptorSets[binding.binding], 
                1, 
                (SnekVk::Utils::Descriptor::Type)binding.type, // TODO: change this to respond to multiple types
                bufferInfo
            );

            writtenDescriptors++;
        }     

        std::cout << "offsets: " << descriptorOffsets.Count() << std::endl;
        std::cout << "sets: " << descriptorSets.Count() << std::endl;

        Utils::Descriptor::WriteSets(device->Device(), writeDescriptorSets, descriptorCount);
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

        // TODO: Need to get the full size of all descriptors so we can 
        // allocate an appropriate array.

        Utils::StackArray<Utils::StackArray<Shader::Uniform<const void*>, MAX_PROPERTIES_COUNT>, 5> uniformBindings;
        for (auto& shader : shaders)
        {
            auto uniforms = shader.GetUniformStructs();

            // TODO: Create a function that accepts an array of uniforms and
            // creates a descriptor set for each one. 

            for(size_t j = 0; j < uniforms.count; j++)
            {
                auto uniform = uniforms.data[j];

                //SetDescriptor(uniform, (VkShaderStageFlags)shader.GetStage(), offset);

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
