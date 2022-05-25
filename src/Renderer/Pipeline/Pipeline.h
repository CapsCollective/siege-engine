#pragma once

#include "../Core.h"
#include "../Device/VulkanDevice.h"

namespace SnekVk 
{
    /**
     * A configuration struct containing all relevant pipeline config data. 
     * The intention is for the struct to be pre-configured to match the user's
     * intent and then be passed into the graphics pipeline when being constructed. 
     * 
     * Configuration includes:
     * 1) viewport configuration
     * 2) viewport scissor configurations
     * 3) input assembly - how vertices are submitted to the shader
     * 4) resterization - how vertices are represented 
     * 5) multisampling - how we handle multi-sampling to avoid aliasing.
     * 6) color blending - how colors are blended together in the shader. 
     * 7) depth stencil - how depth is represented within the shader.
     * 8) A pipeline layout representing how data is laid out in the shader. 
     * 9) Our render pass.
     * 9) Any subpasses used by the pipeline. 
     **/
    struct PipelineConfigInfo 
    {
        VkPipelineViewportStateCreateInfo viewportInfo{};
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
        VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
        VkPipelineMultisampleStateCreateInfo multisampleInfo{};
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};

        Utils::Array<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;

        VkPipelineLayout pipelineLayout{nullptr};
        VkRenderPass renderPass{nullptr};
        u32 subPass{0}; 
    };

    class Pipeline 
    {
        public:
            // 'Structors

            Pipeline(
                VulkanDevice& device, 
                const char* vertFilePath, 
                const char* fragFilePath, 
                const PipelineConfigInfo& configInfo
            );

            ~Pipeline();

            // Delete copy constructors
            Pipeline(const Pipeline&) = delete;
            Pipeline& operator=(const Pipeline&) = delete;

            /**
             * Creates a default pipelineConfig struct. This struct currently configures
             * a pipeline to draw a 2D shape with custom vertices (using a vertex buffer).
             * @param width the image width (generally accessible from the swapchain)
             * @param height the image height (generally accessible from the swapchain)
             * @returns a PipleineConfigInfo struct with a default configuration.
             **/
            static PipelineConfigInfo DefaultPipelineConfig();

            /**
             * Binds a pipeline to a command buffer. This tells out command buffers that 
             * all bound buffers after this should be submitted to the same pipeline. This 
             * in turn means that all data passed in is then used to draw objects related to
             * said pipeline. 
             * @param commandBuffer the command buffer being bound to.
             **/
            void Bind(VkCommandBuffer commandBuffer);

            // TODO: document this
            void RecreatePipeline(
                const char* vertFilePath, 
                const char* fragFilePath, 
                const PipelineConfigInfo& configInfo
            );

            // TODO: document this
            void ClearPipeline();

        private:

            /**
             * Reads a file and returns the contents in binary. This is particularly
             * useful for binary shader file formats (like spir-v).
             * @param filePath a raw c string specifying the file path. 
             * @returns a heap-allocated array containing the file contents.
             **/
            static Utils::Array<char> ReadFile(const char* filePath);

            /**
             * Creates the vulkan graphics pipeline and assigns it to the 'graphicsPipeline' 
             * member variable. 
             * @param vertFilePath the path to vertex shader.
             * @param fragFilePath the path to the fragment shader.
             * @param configInfo the configuration struct for the pipeline. 
             **/
            void CreateGraphicsPipeline(
                char const* vertFilePath, 
                char const* fragFilePath, 
                const PipelineConfigInfo& configInfo
            );

            /**
             * Converts the binary shader into a vulkan shader module.  
             * A shader module is an interface for our pipelines to actually
             * receive shader data. 
             * @param fileData an array containing our binary shader data.
             * @param shaderModule a pointer to a shaderModule that we want to populate. 
             **/
            void CreateShaderModule(Utils::Array<char>& fileData, VkShaderModule* shaderModule);

            /**
             * A reference to our device. A reference is safe here since we 
             * can guarantee that our device will outlive the pipeline (since
             * the device is destroyed last) 
             **/
            VulkanDevice& device;

            /**
             * The vulkan representation of a graphics pipeline. 
             **/
            VkPipeline graphicsPipeline;

            // Shader modules for our vertex and index shaders.
            VkShaderModule vertShader;
            VkShaderModule fragShader; 
    };
}