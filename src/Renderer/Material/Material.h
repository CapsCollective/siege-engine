#pragma once

#include "../Core.h"
#include "../Pipeline/Pipeline.h"
#include "../Buffer/Buffer.h"

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
        
        struct Descriptor
        {
            PipelineConfig::PipelineStage stage; 
            u64 size;
        };

        struct AttributeStorage
        {
            static constexpr size_t MAX_VERTEX_ATTRIBUTES = 10;
            size_t attributeCount = 0;
            VertexDescription::Attribute attributes[MAX_VERTEX_ATTRIBUTES];
        };

        Material();

        Material(const Material&) = delete;
        Material& operator=(const Material&) = delete;

        ~Material();

        void SetDescriptor(Descriptor descriptor);
        void BuildMaterial();
        void AddVertexAttribute(u32 offset, VertexDescription::AttributeType type);

        static void DestroyDescriptorPool() 
        { 
            if (descriptorPool != VK_NULL_HANDLE) 
            {
                vkDestroyDescriptorPool(VulkanDevice::GetDeviceInstance()->Device(), descriptorPool, nullptr);
            }
        }

        template<typename T>
        void SetUniformData(VkDeviceSize dataSize, const T* data)
        {
            Buffer::CopyData<T>(buffer, dataSize, data);
        }

        void Bind(VkCommandBuffer commandBuffer);
        void RecreatePipeline();

        private:

        void CreateDescriptors();
        void CreateLayout(
            VkDescriptorSetLayout* layouts = nullptr, 
            u32 layoutCount = 0, 
            VkPushConstantRange* pushConstants = nullptr, 
            u32 pushConstantCount = 0
        );

        static VkDescriptorPool descriptorPool;

        AttributeStorage attributeStorage;

        VulkanDevice& device;

        Buffer::Buffer buffer;

        VkDescriptorSetLayout layout {VK_NULL_HANDLE}; 
        VkDescriptorSet descriptorSet {VK_NULL_HANDLE};

        Pipeline pipeline;
        VkPipelineLayout pipelineLayout {VK_NULL_HANDLE};
    };
}