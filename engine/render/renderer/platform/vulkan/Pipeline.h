//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_PIPELINE_H
#define SIEGE_ENGINE_VULKAN_PIPELINE_H

#include "utils/Types.h"
#include "render/renderer/utils/StackArray.h"
#include "render/renderer/pass/RenderPass.h"
#include "CommandBuffer.h"
#include "Shader.h"

namespace Siege::Vulkan
{
class Pipeline
{
public:
    struct Builder
    {
        // TODO: Do we need any culling information or should we make culling configurable?
        enum DynamicState
        {
            DYNAMIC_STATE_VIEWPORT = 0,
            DYNAMIC_STATE_SCISSOR = 1
        };

        enum PipelineFillMode
        {
            FILL_MODE_FILL = 0
        };

        Builder& WithDynamicViewport();

        Builder& WithDynamicScissor();

        Builder& WithRenderPass(RenderPass* targetRenderPass);

        Builder& WithVertexShader(const Shader* vertShader);

        Builder& WithFragmentShader(const Shader* fragShader);

        Builder& WithProperties(const ::Siege::Utils::MSArray<VkDescriptorSetLayout, 10>& layouts);

        Pipeline Build();

        const Shader* vertexShader;
        const Shader* fragmentShader;

        ::Siege::Utils::MSArray<DynamicState, 2> dynamicStates;
        ::Siege::Utils::MSArray<VkDescriptorSetLayout, 10> descriptorLayouts;

        Utils::PipelineTopology topology {Utils::TOPOLOGY_TRIANGLE_LIST};
        PipelineFillMode fillMode {PipelineFillMode::FILL_MODE_FILL};
        uint32_t viewportCount {0};
        uint32_t scissorCount {0};

        bool usingDepthTest {true};
        bool usingDepthWrite {true};

        RenderPass* renderPass {nullptr};
    };

    Pipeline() = default;
    Pipeline(Pipeline&& other) noexcept { Swap(other); }
    ~Pipeline();

    Pipeline& operator=(Pipeline&& other) noexcept
    {
        Swap(other);
        return *this;
    }

    void Bind(const CommandBuffer& commandBuffer);
    void BindSets(const CommandBuffer& commandBuffer, ::Siege::Utils::MSArray<VkDescriptorSet, 10> sets);

    // TODO: Remove the next two functions once we incorporate the CommandBuffer class into renderer
    void Bind(VkCommandBuffer commandBuffer);
    void BindSets(VkCommandBuffer, ::Siege::Utils::MSArray<VkDescriptorSet, 10> sets);
private:
    void Swap(Pipeline& other);
    VkPipelineLayout layout {nullptr};
    VkPipeline pipeline {nullptr};
};
}

#endif // SIEGE_ENGINE_VULKAN_PIPELINE_H
