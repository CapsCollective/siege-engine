//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Pipeline.h"

#include <utils/Logging.h>

namespace Siege::Vulkan::Utils::Pipeline
{
VkPipelineLayout CreatePipelineLayout(VkDevice device,
                                      MSArray<VkDescriptorSetLayout, 10> layouts,
                                      VkPushConstantRange* range,
                                      uint32_t pushConstantCount)
{
    auto createInfo = defaultPipelineLayoutCreateInfo;
    createInfo.setLayoutCount = layouts.Count();
    createInfo.pSetLayouts = layouts.Data();
    createInfo.pPushConstantRanges = range;
    createInfo.pushConstantRangeCount = pushConstantCount;

    VkPipelineLayout layout {VK_NULL_HANDLE};

    CC_ASSERT(vkCreatePipelineLayout(device, &createInfo, nullptr, &layout) == VK_SUCCESS,
              "Failed to create pipeline layout!")

    return layout;
}

VkPipelineInputAssemblyStateCreateInfo CreateInputAssembly(VkPrimitiveTopology topology,
                                                           VkBool32 restartEnabled)
{
    auto inputAssembly = defaultInputAssembly;
    inputAssembly.topology = topology;
    return inputAssembly;
}

VkPipelineViewportStateCreateInfo CreateViewportState(uint32_t viewportCount, uint32_t scissorCount)
{
    auto viewportState = defaultViewportInfo;
    viewportState.viewportCount = viewportCount;
    viewportState.scissorCount = scissorCount;
    return viewportState;
}

VkPipelineDynamicStateCreateInfo CreateDynamicStates(uint32_t dynamicStateCount,
                                                     VkDynamicState* states)
{
    auto dynamicStates = defaultDynamicStateCreateInfos;
    dynamicStates.dynamicStateCount = dynamicStateCount;
    dynamicStates.pDynamicStates = states;
    return dynamicStates;
}

VkPipelineShaderStageCreateInfo CreateVertexShaderStage(VkShaderModule shaderModule)
{
    auto vertexShaderStage = defaultVertexShaderStageCreateInfo;
    vertexShaderStage.module = shaderModule;
    return vertexShaderStage;
}

VkPipelineShaderStageCreateInfo CreateFragmentShaderStage(VkShaderModule shaderModule)
{
    auto fragShaderStage = defaultFragmentShaderStageCreateInfo;
    fragShaderStage.module = shaderModule;
    return fragShaderStage;
}

VkPipelineVertexInputStateCreateInfo CreateVertexInputInfo(
    uint32_t attributeCount,
    uint32_t bindingCount,
    const VkVertexInputAttributeDescription* attributes,
    const VkVertexInputBindingDescription* bindings)
{
    return {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            nullptr,
            0,
            bindingCount,
            bindings,
            attributeCount,
            attributes};
}

} // namespace Siege::Vulkan::Utils::Pipeline