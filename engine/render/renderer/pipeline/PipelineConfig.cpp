//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "PipelineConfig.h"

#include <utils/Logging.h>

namespace Siege
{
VkVertexInputBindingDescription VertexDescription::CreateBinding(uint32_t binding,
                                                                 uint32_t stride,
                                                                 VkVertexInputRate inputRate)
{
    return {binding, stride, inputRate};
}

VkVertexInputAttributeDescription VertexDescription::CreateAttribute(uint32_t location,
                                                                     uint32_t binding,
                                                                     VkFormat format,
                                                                     uint32_t offset)
{
    return {location, binding, format, offset};
}

VertexDescription::Data VertexDescription::CreateDescriptions(size_t bindingCount,
                                                              Binding* bindings)
{
    Data vertexData;

    size_t totalAttributes = 0;
    for (size_t i = 0; i < bindingCount; i++)
    {
        totalAttributes += bindings[i].attributeCount;
    }

    vertexData.bindings = Utils::MHArray<VkVertexInputBindingDescription>(bindingCount);
    vertexData.attributes = Utils::MHArray<VkVertexInputAttributeDescription>(totalAttributes);

    size_t processedAttributes = 0;
    for (size_t i = 0; i < bindingCount; i++)
    {
        auto& binding = bindings[i];
        vertexData.bindings[i] =
            CreateBinding(i, binding.stride, (VkVertexInputRate) binding.inputRate);

        for (uint32_t j = 0; j < binding.attributeCount; j++)
        {
            auto attribute = binding.attributes[j];
            size_t attributeIndex = j + processedAttributes;
            vertexData.attributes[attributeIndex] =
                CreateAttribute(j, i, (VkFormat) attribute.type, attribute.offset);
        }

        processedAttributes += binding.attributeCount;
    }

    return vertexData;
}

VertexDescription::Binding VertexDescription::CreateBinding(uint32_t binding,
                                                            uint32_t stride,
                                                            InputRate inputRate,
                                                            Attribute* attributes,
                                                            uint32_t attibuteCount)
{
    return {binding, stride, inputRate, attibuteCount, attributes};
}

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
    uint32_t viewportCount,
    const VkViewport* pViewports,
    uint32_t scissorCount,
    const VkRect2D* pScissors,
    VkPipelineViewportStateCreateFlags flags,
    const void* pNext)
{
    auto sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
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
    return {sType,
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
            lineWidth};
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
    return {sType,
            pNext,
            flags,
            rasterisationSamples,
            sampleShadingEnable,
            minSampleShading,
            pSampleMask,
            alphaToCoverageEnable,
            alphaToOneEnable};
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
    return {blendEnable,
            srcColorBlendFactor,
            dstColorBlendFactor,
            colorBlendOp,
            srcAlphaBlendFactor,
            dstAlphaBlendFactor,
            alphaBlendOp,
            colorWriteMask};
}

VkPipelineColorBlendStateCreateInfo PipelineConfig::InitColorBlendCreateInfo(
    VkBool32 logicOpEnable,
    VkLogicOp logicOp,
    uint32_t attachmentCount,
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
    return {sType,
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
            maxDepthBounds};
}

VkPipelineDynamicStateCreateInfo PipelineConfig::InitDynamicStateCreateInfo(
    uint32_t dynamicStateCount,
    const VkDynamicState* pDynamicStates,
    VkPipelineDynamicStateCreateFlags flags,
    const void* pNext)
{
    auto sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    return {sType, pNext, flags, dynamicStateCount, pDynamicStates};
}

VkPushConstantRange PipelineConfig::CreatePushConstantRange(VkShaderStageFlags stageflags,
                                                            uint32_t offset,
                                                            uint32_t size)
{
    return {stageflags, offset, size};
}

VkPipelineLayoutCreateInfo PipelineConfig::CreatePipelineLayoutCreateInfo(
    VkDescriptorSetLayout* layouts,
    uint32_t layoutCount,
    VkPushConstantRange* pushConstants,
    uint32_t pushConstantCount)
{
    auto sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    return {sType, nullptr, 0, layoutCount, layouts, pushConstantCount, pushConstants};
}

void PipelineConfig::CreatePipelineLayout(VkDevice device,
                                          VkPipelineLayout* pipelineLayout,
                                          VkDescriptorSetLayout* layouts,
                                          uint32_t layoutCount,
                                          VkPushConstantRange* pushConstants,
                                          uint32_t pushConstantCount)
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo =
        CreatePipelineLayoutCreateInfo(layouts, layoutCount, pushConstants, pushConstantCount);

    CC_ASSERT(
        vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, pipelineLayout) == VK_SUCCESS,
        "Failed to create pipeline layout!");
}

VkPipelineShaderStageCreateInfo PipelineConfig::CreateShaderStage(
    VkShaderStageFlagBits stage,
    VkShaderModule module,
    const char* pName,
    VkPipelineShaderStageCreateFlags flags,
    const void* pNext,
    const VkSpecializationInfo* pSpecialisationInfo)
{
    auto sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    return {sType, pNext, flags, stage, module, pName, pSpecialisationInfo};
}

void PipelineConfig::CreateDefaultPipelineStages(
    VkPipelineShaderStageCreateInfo* pShaderStageCreateInfos,
    PipelineStage* stages,
    VkShaderModule* modules,
    uint32_t stageCount)
{
    for (size_t i = 0; i < stageCount; i++)
    {
        pShaderStageCreateInfos[i] =
            CreateShaderStage((VkShaderStageFlagBits) stages[i], modules[i]);
    }
}
} // namespace Siege
