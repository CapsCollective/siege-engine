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

        Material();

        Material(const Material&) = delete;
        Material& operator=(const Material&) = delete;

        ~Material();

        void SetDescriptor(Descriptor descriptor);
        void BuildMaterial();

        private:

        void CreateDescriptors();
        void CreateLayout(
            VkDescriptorSetLayout* layouts = nullptr, 
            u32 layoutCount = 0, 
            VkPushConstantRange* pushConstants = nullptr, 
            u32 pushConstantCount = 0
        );

        static VkDescriptorPool descriptorPool;

        VulkanDevice& device;

        Buffer::Buffer buffer;

        VkDescriptorSetLayout layout {VK_NULL_HANDLE}; 
        VkDescriptorSet descriptorSet {VK_NULL_HANDLE};

        Pipeline pipeline;
        VkPipelineLayout pipelineLayout {VK_NULL_HANDLE};
    };
}