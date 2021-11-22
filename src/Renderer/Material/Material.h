#pragma once

#include "../Core.h"
#include "../Pipeline/Pipeline.h"
#include "../Buffer/Buffer.h"
#include "../Shader/Shader.h"
#include "../Shader/Shader2.h"
#include "../Utils/Descriptor.h"

namespace SnekVk
{
    class Material
    {
        public:

        enum DescriptorType
        {
            UNIFORM = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 
            STORAGE = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
        };

        enum PolygonMode
        {
            FILL = 0, 
            LINE = 1, 
            POINT = 2
        };

        // TODO: Finish this class
        class MaterialBuilder
        {
            public: 

            MaterialBuilder() = default;

            ~MaterialBuilder() {};

            MaterialBuilder& WithVertexShader(ShaderBuilder* vertexShader);
            MaterialBuilder& WithFragmentShader(ShaderBuilder* fragmentShader);

            MaterialBuilder& WithPolygonMode(PolygonMode mode); 

            private: 

            struct {
                PolygonMode mode = PolygonMode::FILL;
            } shaderSettings;

            ShaderBuilder* vertexShader {nullptr};
            ShaderBuilder* fragmentShader {nullptr};

            u64 bufferSize;
            size_t vertexCount = 0;
            size_t shaderCount = 0;
        };

        static constexpr size_t MAX_SHADER_COUNT = 5;
        static constexpr size_t MAX_PROPERTIES_COUNT = 10;
        static constexpr uint32_t MAX_SETS_PER_BINDING = 5;

        Material();

        Material(const Material&) = delete;
        Material& operator=(const Material&) = delete;

        ~Material();

        void SetDescriptors();

        void SetPolygonMode(PolygonMode mode) { shaderSettings.mode = mode; }
        void BuildMaterial();

        void AddShader(Shader shader);
        void AddShader(ShaderBuilder shader);
        void AddShaders(std::initializer_list<Shader> shaders);

        static void DestroyDescriptorPool() 
        { 
            if (descriptorPool != VK_NULL_HANDLE) 
            {
                vkDestroyDescriptorPool(VulkanDevice::GetDeviceInstance()->Device(), descriptorPool, nullptr);
            }
        }

        void SetUniformData(VkDeviceSize dataSize, const void* data);
        void SetUniformData(Utils::StringId id, VkDeviceSize dataSize, const void* data);

        void Bind(VkCommandBuffer commandBuffer);
        void RecreatePipeline();

        static void BuildMaterials(std::initializer_list<Material*> materials);

        private:

        // Add stage and dynamic offset to this
        struct DescriptorBinding {
            uint32_t binding = 0;
            VkDescriptorSetLayout layout {VK_NULL_HANDLE};
            VkDescriptorSet descriptorSet;
            DescriptorType type;
        }; 

        struct Property
        {
            Utils::StringId id;
            VkShaderStageFlags stage;
            u64 offset;
            u64 size;
            const void* data;
        };

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

        Utils::StackArray<Shader, MAX_SHADER_COUNT> shaders;
        // TODO: Split properties into their shader stages
        Utils::StackArray<Property, MAX_PROPERTIES_COUNT> propertiesArray;

        Buffer::Buffer buffer;
        u64 bufferSize = 0;

        Utils::StackArray<VkDescriptorSet, MAX_PROPERTIES_COUNT> descriptorSets;
        Utils::StackArray<u32, MAX_PROPERTIES_COUNT> descriptorOffsets;

        Utils::StackArray<DescriptorBinding, MAX_PROPERTIES_COUNT> descriptorBindings;
        
        Pipeline pipeline;
        VkPipelineLayout pipelineLayout {VK_NULL_HANDLE};
    };
}
