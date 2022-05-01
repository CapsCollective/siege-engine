#pragma once

#include "../Core.h"
#include "../Device/VulkanDevice.h"

namespace SnekVk 
{
    struct PipelineConfigInfo 
    {
        VkViewport viewport{};
        VkRect2D scissor{};
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
        VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
        VkPipelineMultisampleStateCreateInfo multisampleInfo{};
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
        VkPipelineLayout pipelineLayout{nullptr};
        VkRenderPass renderPass{nullptr};
        u32 subPass{0}; 
    };

    class Pipeline 
    {
        public:
            Pipeline(
                VulkanDevice& device, 
                const char* vertFilePath, 
                const char* fragFilePath, 
                const PipelineConfigInfo& configInfo
            );

            ~Pipeline();

            static void DestroyGraphicsPipeline(Pipeline& pipeline); 

            Pipeline(const Pipeline&) = delete;
            void operator=(const Pipeline&) = delete;

            static PipelineConfigInfo DefaultPipelineConfig(u32 width, u32 height);

            void Bind(VkCommandBuffer commandBuffer);
        private:

            static struct FileData ReadFile(const char* filePath);
            void CreateGraphicsPipeline(
                char const* vertFilePath, 
                char const* fragFilePath, 
                const PipelineConfigInfo& configInfo
            );

            void CreateShaderModule(struct FileData fileData, VkShaderModule* shaderModule);

            VulkanDevice& device;
            VkPipeline graphicsPipeline;

            VkShaderModule vertShader;
            VkShaderModule fragShader; 
    };

    struct FileData
    {
        char* buffer = nullptr;
        u32 bufferSize = 0;
    };

    void DestroyFileData(FileData& data);
}