//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_PIPELINE_H
#define SIEGE_ENGINE_VULKAN_PIPELINE_H

#include <utils/collections/StackArray.h>

#include "CommandBuffer.h"
#include "Shader.h"
#include "render/renderer/pass/RenderPass.h"
#include "utils/Types.h"

namespace Siege::Vulkan
{
/**
 *  The Pipeline class represents a Vulkan graphics pipeline. The class provides a base set of
 *  configuration options for graphics pipelines and provides a set of binding options for
 *  descriptor sets.
 *
 *  @param layout the pipeline layout
 *  @param pipeline the raw vulkan pipeline object
 */
class Pipeline
{
public:

    /**
     *  The Pipeline Builder class is a configuration class for storing and building graphics
     *  pipelines.
     *
     *  @param vertexShader a pointer to the vertex shader used by the pipeline
     *  @param fragmentShader a pointer to the fragment shader used by the pipeline
     *  @param dynamicStates a list of dynamic states used by the pipeline
     *  @param descriptorLayouts a list of descriptor layouts (uniform bindings)
     *  @param topology the topology of the pipeline
     *  @param fillMode the fill mode for pipeline polygons
     *  @param viewportCount the number of viewports
     *  @param scissorCount the number of scissors
     *  @param usingDepthTest a boolean specifying if the pipeline will run a depth test
     *  @param usingDepthWrite a boolean specifying if the pipeline will run a depth write operation
     *  @param renderPass the RenderPass used by the pipeline for rendering
     */
    struct Builder
    {
        // TODO: Do we need any culling information or should we make culling configurable?
        /**
         *  An enum specifying what type of dynamic state the pipeline will use
         */
        enum DynamicState
        {
            DYNAMIC_STATE_VIEWPORT = 0,
            DYNAMIC_STATE_SCISSOR = 1
        };

        /**
         *  An enum specifying the polygon fill mode of the pipeline vertices
         */
        enum PipelineFillMode
        {
            FILL_MODE_FILL = 0
        };

        /**
         * Specifies that a dynamic viewport is going be used by the pipeline
         * @return a reference to the current builder instance
         */
        Builder& WithDynamicViewport();

        /**
         * Specifies that a dynamic scissor is going to be used by the pipeline
         * @return a reference to the current builder instance
         */
        Builder& WithDynamicScissor();

        /**
         * Specifies which render pass the pipeline will use
         * @param targetRenderPass the render pass to use for rendering
         * @return a reference to the current builder instance
         */
        Builder& WithRenderPass(RenderPass* targetRenderPass);

        /**
         * Specifies the vertex shader which will be bound to the pipeline
         * @param vertShader a pointer to a vertex shader
         * @return a reference to the current builder instance
         */
        Builder& WithVertexShader(const Shader* vertShader);

        /**
         * Specifies the fragment shader which will be bound to the pipeline
         * @param fragShader a pointer to a fragment shader
         * @return a reference to the current builder instance
         */
        Builder& WithFragmentShader(const Shader* fragShader);

        /**
         * Specifies the descriptor set layouts used by the pipeline
         * @param layouts an array of descriptor set layouts for the pipeline
         * @return a reference to the current builder instance
         */
        Builder& WithProperties(const ::Siege::Utils::MSArray<VkDescriptorSetLayout, 10>& layouts);

        /**
         * Builds a new graphics Pipeline and returns the newly created instance
         * @return a new Pipeline instance
         */
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

    /**
     * The default empty Pipeline constructor
     */
    Pipeline() = default;
    /**
     * A Pipeline move constructor
     * @param other the Pipeline to be moved
     */
    Pipeline(Pipeline&& other) noexcept
    {
        Swap(other);
    }
    /**
     * The Pipeline destructor
     */
    ~Pipeline();

    /**
     * A Pipeline move assignment operator
     * @param other the Pipeline to be moved
     * @return a reference to the newly created Pipeline instance
     */
    Pipeline& operator=(Pipeline&& other) noexcept
    {
        Swap(other);
        return *this;
    }

    /**
     * Binds the Pipeline to a commandBuffer that's actively being recorded
     * @param commandBuffer the commandBuffer being recorded. Must be active for rendering
     */
    void Bind(const CommandBuffer& commandBuffer);
    void BindSets(const CommandBuffer& commandBuffer,
                  ::Siege::Utils::MSArray<VkDescriptorSet, 2> sets);

private:

    /**
     * Swaps the contents of two Pipelines
     * @param other the Pipeline to swap with
     */
    void Swap(Pipeline& other);
    VkPipelineLayout layout {nullptr};
    VkPipeline pipeline {nullptr};
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VULKAN_PIPELINE_H
