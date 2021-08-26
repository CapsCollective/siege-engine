#pragma once

#include "../Core.h"

namespace SnekVk
{   
    class PipelineConfig
    {
        public:
        /**
         * @param pNext a pointer to a struct that's extending this struct. Defaults to nullptr.
         * @param flags any specific flags that need to be set. Defaults to 0.
         * @param topology specifies how vertices are to be interpreted. Defaults to a triangle list.
         * @param restartEnabled Allows a special vertex value to restart the assembly. Cannot be true with list topologies. Defaults to false.
         * @returns a VkPipelineInputAssemblyStateCreateInfo struct.
         **/
        static VkPipelineInputAssemblyStateCreateInfo InitInputAssemblyStateCreateInfo(
            const void* pNext = nullptr, 
            VkPipelineInputAssemblyStateCreateFlags flags = 0, 
            VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            VkBool32 restartEnabled = VK_FALSE
        );

        /**
         * @param viewportCount the number of viewports used by the shader
         * @param pViewPorts a pointer to an array of viewports
         * @param scissorCount the number of scissors (must match the number of viewports)
         * @param pScissors a pointer to an array of rect2Ds (specifying the scissors)
         * @param flags configuration flags, defaults to 0.
         * @param pNext a pointer to a struct which is extending this struct. 
         * @returns a VkPipelineViewportStateCreateInfo struct with the inputted fields.
         **/
        static VkPipelineViewportStateCreateInfo InitViewPortCreateInfo(
            u32 viewportCount,
            const VkViewport* pViewports,
            u32 scissorCount,
            const VkRect2D* pScissors,
            VkPipelineViewportStateCreateFlags flags = 0,
            const void* pNext = nullptr
        );


        /**
         * @param depthClampEnable controls whether the fragment's depth values should be clamped.
         * @param rasterizerDiscardEnable controls if primitives are discarded immediately before rasterisation stage.
         * @param polygonMode controls how the polygon is rendered.
         * @param cullMode the tringle facing direction for culling.
         * @param frontFace specifies the front-face triangle orinetation for culling.
         * @param depthBiasEnable enables biasing fragment depth values by a number.
         * @param depthBiasConstantFactor specifies a constant depth for all fragments. Defaults to 0.0f.
         * @param depthBiasClamp the maximum depth bias value for a fragment. Defaults. 0.0f.
         * @param depthBiasSlopeFactor scalar factor applied for a fragment's depth bias. Defaults to 0.0f.
         * @param lineWidth the width of line segments. Defaults to 1.0f.
         * @param flags configuration flags. Defaults to 0.
         * @param pNext a pointer to a struct that extends this struct. Defaults to nullptr.
         * @returns a VkPipelineRasterizationStateCreateInfo struct with all inputted fields.
         **/
        static VkPipelineRasterizationStateCreateInfo InitRasterizationCreateInfo(
            VkBool32 depthClampEnable,
            VkBool32 rasterizerDiscardEnable,
            VkPolygonMode polygonMode,
            VkCullModeFlags cullMode,
            VkFrontFace frontFace, 
            VkBool32 depthBiasEnable,
            float depthBiasConstantFactor = 0.0f,
            float depthBiasClamp = 0.0f,
            float depthBiasSlopeFactor = 0.0f,
            float lineWidth = 1.0f,
            VkPipelineRasterizationStateCreateFlags flags = 0,
            const void* pNext = nullptr
        );
        private:
    };
}