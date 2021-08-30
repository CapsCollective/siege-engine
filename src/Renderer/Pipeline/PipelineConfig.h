#pragma once

#include "../Core.h"

namespace SnekVk
{
    // TEMPORARY
    class VertexDescription
    {
        public:
        
        template<size_t X, size_t Y>
        struct VertexDescriptionData
        {
            std::array<VkVertexInputBindingDescription, X> bindings;
            std::array<VkVertexInputAttributeDescription, Y> attributes; 
        };

        static VkVertexInputBindingDescription CreateBinding(
            u32 binding,
            u32 stride, 
            VkVertexInputRate inputRate
        );

        static VkVertexInputAttributeDescription CreateAttribute(
            u32 location, 
            u32 binding, 
            VkFormat format, 
            u32 offset
        );
    };   

    class PipelineConfig
    {
        public:
        enum PipelineStage
        {
            VERTEX = VK_SHADER_STAGE_VERTEX_BIT,
            FRAGMENT = VK_SHADER_STAGE_FRAGMENT_BIT
        };

        /**
         * @param pNext a pointer to a struct that's extending this struct. Defaults to nullptr.
         * @param flags any specific flags that need to be set. Defaults to 0.
         * @param topology specifies how vertices are to be interpreted. Defaults to a triangle list.
         * @param restartEnabled Allows a special vertex value to restart the assembly. Cannot be true with list topologies. Defaults to false.
         * @returns a VkPipelineInputAssemblyStateCreateInfo struct.
         **/
        static VkPipelineInputAssemblyStateCreateInfo InitInputAssemblyStateCreateInfo(
            const void* pNext = nullptr, 
            VkPipelineInputAssemblyStateCreateFlags flags = 0, 
            VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            VkBool32 restartEnabled = VK_FALSE
        );

        /**
         * @param viewportCount the number of viewports used by the shader
         * @param pViewPorts a pointer to an array of viewports
         * @param scissorCount the number of scissors (must match the number of viewports)
         * @param pScissors a pointer to an array of rect2Ds (specifying the scissors)
         * @param flags configuration flags, defaults to 0.
         * @param pNext a pointer to a struct which is extending this struct. 
         * @returns a VkPipelineViewportStateCreateInfo struct with the inputted fields.
         **/
        static VkPipelineViewportStateCreateInfo InitViewPortCreateInfo(
            u32 viewportCount,
            const VkViewport* pViewports,
            u32 scissorCount,
            const VkRect2D* pScissors,
            VkPipelineViewportStateCreateFlags flags = 0,
            const void* pNext = nullptr
        );


        /**
         * @param depthClampEnable controls whether the fragment's depth values should be clamped.
         * @param rasterizerDiscardEnable controls if primitives are discarded immediately before rasterisation stage.
         * @param polygonMode controls how the polygon is rendered.
         * @param cullMode the tringle facing direction for culling.
         * @param frontFace specifies the front-face triangle orinetation for culling.
         * @param depthBiasEnable enables biasing fragment depth values by a number.
         * @param depthBiasConstantFactor specifies a constant depth for all fragments. Defaults to 0.0f.
         * @param depthBiasClamp the maximum depth bias value for a fragment. Defaults. 0.0f.
         * @param depthBiasSlopeFactor scalar factor applied for a fragment's depth bias. Defaults to 0.0f.
         * @param lineWidth the width of line segments. Defaults to 1.0f.
         * @param flags configuration flags. Defaults to 0.
         * @param pNext a pointer to a struct that extends this struct. Defaults to nullptr.
         * @returns a VkPipelineRasterizationStateCreateInfo struct with all inputted fields.
         **/
        static VkPipelineRasterizationStateCreateInfo InitRasterizationCreateInfo(
            VkBool32 depthClampEnable,
            VkBool32 rasterizerDiscardEnable,
            VkPolygonMode polygonMode,
            VkCullModeFlags cullMode,
            VkFrontFace frontFace, 
            VkBool32 depthBiasEnable,
            float depthBiasConstantFactor = 0.0f,
            float depthBiasClamp = 0.0f,
            float depthBiasSlopeFactor = 0.0f,
            float lineWidth = 1.0f,
            VkPipelineRasterizationStateCreateFlags flags = 0,
            const void* pNext = nullptr
        );

        /**
         * Creates a multi-sampling state. This allows us to specify how multi-sampling would
         * work for our pipelines. For more info, please check: https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#fragops-covg
         * @param rasterisationSamples a bitmask specifying the number of samples to use.
         * @param sampleShadingEnable a bool specifying if sample shading is enabled.
         * @param minSampleShading if sample shading is enabled, specify the minimum fraction fraction to use.
         * @param pSampleMask a pointer to an array of sample masks to be used for multi-sampling.
         * @param alphaToCoverageEnable specifies if a temporary coverage value is generated for the alpha component. 
         * @param alphaToOneEnable specifies if an alpha component is replaced in accordance to multi-sampling rules. 
         * @returns a VkPipelineMultisampleStateCreateInfo struct. 
         **/
        static VkPipelineMultisampleStateCreateInfo InitMultiSampleCreateInfo(
            VkSampleCountFlagBits rasterisationSamples,
            VkBool32 sampleShadingEnable, 
            float minSampleShading, 
            const VkSampleMask* pSampleMask, 
            VkBool32 alphaToCoverageEnable,
            VkBool32 alphaToOneEnable,
            VkPipelineMultisampleStateCreateFlags flags = 0,
            const void* pNext = nullptr
        );

        /**
         * A config struct which specifies how colors are blended by the pipeline.
         * @param blendEnable specifies if color blending is enabled. 
         * @param srcColorBlendFactor specifies the blend factor used for the source.
         * @param dstColorBlendFactor specifies the blend factor used for the destination.
         * @param colorBlendOp selects which blend operation to use to calculate RGB values.
         * @param srcAlphaBlendFactor specifies the blend factor to be used for alpha values on the source. 
         * @param dstAlphaBlendFactor specifies the blend factor to be used for alpha values on the destination.
         * @param alphaBlendOp selects which operation to use to calculate alpha values. 
         * @param colorWriteMask a bitmask which specifies which color channels to expect. Defaults to RGBA.
         * @returns a configured VkPipelineColorBlendAttachmentState struct
         **/
        static VkPipelineColorBlendAttachmentState InitColorBlendAttachment(
            VkBool32 blendEnable,
            VkBlendFactor srcColorBlendFactor,
            VkBlendFactor dstColorBlendFactor,
            VkBlendOp colorBlendOp,
            VkBlendFactor srcAlphaBlendFactor,
            VkBlendFactor dstAlphaBlendFactor,
            VkBlendOp alphaBlendOp,
            VkColorComponentFlags colorWriteMask = 
                VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT 
                | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
        );

        /**
         * Creates an info struct for configuring the pipeline's color blend state.
         * @param logicOpEnable controls whether logical operations are applied to colors. 
         * @param logicOp selects which logical opeartion to apply
         * @param attachmentCount specifies the number of color blend attachments applied
         * @param pAttachments a pointer to an array of color blend attachments
         * @param blendConstantR the R value to be used for color blending. Defaults to 0.0f.
         * @param blendConstantG the G value to be used for color blending. Defaults to 0.0f.
         * @param blendConstantB the B value to be used for color blending. Defaults to 0.0f.
         * @param blendConstantA the A value to be used for color blending. Defaults to 0.0f.
         * @param flags configuration flags. Defaults to 0
         * @param pNext and structs which extend this struct. Defaults to nullptr.
         * @returns a VkPipelineColorBlendStateCreateInfo struct. 
         **/
        static VkPipelineColorBlendStateCreateInfo InitColorBlendCreateInfo(
            VkBool32 logicOpEnable,
            VkLogicOp logicOp, 
            u32 attachmentCount,
            const VkPipelineColorBlendAttachmentState* pAttachments,
            float blendConstantR = 0.0f,
            float blendConstantG = 0.0f,
            float blendConstantB = 0.0f,
            float blendConstantA = 0.0f,
            VkPipelineColorBlendStateCreateFlags flags = 0,
            const void* pNext = nullptr
        );

        static VkPipelineDepthStencilStateCreateInfo InitDepthStencilCreateInfo(
            VkBool32 depthTestEnable,
            VkBool32 depthWriteEnable, 
            VkCompareOp depthCompareOp,
            VkBool32 depthBoundsTestEnable,
            VkBool32 stencilTestEnable,
            VkStencilOpState front = {},
            VkStencilOpState back = {}, 
            float minDepthBounds = 0.0f,
            float maxDepthBounds = 1.0f,
            VkPipelineDepthStencilStateCreateFlags flags = 0, 
            const void* pNext = nullptr
        );

        static VkPipelineDynamicStateCreateInfo InitDynamicStateCreateInfo(
            u32 dynamicStateCount,
            const VkDynamicState* pDynamicStates,
            VkPipelineDynamicStateCreateFlags flags = 0,
            const void* pNext = nullptr
        );

        static VkPushConstantRange CreatePushConstantRange(VkShaderStageFlags stageflags, u32 offset, u32 size);

        static VkPipelineLayoutCreateInfo CreatePipelineLayoutCreateInfo(
            VkDescriptorSetLayout* layouts, 
            u32 layoutCount, 
            VkPushConstantRange* pushConstants, 
            u32 pushConstantCount
        );

        static void CreatePipelineLayout(
            VkDevice device,
            VkPipelineLayout* pipelineLayout,
            VkDescriptorSetLayout* layouts, 
            u32 layoutCount, 
            VkPushConstantRange* pushConstants, 
            u32 pushConstantCount
        );

        static VkPipelineShaderStageCreateInfo CreateShaderStage(
            VkShaderStageFlagBits stage, 
            VkShaderModule module, 
            const char* pName = "main",
            VkPipelineShaderStageCreateFlags flags = 0,
            const void* pNext = nullptr,
            const VkSpecializationInfo* pSpecialisationInfo = nullptr
        );

        static void CreateDefaultPipelineStages(
            VkPipelineShaderStageCreateInfo* pShaderStageCreateInfos, 
            PipelineStage* stages, 
            VkShaderModule* modules,
            u32 stageCount
        );

        private:
    };
}