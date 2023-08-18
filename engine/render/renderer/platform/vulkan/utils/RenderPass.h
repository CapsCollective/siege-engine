//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_UTILS_RENDERPASS_H
#define SIEGE_ENGINE_VULKAN_UTILS_RENDERPASS_H

#include <volk/volk.h>

namespace Siege::Vulkan::Utils
{

inline constexpr VkAttachmentDescription DEFAULT_COLOR_ATTACHMENT = {
    0,
    VK_FORMAT_UNDEFINED,
    VK_SAMPLE_COUNT_1_BIT,
    VK_ATTACHMENT_LOAD_OP_CLEAR,
    VK_ATTACHMENT_STORE_OP_STORE, // We do care about the results of the render pass, so keep it
                                  // stored
    VK_ATTACHMENT_LOAD_OP_DONT_CARE, // No stencil data, so we don't care what happens here
    VK_ATTACHMENT_STORE_OP_DONT_CARE, // We don't care about the results of our stencil
                                      // operations either
    VK_IMAGE_LAYOUT_UNDEFINED,
    VK_IMAGE_LAYOUT_PRESENT_SRC_KHR};

inline constexpr VkAttachmentDescription DEFAULT_DEPTH_ATTACHMENT = {
    0,
    VK_FORMAT_UNDEFINED,
    VK_SAMPLE_COUNT_1_BIT, // We'll only use one sample per pixel for now
    VK_ATTACHMENT_LOAD_OP_CLEAR, // When starting the render pass, clear the entire render area
    VK_ATTACHMENT_STORE_OP_DONT_CARE, // Discard the end data from a render pass - we don't need
                                      // to re-use it
    VK_ATTACHMENT_LOAD_OP_DONT_CARE, // We don't care what happens with stencil data
    VK_ATTACHMENT_STORE_OP_DONT_CARE, // We don't care what happens to the results of a stencil
                                      // render pass
    VK_IMAGE_LAYOUT_UNDEFINED, // Layout will start empty
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL // layout will become a depth stencil
};

inline constexpr VkSubpassDescription DEFAULT_GRAPHICS_SUBPASS_DESCRIPTION =
    {0, VK_PIPELINE_BIND_POINT_GRAPHICS, 0, nullptr, 0, nullptr, nullptr, nullptr, 0, nullptr};

inline constexpr unsigned int DEFAULT_FRAGMENT_STAGE_MASK =
    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

inline constexpr unsigned int DEFAULT_DST_ACCESS_MASK =
    VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

inline constexpr VkSubpassDescription CreateGraphicsSubpass(
    VkAttachmentReference* colourAttachments,
    unsigned int colourAttachmentsCount,
    VkAttachmentReference* depthAttachmentReference)
{
    auto defaultDescription = DEFAULT_GRAPHICS_SUBPASS_DESCRIPTION;
    defaultDescription.pColorAttachments = colourAttachments;
    defaultDescription.colorAttachmentCount = colourAttachmentsCount;
    defaultDescription.pDepthStencilAttachment = depthAttachmentReference;

    return defaultDescription;
}

VkRenderPass Create(VkDevice device,
                    VkAttachmentDescription* attachments,
                    unsigned int attachmentCount,
                    VkSubpassDescription* subpasses,
                    unsigned int subpassCount,
                    VkSubpassDependency* dependencies,
                    unsigned int dependencyCount);

void BeginRenderPass(VkCommandBuffer commandBuffer,
                     VkRenderPass renderPass,
                     VkClearValue* clearValues,
                     uint32_t clearValueCount,
                     VkFramebuffer framebuffer,
                     VkRect2D renderArea);
void EndRenderPass(VkCommandBuffer commandBuffer);

void Free(VkDevice device, VkRenderPass renderPass);

} // namespace Siege::Vulkan::Utils

#endif // SIEGE_ENGINE_VULKAN_UTILS_RENDERPASS_H
