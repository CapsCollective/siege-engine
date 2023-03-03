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
    const ::Siege::Utils::MSArray<VkDescriptorSetLayout, 10>& layouts)
{
    descriptorLayouts = layouts;
    return *this;
}

Pipeline Pipeline::Builder::Build()
{
    Pipeline newPipeline;

    auto device = Context::GetVkLogicalDevice();

    using namespace Utils::Pipeline;

    newPipeline.layout = CreatePipelineLayout(device, descriptorLayouts);

    auto inputAssembly =
        CreateInputAssembly(ToVkPrimitiveTopology(vertexShader->GetVertexTopology()));

    auto viewportInfo = CreateViewportState(viewportCount, scissorCount);

    auto dynamicStateCreateInfos =
        CreateDynamicStates(viewportCount + scissorCount, (VkDynamicState*) dynamicStates.Data());

    // Get shader number and their stages.

    ::Siege::Utils::MSArray<VkPipelineShaderStageCreateInfo, 5> shaderStages {
        CreateVertexShaderStage(vertexShader->GetShaderModule()),
        CreateFragmentShaderStage(fragmentShader->GetShaderModule()),
    };

    // Process vertices

    auto& vertShaderVertices = vertexShader->GetVertexBindings();

    VkVertexInputBindingDescription inputDescriptions[vertShaderVertices.Count()];
    VkVertexInputAttributeDescription attributeDescriptions[vertexShader->GetTotalAttributeCount()];

    uint32_t processedAttributes = 0;
    vertShaderVertices.MForEachI([&](const Shader::VertexBinding& binding, size_t i) {
        inputDescriptions[i] = {static_cast<uint32_t>(i),
                                binding.stride,
                                VK_VERTEX_INPUT_RATE_VERTEX};

        auto& attributes = binding.attributes;

        attributes.MForEachI([&](const Shader::VertexAttribute& vertex, size_t j) {
            size_t attributeIndex = j + processedAttributes;

            attributeDescriptions[attributeIndex] = {static_cast<uint32_t>(j),
                                                     static_cast<uint32_t>(i),
                                                     Utils::ToVkFormat(vertex.type),
                                                     vertex.offset};
        });

        processedAttributes += attributes.Count();
    });

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

void Pipeline::BindSets(const CommandBuffer& commandBuffer,
                        ::Siege::Utils::MSArray<VkDescriptorSet, 2> sets)
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
