#include "Material.h"
#include "../Mesh/Mesh.h"
#include "../Swapchain/Swapchain.h"
#include "../Utils/Descriptor.h"

namespace SnekVk
{
    Material::Material()
        : Material(nullptr, nullptr, 0) {}

    Material::Material(Shader* vertexShader) 
        : Material(vertexShader, nullptr, 1) 
    {
        SNEK_ASSERT(vertexShader != nullptr, 
            "Error: the vertex shader must not be null when using this constructor");

        bufferSize += Buffer::PadUniformBufferSize(vertexShader->GetUniformSize());
    }
    
    Material::Material(Shader* vertexShader, Shader* fragmentShader)
        : Material(vertexShader, fragmentShader, 2) 
    {
        SNEK_ASSERT(vertexShader != nullptr && fragmentShader != nullptr, 
            "Error: the vertex and fragment shaders must not be null when using this constructor");
        
        bufferSize +=
            (Buffer::PadUniformBufferSize(vertexShader->GetUniformSize())
            + Buffer::PadUniformBufferSize(fragmentShader->GetUniformSize()));
    }

    Material::Material(Shader* vertexShader, Shader* fragmentShader, u32 shaderCount)
        : vertexShader(vertexShader), fragmentShader(fragmentShader), shaderCount(shaderCount)
    {}

    void Material::SetVertexShader(Shader* shader) 
    {
        vertexShader = shader; 
        bufferSize += Buffer::PadUniformBufferSize(shader->GetUniformSize());
    }

    void Material::SetFragmentShader(Shader* shader) 
    {
        fragmentShader = shader; 
        bufferSize += Buffer::PadUniformBufferSize(shader->GetUniformSize());
    }

    Material::~Material() 
    {
        auto device = VulkanDevice::GetDeviceInstance();

        for (auto& property : propertiesArray)
        {
            vkDestroyDescriptorSetLayout(device->Device(), property.descriptorBinding.layout, nullptr);
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

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, descriptorSets.Count(), descriptorSets.Data(), descriptorOffsets.Count(), descriptorOffsets.Data());
    }

    void Material::RecreatePipeline()
    {
        // Clear our graphics pipeline before swapchain re-creation
        pipeline.ClearPipeline();

        CreatePipeline();
    }

    void Material::CreatePipeline()
    {
        Utils::StackArray<PipelineConfig::ShaderConfig, MAX_SHADERS> shaderConfigs;

        if (vertexShader) shaderConfigs.Append({ vertexShader->GetPath(), vertexShader->GetStage() });
        if (fragmentShader) shaderConfigs.Append({ fragmentShader->GetPath(), fragmentShader->GetStage() });

        size_t bindingCount = propertiesArray.Count();

        VkDescriptorSetLayout layouts[bindingCount];

        for(size_t i = 0; i < bindingCount; i++)
        {
            layouts[i] = propertiesArray.Get(i).descriptorBinding.layout;
        }

        CreateLayout(layouts, bindingCount);

        SNEK_ASSERT(pipelineLayout != nullptr, "Cannot create pipeline without a valid layout!");
        
        // TODO(Aryeh): Maybe pipelineConfig should be a builder class?

        auto pipelineConfig = Pipeline::DefaultPipelineConfig();
        pipelineConfig.rasterizationInfo.polygonMode = (VkPolygonMode)shaderSettings.mode;
        
        pipelineConfig.renderPass = SwapChain::GetInstance()->GetRenderPass()->GetRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        
        pipelineConfig.vertexData = VertexDescription::CreateDescriptions(vertexCount, vertexBindings.Data());

        pipeline.RecreatePipeline(
            shaderConfigs.Data(),
            shaderCount,
            pipelineConfig
        );
    }

    // NOTE(Aryeh): At some point it might be useful to batch create
    // bindings. To do this we'd need to sort all bindings by type, stage, and 
    // binding. 
    
    // NOTE(Aryeh): This could potentially break if we have multiple 
    // uniforms in the same dynamic uniform or storage buffer. 
    void Material::CreateDescriptors()
    {
        auto device = VulkanDevice::GetDeviceInstance();

        auto descriptorPool = DescriptorPool::GetDescriptorPool();

        // Create a descriptor set for our object transforms.

        // A layout binding must be created for each resource used by a shader. 
        // If we have two uniforms at different bindings (binding 0, 1, 2...etc), then we need
        // a new layout binding for each one. 

        size_t propertiesCount = propertiesArray.Count();

        VkWriteDescriptorSet writeDescriptorSets[propertiesCount];
        VkDescriptorBufferInfo bufferInfos[propertiesCount];

        std::cout << "Allocating descriptor set storage of " << propertiesCount << std::endl;

        for (size_t i = 0; i < propertiesCount; i++)
        {
            auto& property = propertiesArray.Get(i);
            auto& binding = property.descriptorBinding;

            // TODO: bindings MUST be unique for all bound shaders. This means that
            // TODO: new bindings on a new shader must follow a consecutive order.
            auto layoutBinding = Utils::Descriptor::CreateLayoutBinding(
                property.binding, 
                1, 
                binding.type,
                property.stage
            );

            auto stage = property.stage == VK_SHADER_STAGE_VERTEX_BIT ? "vertex" : 
                property.stage == VK_SHADER_STAGE_FRAGMENT_BIT ? "fragment" : "unknown";

            std::cout << "Creating a layout binding for binding " << property.binding << " at stage: " << stage << std::endl;

            // Create all layouts
            
            SNEK_ASSERT(Utils::Descriptor::CreateLayout(device->Device(), OUT binding.layout, &layoutBinding, 1),
            "Failed to create descriptor set!");

            u64 offset = property.offset;

            std::cout << property.size + offset << std::endl;

            bufferInfos[i] = Utils::Descriptor::CreateBufferInfo(buffer.buffer, offset, property.size);

            std::cout << "Allocating descriptor set for binding " << property.binding << std::endl;
            Utils::Descriptor::AllocateSets(device->Device(), &binding.descriptorSet, descriptorPool, 1, &binding.layout);

            descriptorSets.Append(binding.descriptorSet);

            writeDescriptorSets[i] = Utils::Descriptor::CreateWriteSet(
                property.binding, 
                binding.descriptorSet, 
                1, 
                (VkDescriptorType)binding.type,
                bufferInfos[i]
            );
        }

        std::cout << "Successfully created all required layouts!" << std::endl;

        std::cout << "Total descriptor sets: " <<  descriptorSets.Count() << std::endl;

        Utils::Descriptor::WriteSets(device->Device(), writeDescriptorSets, propertiesCount);
    }

    void Material::AddShader(Shader* shader)
    {
        auto& vertices = shader->GetVertexBindings();

        vertexCount += vertices.Count();

        for(size_t i = 0; i < vertices.Count(); i++)
        {
            auto& binding = vertices.Get(i);
            auto& attributes = binding.attributes;

            if (attributes.Count() == 0) continue;

            vertexBindings.Append(
                VertexDescription::CreateBinding(
                i, 
                binding.vertexStride, 
                VertexDescription::VERTEX, 
                attributes.Data(), 
                attributes.Count()
            ));
        }
    }

    void Material::SetShaderProperties(Shader* shader, u64& offset)
    {
        auto uniforms = shader->GetUniforms();

        for(auto& uniform : uniforms)
        {
            if (HasProperty(uniform.id))
            {
                std::cout << "Property already exists!" << std::endl;
                auto& property = GetProperty(uniform.id);
                property.stage = property.stage | (VkShaderStageFlags) shader->GetStage();
                continue;
            };

            Property property = {
                uniform.binding, 
                uniform.id, 
                (VkShaderStageFlags)shader->GetStage(), 
                offset,  
                uniform.size, 
                uniform.dynamicCount
            };

            property.descriptorBinding = { VK_NULL_HANDLE, VK_NULL_HANDLE, (Shader::DescriptorType)uniform.type };
            propertiesArray.Append(property);

            std::cout << "Added new uniform to binding: " << uniform.binding << std::endl;

            std::cout << "Added new property of size: " << uniform.size << " with buffer offset: " << offset << std::endl;

            offset += (uniform.size * uniform.arraySize) * uniform.dynamicCount;
        }
    }

    void Material::SetUniformData(VkDeviceSize dataSize, const void* data)
    {
        Buffer::CopyData(buffer, dataSize, data);
    }

    void Material::SetUniformData(Utils::StringId id, VkDeviceSize dataSize, const void* data)
    {
        for(auto& property : propertiesArray)
        {
            if (id == property.id) {
                Buffer::CopyData(buffer, dataSize, data, property.offset);
                return;
            }
        }
    }

    bool Material::HasProperty(Utils::StringId id)
    {
        for(auto& property : propertiesArray)
        {
            if (id == property.id) {
                return true;
            }
        }
        return false;
    }

    void Material::SetUniformData(const char* name, VkDeviceSize dataSize, const void* data)
    {
        auto id = INTERN_STR(name);

        for(auto& property : propertiesArray)
        {
            if (id == property.id) {
                Buffer::CopyData(buffer, dataSize, data, property.offset);
                return;
            }
        }
    }

    void Material::BuildMaterials(std::initializer_list<Material*> materials)
    {
        for (auto material : materials) material->BuildMaterial();
    }

    Material::Property& Material::GetProperty(Utils::StringId id)
    {
        for(auto& property : propertiesArray)
        {
            if (id == property.id) return property;
        }

        SNEK_ASSERT(false, "No property with ID: " << id << " exists!");
    }

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

        u64 offset = 0;

        if (vertexShader)
        {
            AddShader(vertexShader);
            SetShaderProperties(vertexShader, OUT offset);
        }

        if (fragmentShader)
        {
            AddShader(fragmentShader);
            SetShaderProperties(fragmentShader, OUT offset);
        }
        
        std::cout << "Total properties: " << propertiesArray.Count() << std::endl;

        CreateDescriptors();

        CreatePipeline();

        std::cout << "Built material with size: " << bufferSize << std::endl;
    }
}
