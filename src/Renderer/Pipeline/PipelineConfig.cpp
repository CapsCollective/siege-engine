#include "PipelineConfig.h"

namespace SnekVk
{
    VkPipelineInputAssemblyStateCreateInfo PipelineConfig::InitInputAssemblyStateCreateInfo(
            const void* pNext, 
            VkPipelineInputAssemblyStateCreateFlags flags, 
            VkPrimitiveTopology topology,
            VkBool32 restartEnabled)
    {
        auto sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;

        return {sType, pNext, flags, topology, restartEnabled};
    }

    VkPipelineViewportStateCreateInfo PipelineConfig::InitViewPortCreateInfo(
            u32 viewportCount,
            const VkViewport* pViewports,
            u32 scissorCount,
            const VkRect2D* pScissors,
            VkPipelineViewportStateCreateFlags flags,
            const void* pNext)
    {
        auto sType =  VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        return {sType, pNext, flags, viewportCount, pViewports, scissorCount, pScissors};
    }

    VkPipelineRasterizationStateCreateInfo PipelineConfig::InitRasterizationCreateInfo(
            VkBool32 depthClampEnable,
            VkBool32 rasterizerDiscardEnable,
            VkPolygonMode polygonMode,
            VkCullModeFlags cullMode,
            VkFrontFace frontFace, 
            VkBool32 depthBiasEnable,
            float depthBiasConstantFactor,
            float depthBiasClamp,
            float depthBiasSlopeFactor,
            float lineWidth,
            VkPipelineRasterizationStateCreateFlags flags,
            const void* pNext)
    {
        auto sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        return {
            sType, 
            pNext,
            flags, 
            depthClampEnable,
            rasterizerDiscardEnable,
            polygonMode,
            cullMode,
            frontFace,
            depthBiasEnable,
            depthBiasConstantFactor,
            depthBiasClamp,
            depthBiasSlopeFactor,
            lineWidth
        };
    }

    VkPipelineMultisampleStateCreateInfo PipelineConfig::InitMultiSampleCreateInfo(
            VkSampleCountFlagBits rasterisationSamples,
            VkBool32 sampleShadingEnable, 
            float minSampleShading, 
            const VkSampleMask* pSampleMask, 
            VkBool32 alphaToCoverageEnable,
            VkBool32 alphaToOneEnable,
            VkPipelineMultisampleStateCreateFlags flags,
            const void* pNext)
    {
        auto sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        return {
            sType, 
            pNext, 
            flags, 
            rasterisationSamples, 
            sampleShadingEnable, 
            minSampleShading, 
            pSampleMask, 
            alphaToCoverageEnable, 
            alphaToOneEnable
        };
    }

    VkPipelineColorBlendAttachmentState PipelineConfig::InitColorBlendAttachment(
            VkBool32 blendEnable,
            VkBlendFactor srcColorBlendFactor,
            VkBlendFactor dstColorBlendFactor,
            VkBlendOp colorBlendOp,
            VkBlendFactor srcAlphaBlendFactor,
            VkBlendFactor dstAlphaBlendFactor,
            VkBlendOp alphaBlendOp,
            VkColorComponentFlags colorWriteMask)
    {
        return {
            blendEnable, 
            srcColorBlendFactor,
            dstColorBlendFactor,
            colorBlendOp,
            srcAlphaBlendFactor,
            dstAlphaBlendFactor,
            alphaBlendOp,
            colorWriteMask
        };
    }

    VkPipelineColorBlendStateCreateInfo PipelineConfig::InitColorBlendCreateInfo(
            VkBool32 logicOpEnable,
            VkLogicOp logicOp, 
            u32 attachmentCount,
            const VkPipelineColorBlendAttachmentState* pAttachments,
            float blendConstantR,
            float blendConstantG,
            float blendConstantB,
            float blendConstantA,
            VkPipelineColorBlendStateCreateFlags flags,
            const void* pNext)
    {
        auto sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        VkPipelineColorBlendStateCreateInfo createInfo = {
            sType, 
            pNext,
            flags, 
            logicOpEnable,
            logicOp,
            attachmentCount, 
            pAttachments,
        };

        createInfo.blendConstants[0] = blendConstantR;
        createInfo.blendConstants[1] = blendConstantG;
        createInfo.blendConstants[2] = blendConstantB;
        createInfo.blendConstants[3] = blendConstantA;

        return createInfo;
    }

    VkPipelineDepthStencilStateCreateInfo PipelineConfig::InitDepthStencilCreateInfo(
            VkBool32 depthTestEnable,
            VkBool32 depthWriteEnable, 
            VkCompareOp depthCompareOp,
            VkBool32 depthBoundsTestEnable,
            VkBool32 stencilTestEnable,
            VkStencilOpState front,
            VkStencilOpState back, 
            float minDepthBounds,
            float maxDepthBounds,
            VkPipelineDepthStencilStateCreateFlags flags, 
            const void* pNext)
    {
        auto sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        return 
        {
            sType, 
            pNext, 
            flags, 
            depthTestEnable, 
            depthWriteEnable, 
            depthCompareOp, 
            depthBoundsTestEnable, 
            stencilTestEnable, 
            front, 
            back, 
            minDepthBounds, 
            maxDepthBounds
        };
    }

    VkPipelineDynamicStateCreateInfo PipelineConfig::InitDynamicStateCreateInfo(
            u32 dynamicStateCount,
            const VkDynamicState* pDynamicStates,
            VkPipelineDynamicStateCreateFlags flags,
            const void* pNext)
    {
        auto sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        return 
        {
            sType, 
            pNext, 
            flags, 
            dynamicStateCount,
            pDynamicStates
        };
    }

    VkPushConstantRange PipelineConfig::CreatePushConstantRange(VkShaderStageFlags stageflags, u32 offset, u32 size)
    {
        return { stageflags, offset, size};
    }

    VkPipelineLayoutCreateInfo PipelineConfig::CreatePipelineLayoutCreateInfo(
            VkDescriptorSetLayout* layouts, 
            u32 layoutCount, 
            VkPushConstantRange* pushConstants, 
            u32 pushConstantCount)
    {
        auto sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

        return { sType, nullptr, 0, layoutCount, layouts, pushConstantCount, pushConstants};
    }

    void PipelineConfig::CreatePipelineLayout(
            VkDevice device,
            VkPipelineLayout* pipelineLayout,
            VkDescriptorSetLayout* layouts, 
            u32 layoutCount, 
            VkPushConstantRange* pushConstants, 
            u32 pushConstantCount
    )
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo = 
                CreatePipelineLayoutCreateInfo(layouts, layoutCount, pushConstants, pushConstantCount);

        SNEK_ASSERT(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, pipelineLayout) == VK_SUCCESS, 
            "Failed to create pipeline layout!");
    }
}