#pragma once

#include "../Core.h"
#include "../Pipeline/Pipeline.h"
#include "../Buffer/Buffer.h"
#include "../Shader/Shader.h"

namespace SnekVk
{
    class Material
    {
        public:

        static constexpr size_t MAX_SHADERS = 2;
        static constexpr size_t MAX_BINDINGS_PER_SHADER = 5;
        static constexpr size_t MAX_MATERIAL_BINDINGS = MAX_SHADERS * MAX_BINDINGS_PER_SHADER;
        
        enum PolygonMode
        {
            FILL = 0, 
            LINE = 1, 
            POINT = 2
        };

        Material();
        Material(Shader* vertexShader);
        Material(Shader* vertexShader, Shader* fragmentShader);

        Material(const Material&) = delete;
        Material& operator=(const Material&) = delete;

        ~Material();

        void CreateDescriptors();

        void SetPolygonMode(PolygonMode mode) { shaderSettings.mode = mode; }
        void BuildMaterial();

        static void DestroyDescriptorPool() 
        { 
            if (descriptorPool != VK_NULL_HANDLE) 
            {
                vkDestroyDescriptorPool(VulkanDevice::GetDeviceInstance()->Device(), descriptorPool, nullptr);
            }
        }

        void SetUniformData(VkDeviceSize dataSize, const void* data);
        void SetUniformData(Utils::StringId id, VkDeviceSize dataSize, const void* data);
        void SetUniformData(const char* name, VkDeviceSize dataSize, const void* data);

        void Bind(VkCommandBuffer commandBuffer);
        void CreatePipeline();
        void RecreatePipeline();

        static void BuildMaterials(std::initializer_list<Material*> materials);

        private:

        struct DescriptorBinding {
            VkDescriptorSetLayout layout {VK_NULL_HANDLE};
            VkDescriptorSet descriptorSet {VK_NULL_HANDLE};
            Shader::DescriptorType type;
        }; 

        struct Property
        {
            uint32_t binding = 0;
            Utils::StringId id = 0;
            VkShaderStageFlags stage;
            u64 offset = 0;
            u64 size = 0;
            size_t count = 0;
            DescriptorBinding descriptorBinding;
        };

        Material(Shader* vertexShader, Shader* fragmentShader, u32 shaderCount);

        void AddShader(Shader* shader);
        void SetShaderProperties(Shader* shader, u64& offset);

        void CreateLayout(
            VkDescriptorSetLayout* layouts = nullptr, 
            u32 layoutCount = 0, 
            VkPushConstantRange* pushConstants = nullptr, 
            u32 pushConstantCount = 0
        );

        struct {
            PolygonMode mode = PolygonMode::FILL;
        } shaderSettings;

        static VkDescriptorPool descriptorPool;

        size_t vertexCount = 0;
        u32 shaderCount = 0;

        Shader* vertexShader {nullptr};
        Shader* fragmentShader {nullptr};

        Utils::StackArray<Property, MAX_MATERIAL_BINDINGS> propertiesArray;

        Buffer::Buffer buffer;
        u64 bufferSize = 0;

        Utils::StackArray<VkDescriptorSet, MAX_MATERIAL_BINDINGS> descriptorSets;
        Utils::StackArray<u32, MAX_MATERIAL_BINDINGS> descriptorOffsets;

        Utils::StackArray<VertexDescription::Binding, MAX_MATERIAL_BINDINGS> vertexBindings;
        
        Pipeline pipeline;
        VkPipelineLayout pipelineLayout {VK_NULL_HANDLE};
    };
}
