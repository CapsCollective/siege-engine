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
}