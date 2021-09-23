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
        
        struct Descriptor
        {
            PipelineConfig::PipelineStage stage; 
            u64 size;
        };

        Material();

        Material(const Material&) = delete;
        Material& operator=(const Material&) = delete;

        ~Material();

        void SetDescriptor(Descriptor descriptor);
        void SetPolygonMode(PolygonMode mode) { shaderSettings.mode = mode; }
        void BuildMaterial();

        void AddVertexAttribute(u32 binding, u32 offset, VertexDescription::AttributeType type);
        void SetVertexInputSize(u32 binding, u32 offset);
        void AddShader(const char* filePath, PipelineConfig::PipelineStage stage);
        void AddShader(Shader shader);

        static void DestroyDescriptorPool() 
        { 
            if (descriptorPool != VK_NULL_HANDLE) 
            {
                vkDestroyDescriptorPool(VulkanDevice::GetDeviceInstance()->Device(), descriptorPool, nullptr);
            }
        }

        void SetUniformData(VkDeviceSize dataSize, const void* data);

        void Bind(VkCommandBuffer commandBuffer);
        void RecreatePipeline();

        private:
        
        // TODO: Change this to std::array?
        template<typename T, size_t S>
        struct Storage
        {
            static constexpr size_t MAX_COUNT = S;
            size_t count = 0; 
            T data[MAX_COUNT];
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

        Storage<VertexBinding, 5> vertexStorage;
        Storage<Shader, 5> shaders;
        Storage<PipelineConfig::ShaderConfig, 5> shaderStorage;

        Buffer::Buffer buffer;

        VkDescriptorSetLayout layout {VK_NULL_HANDLE}; 
        VkDescriptorSet descriptorSet {VK_NULL_HANDLE};

        Pipeline pipeline;
        VkPipelineLayout pipelineLayout {VK_NULL_HANDLE};
    };
}