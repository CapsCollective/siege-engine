//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Pipeline.h"

#include "utils/TypeAdaptor.h"

#include <utils/Logging.h>

namespace Siege::Vulkan
{

static constexpr VkPipelineColorBlendAttachmentState defaultColourBlendState =
{
    VK_TRUE,
    VK_BLEND_FACTOR_SRC_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    VK_BLEND_OP_ADD,
    VK_BLEND_FACTOR_SRC_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    VK_BLEND_OP_ADD,
    VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
    VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
};

static constexpr VkPipelineMultisampleStateCreateInfo defaultMultiSampleState =
{
    VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
    nullptr,
    0,
    VK_SAMPLE_COUNT_1_BIT,
    VK_FALSE,
    1.0f,
    nullptr,
    VK_FALSE,
    VK_FALSE
};

static constexpr VkPipelineRasterizationStateCreateInfo defaultRasterizationState
{
    VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        nullptr,
        0,
        VK_FALSE,
        VK_FALSE,
        static_cast<VkPolygonMode>(Pipeline::Builder::PipelineFillMode::FILL_MODE_FILL),
        VK_CULL_MODE_NONE,
        VK_FRONT_FACE_CLOCKWISE,
        VK_FALSE,
        0.f,
        0.f,
        0.f,
        1.f
};

Pipeline::Builder& Pipeline::Builder::WithRenderPass(RenderPass* targetRenderPass)
{
    renderPass = targetRenderPass;
    return *this;
}

Pipeline::Builder& Pipeline::Builder::WithDynamicViewport()
{
    viewportCount += 1;
    dynamicStates.Append(DynamicState::DYNAMIC_STATE_VIEWPORT);
    return *this;
}

Pipeline::Builder& Pipeline::Builder::WithDynamicScissor()
{
    scissorCount += 1;
    dynamicStates.Append(DynamicState::DYNAMIC_STATE_SCISSOR);
    return *this;
}

Pipeline::Builder& Pipeline::Builder::WithVertexShader(const Shader* vertShader)
{
    vertexShader = vertShader;
    return *this;
}

Pipeline::Builder& Pipeline::Builder::WithFragmentShader(const Shader* fragShader)
{
    fragmentShader = fragShader;
    return *this;
}

Pipeline::Builder& Pipeline::Builder::WithProperties(const ::Siege::Utils::MSArray<VkDescriptorSetLayout, 10>& layouts)
{
    descriptorLayouts = layouts;
    return *this;
}

Pipeline Pipeline::Builder::Build()
{
    Pipeline newPipeline;

    auto device = Context::GetVkLogicalDevice();

    VkPipelineLayoutCreateInfo layoutCreateInfo =
    {
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        nullptr,
        0,
        static_cast<uint32_t>(descriptorLayouts.Count()),
        descriptorLayouts.Data(),
        0,
        nullptr
    };

    CC_ASSERT(
        vkCreatePipelineLayout(device, &layoutCreateInfo, nullptr, &newPipeline.layout) == VK_SUCCESS,
        "Failed to create pipeline layout!")

    VkPipelineInputAssemblyStateCreateInfo inputAssembly
    {
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        nullptr,
        0,
        Utils::ToVkPrimitiveTopology(topology),
        0
    };

    VkPipelineViewportStateCreateInfo viewport
    {
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        nullptr,
        0,
        viewportCount,
        nullptr,    // TODO: Is there ever a situation where we do need to add in a viewport?
        scissorCount,
        nullptr
    };

    VkPipelineColorBlendStateCreateInfo defaultColourBlendStateCreate
    {
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        nullptr,
        0,
        VK_FALSE,
        VK_LOGIC_OP_COPY,
        1,
        &defaultColourBlendState,
        { 0.f, 0.f, 0.f, 0.f}
    };

    VkPipelineDepthStencilStateCreateInfo defaultStencilCreateState
    {
        VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        nullptr,
        0,
        VK_TRUE,
        VK_TRUE,
        VK_COMPARE_OP_LESS,
        VK_FALSE,
        VK_FALSE,
        {},
        {},
        0.f,
        1.f
    };

    auto* targetDynamicStates = (VkDynamicState*)dynamicStates.Data();

    VkPipelineDynamicStateCreateInfo dynamicStateCreateInfos
    {
            VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            nullptr,
            0,
            viewportCount + scissorCount,
            targetDynamicStates
    };

    // Get shader number and their stages.

    VkPipelineShaderStageCreateInfo shaderStages[2];

    shaderStages[0] = {
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        nullptr,
        0,
        Utils::ToVkShaderStageFlagBits(vertexShader->GetShaderType()),
        vertexShader->GetShaderModule(),
        "main",
        nullptr
    };

    shaderStages[1] = {
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        nullptr,
        0,
        Utils::ToVkShaderStageFlagBits(fragmentShader->GetShaderType()),
        fragmentShader->GetShaderModule(),
        "main",
        nullptr
    };

    // Process vertices

    auto& vertShaderVertices = vertexShader->GetVertexBindings();

    VkVertexInputBindingDescription inputDescriptions[vertShaderVertices.Count()];
    VkVertexInputAttributeDescription attributeDescriptions[vertexShader->GetTotalAttributeCount()];

    uint32_t processedAttributes {0};
    for (uint32_t i = 0; i < vertShaderVertices.Count(); i++)
    {
        auto& vertex = vertShaderVertices[i];
        inputDescriptions[i] =
        {
            i,
            vertex.stride,
            VK_VERTEX_INPUT_RATE_VERTEX
        };

        for(uint32_t j = 0; j < vertex.attributes.Count(); j++)
        {
            auto& attribute = vertex.attributes[j];

            size_t attributeIndex = j + processedAttributes;

            attributeDescriptions[attributeIndex] =
            {
                j, i, Utils::ToVkFormat(attribute.type), attribute.offset
            };
        }

        processedAttributes += vertex.attributes.Count();
    }

    VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo {};
    vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputCreateInfo.vertexAttributeDescriptionCount =
        static_cast<uint32_t>(vertexShader->GetTotalAttributeCount());
    vertexInputCreateInfo.vertexBindingDescriptionCount =
        static_cast<uint32_t>(vertShaderVertices.Count());
    vertexInputCreateInfo.pVertexAttributeDescriptions = attributeDescriptions;
    vertexInputCreateInfo.pVertexBindingDescriptions = inputDescriptions;

    VkGraphicsPipelineCreateInfo pipelineCreateInfo {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount = 2;
    pipelineCreateInfo.pStages = shaderStages;
    pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
    pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
    pipelineCreateInfo.pViewportState = &viewport;
    pipelineCreateInfo.pRasterizationState = &defaultRasterizationState;
    pipelineCreateInfo.pMultisampleState = &defaultMultiSampleState;
    pipelineCreateInfo.pColorBlendState = &defaultColourBlendStateCreate;
    pipelineCreateInfo.pDynamicState = &dynamicStateCreateInfos;
    pipelineCreateInfo.pDepthStencilState = &defaultStencilCreateState;

    pipelineCreateInfo.layout = newPipeline.layout;
    pipelineCreateInfo.renderPass = renderPass->GetRenderPass();
    pipelineCreateInfo.subpass = 0;

    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineCreateInfo.basePipelineIndex = -1;

    CC_ASSERT(vkCreateGraphicsPipelines(Vulkan::Context::GetVkLogicalDevice(),
                                        VK_NULL_HANDLE,
                                        1,
                                        &pipelineCreateInfo,
                                        nullptr,
                                        OUT &newPipeline.pipeline) == VK_SUCCESS,
              "Failed to create graphics pipeline!")

    return newPipeline;
}

Pipeline::~Pipeline()
{
    vkDestroyPipelineLayout(Vulkan::Context::GetVkLogicalDevice(), layout, nullptr);
    vkDestroyPipeline(Vulkan::Context::GetVkLogicalDevice(), pipeline, nullptr);
}

void Pipeline::Bind(const CommandBuffer& commandBuffer)
{
    vkCmdBindPipeline(commandBuffer.Get(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}

void Pipeline::BindSets(const CommandBuffer& commandBuffer, ::Siege::Utils::MSArray<VkDescriptorSet, 10> sets)
{
    vkCmdBindDescriptorSets(commandBuffer.Get(),
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            layout,
                            0,
                            sets.Count(),
                            sets.Data(),
                            0,
                            nullptr);
}

// TODO: Remove the next two functions once we incorporate the CommandBuffer class into renderer
void Pipeline::Bind(VkCommandBuffer commandBuffer)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}
void Pipeline::BindSets(VkCommandBuffer commandBuffer, ::Siege::Utils::MSArray<VkDescriptorSet, 10> sets)
{
    vkCmdBindDescriptorSets(commandBuffer,
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            layout,
                            0,
                            sets.Count(),
                            sets.Data(),
                            0,
                            nullptr);
}

void Pipeline::Swap(Pipeline& other)
{
    auto tmpPipelineLayout = layout;
    auto tmpPipeline = pipeline;

    layout = other.layout;
    pipeline = other.pipeline;

    other.pipeline = tmpPipeline;
    other.layout = tmpPipelineLayout;
}
}
