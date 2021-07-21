#pragma once

#include "../Renderer.h"
#include "../Device/VulkanDevice.h"

namespace SnekVk 
{
    struct PipelineConfigInfo {};
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

            Pipeline(const Pipeline&) = delete;
            void operator=(const Pipeline&) = delete;

            static PipelineConfigInfo DefaultPipelineConfig(u32 width, u32 height);
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