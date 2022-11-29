//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_UTILS_PIPELINE_H
#define SIEGE_ENGINE_VULKAN_UTILS_PIPELINE_H

#include <utils/collections/StackArray.h>
#include <volk/volk.h>

namespace Siege::Vulkan::Utils::Pipeline
{
// Default instantiations for some important Vulkan pipeline objects

inline constexpr VkPipelineColorBlendAttachmentState defaultColourBlendState = {
    VK_TRUE,
    VK_BLEND_FACTOR_SRC_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    VK_BLEND_OP_ADD,
    VK_BLEND_FACTOR_SRC_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    VK_BLEND_OP_ADD,
    VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT};

inline constexpr VkPipelineMultisampleStateCreateInfo defaultMultiSampleState = {
    VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
    nullptr,
    0,
    VK_SAMPLE_COUNT_1_BIT,
    VK_FALSE,
    1.0f,
    nullptr,
    VK_FALSE,
    VK_FALSE};

inline constexpr VkPipelineRasterizationStateCreateInfo defaultRasterizationState {
    VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
    nullptr,
    0,
    VK_FALSE,
    VK_FALSE,
    VK_POLYGON_MODE_FILL,
    VK_CULL_MODE_NONE,
    VK_FRONT_FACE_CLOCKWISE,
    VK_FALSE,
    0.f,
    0.f,
    0.f,
    1.f};

inline constexpr VkPipelineLayoutCreateInfo defaultPipelineLayoutCreateInfo {
    VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    nullptr,
    0,
    0,
    nullptr,
    0,
    nullptr};

inline constexpr VkPipelineInputAssemblyStateCreateInfo defaultInputAssembly {
    VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
    nullptr,
    0,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    0};

inline constexpr VkPipelineViewportStateCreateInfo defaultViewportInfo {
    VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    nullptr,
    0,
    0,
    nullptr, // TODO: Is there ever a situation where we do need to add in a viewport?
    0,
    nullptr};

inline constexpr VkPipelineColorBlendStateCreateInfo defaultColourBlendStateCreate {
    VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    nullptr,
    0,
    VK_FALSE,
    VK_LOGIC_OP_COPY,
    1,
    &Utils::Pipeline::defaultColourBlendState,
    {0.f, 0.f, 0.f, 0.f}};

inline constexpr VkPipelineDepthStencilStateCreateInfo defaultStencilCreateState {
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
    1.f};

inline constexpr VkPipelineDynamicStateCreateInfo defaultDynamicStateCreateInfos {
    VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
    nullptr,
    0,
    0,
    nullptr};

inline constexpr VkPipelineShaderStageCreateInfo defaultVertexShaderStageCreateInfo {
    VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    nullptr,
    0,
    VK_SHADER_STAGE_VERTEX_BIT,
    nullptr,
    "main",
    nullptr};

inline constexpr VkPipelineShaderStageCreateInfo defaultFragmentShaderStageCreateInfo {
    VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    nullptr,
    0,
    VK_SHADER_STAGE_FRAGMENT_BIT,
    nullptr,
    "main",
    nullptr};

/**
 * Creates a Pipeline layout based off the expected descriptor set layouts
 * @param device the GPU the data will be allocated from
 * @param layouts an array of descriptor set layouts expected by the pipeline
 * @return the newly created PipelineLayout
 */
VkPipelineLayout CreatePipelineLayout(VkDevice device,
                                      ::Siege::Utils::MSArray<VkDescriptorSetLayout, 10> layouts);
/**
 * Creates a configured InputAssemblyInfo struct. This struct specifies how we expect the vertices
 * received by the shader to be formatted and linked
 * @param topology the expected topology for the vertices
 * @param restartEnabled specifies if we want a certain value to act as a restart on vertex links
 * @return a newly created VkPipelineInputAssemblyStateCreateInfo struct
 */
VkPipelineInputAssemblyStateCreateInfo CreateInputAssembly(VkPrimitiveTopology topology,
                                                           VkBool32 restartEnabled = 0);
/**
 * Creates a configured viewport state object. Since we're using dynamic viewports, we only need
 * to specify how many viewports we're using
 * @param viewportCount the number of viewports expected by the Pipeline
 * @param scissorCount the number of scissors expected by the Pipeline
 * @return a newly created VkPipelineViewportStateCreateInfo struct
 */
VkPipelineViewportStateCreateInfo CreateViewportState(uint32_t viewportCount,
                                                      uint32_t scissorCount);
/**
 * Creates a configured Pipeline Dynamic state object
 * @param dynamicStateCount the number of dynamic states stored by the Pipeline
 * @param states an array of dynamic states stored expected by the Pipeline
 * @return a newly created VkPipelineViewportStateCreateInfo struct
 */
VkPipelineDynamicStateCreateInfo CreateDynamicStates(uint32_t dynamicStateCount,
                                                     VkDynamicState* states);
/**
 * Creates a configured vertex Shader Stage object
 * @param shaderModule the shader module held by the stage. Must point to a vertex shader
 * @return a newly created VkPipelineShaderStageCreateInfo struct
 */
VkPipelineShaderStageCreateInfo CreateVertexShaderStage(VkShaderModule shaderModule);

/**
 * Creates a configured fragment Shader Stage object
 * @param shaderModule the shader module held by the stage. Must point to a fragment shader
 * @return a newly created VkPipelineShaderStageCreateInfo struct
 */
VkPipelineShaderStageCreateInfo CreateFragmentShaderStage(VkShaderModule shaderModule);

/**
 * Creates a vertex input state object for the Pipeline
 * @param attributeCount the number of vertex attributes expected by the Pipeline
 * @param bindingCount the number of bindings expected by the Pipeline
 * @param attributes an array pointing to the Pipeline vertex attributes
 * @param bindings an array pointing to the Pipeline vertex bindings
 * @return a newly created VkPipelineVertexInputStateCreateInfo struct
 */
VkPipelineVertexInputStateCreateInfo CreateVertexInputInfo(
    uint32_t attributeCount,
    uint32_t bindingCount,
    const VkVertexInputAttributeDescription* attributes,
    const VkVertexInputBindingDescription* bindings);
} // namespace Siege::Vulkan::Utils::Pipeline

#endif // SIEGE_ENGINE_VULKAN_UTILS_PIPELINE_H
