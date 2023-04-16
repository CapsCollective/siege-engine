//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Pipeline.h"

#include <utils/Logging.h>

#include "utils/Pipeline.h"
#include "utils/TypeAdaptor.h"

namespace Siege::Vulkan
{
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

Pipeline::Builder& Pipeline::Builder::WithProperties(
    const MSArray<VkDescriptorSetLayout, 10>& layouts)
{
    descriptorLayouts = layouts;
    return *this;
}

Pipeline::Builder& Pipeline::Builder::WithPushConstant(uint32_t size, Utils::ShaderType type)
{
    pushConstant = {size, type};
    return *this;
}

Pipeline Pipeline::Builder::Build()
{
    Pipeline newPipeline;

    auto device = Context::GetVkLogicalDevice();

    using namespace Utils::Pipeline;

    // We're assuming for now that we'll only ever have one push constant at any point in time
    VkPushConstantRange range {Utils::ToVkShaderStageFlagBits(pushConstant.type),
                               0,
                               pushConstant.size};

    newPipeline.layout =
        CreatePipelineLayout(device, descriptorLayouts, &range, pushConstant.size > 0);

    auto inputAssembly =
        CreateInputAssembly(ToVkPrimitiveTopology(vertexShader->GetVertexTopology()));

    auto viewportInfo = CreateViewportState(viewportCount, scissorCount);

    auto dynamicStateCreateInfos =
        CreateDynamicStates(viewportCount + scissorCount, (VkDynamicState*) dynamicStates.Data());

    // Get shader number and their stages.

    MSArray<VkPipelineShaderStageCreateInfo, 5> shaderStages {
        CreateVertexShaderStage(vertexShader->GetShaderModule()),
        CreateFragmentShaderStage(fragmentShader->GetShaderModule()),
    };

    // Process vertices

    auto& vertShaderVertices = vertexShader->GetVertexBindings();

    VkVertexInputBindingDescription inputDescriptions[vertShaderVertices.Count()];
    VkVertexInputAttributeDescription attributeDescriptions[vertexShader->GetTotalAttributeCount()];

    uint32_t processedAttributes = 0;

    uint32_t currentLocation = 0;
    for (auto vertIt = vertShaderVertices.CreateIterator(); vertIt; ++vertIt)
    {
        auto& binding = *vertIt;
        inputDescriptions[vertIt.GetIndex()] = {static_cast<uint32_t>(vertIt.GetIndex()),
                                                binding.stride,
                                                Utils::ToVkVertexInputRate(binding.inputRate)};

        auto& attributes = binding.attributes;

        for (auto attrIt = attributes.CreateIterator(); attrIt; ++attrIt)
        {
            auto& vertex = *attrIt;
            auto attributeIndex = attrIt.GetIndex() + processedAttributes;

            attributeDescriptions[attributeIndex] = {currentLocation,
                                                     static_cast<uint32_t>(vertIt.GetIndex()),
                                                     Utils::ToVkFormat(vertex.type),
                                                     vertex.offset};
            currentLocation++;
        }

        processedAttributes += attributes.Count();
    }

    auto vertexInputCreateInfo =
        CreateVertexInputInfo(static_cast<uint32_t>(vertexShader->GetTotalAttributeCount()),
                              static_cast<uint32_t>(vertShaderVertices.Count()),
                              attributeDescriptions,
                              inputDescriptions);

    VkGraphicsPipelineCreateInfo pipelineCreateInfo {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount = shaderStages.Count();
    pipelineCreateInfo.pStages = shaderStages.Data();
    pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
    pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
    pipelineCreateInfo.pViewportState = &viewportInfo;
    pipelineCreateInfo.pRasterizationState = &Utils::Pipeline::defaultRasterizationState;
    pipelineCreateInfo.pMultisampleState = &Utils::Pipeline::defaultMultiSampleState;
    pipelineCreateInfo.pColorBlendState = &Utils::Pipeline::defaultColourBlendStateCreate;
    pipelineCreateInfo.pDynamicState = &dynamicStateCreateInfos;
    pipelineCreateInfo.pDepthStencilState = &Utils::Pipeline::defaultStencilCreateState;

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
                                        OUT & newPipeline.pipeline) == VK_SUCCESS,
              "Failed to create graphics pipeline!")

    return newPipeline;
}

Pipeline::~Pipeline()
{
    Destroy();
}

void Pipeline::Destroy()
{
    auto device = Vulkan::Context::GetVkLogicalDevice();

    // This check is only necessary because we're using globals.
    // TODO: Remove all globals so that we can avoid this kind of thing in future
    if (device == nullptr) return;

    vkDestroyPipelineLayout(device, layout, nullptr);
    vkDestroyPipeline(device, pipeline, nullptr);
}

void Pipeline::Bind(const CommandBuffer& commandBuffer)
{
    vkCmdBindPipeline(commandBuffer.Get(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}

void Pipeline::BindSets(const CommandBuffer& commandBuffer, MSArray<VkDescriptorSet, 2> sets)
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

void Pipeline::PushConstants(const CommandBuffer& commandBuffer,
                             Utils::ShaderType type,
                             uint32_t size,
                             const void* values)
{
    vkCmdPushConstants(commandBuffer.Get(),
                       layout,
                       Utils::ToVkShaderStageFlagBits(type),
                       0,
                       size,
                       values);
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
} // namespace Siege::Vulkan
