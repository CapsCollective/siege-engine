#include "RenderPass.h"

namespace SnekVk
{

    RenderPass::~RenderPass()
    {
        DestroyRenderPass();
    }

    void RenderPass::DestroyRenderPass()
    {
        vkDestroyRenderPass(device->Device(), renderPass, nullptr);
    }

    void
    RenderPass::Begin(VkRenderPass renderPass, VkCommandBuffer commandBuffer, VkFramebuffer frameBuffer, VkOffset2D offset, VkExtent2D extent,
                      VkClearValue *clearValues, u32 clearValueCount)
    {
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = frameBuffer;

        renderPassInfo.renderArea.offset = offset;
        renderPassInfo.renderArea.extent = extent;

        renderPassInfo.clearValueCount = clearValueCount;
        renderPassInfo.pClearValues = clearValues;

        vkCmdBeginRenderPass(OUT commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void RenderPass::End(VkCommandBuffer commandBuffer)
    {
        vkCmdEndRenderPass(OUT commandBuffer);
    }

    RenderPass::RenderPass() = default;

    void RenderPass::Build(VulkanDevice* vulkanDevice,
                           VkAttachmentDescription *attachments,
                           u32 attachmentCount,
                           VkSubpassDescription *subpasses,
                           u32 subpassCount,
                           VkSubpassDependency *dependencies,
                           u32 dependencyCount)
    {
        device = vulkanDevice;

        VkRenderPassCreateInfo renderPassCreateInfo{};
        renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCreateInfo.attachmentCount = attachmentCount;
        renderPassCreateInfo.pAttachments = attachments;
        renderPassCreateInfo.subpassCount = subpassCount;
        renderPassCreateInfo.pSubpasses = subpasses;
        renderPassCreateInfo.dependencyCount = dependencyCount;
        renderPassCreateInfo.pDependencies = dependencies;

        SNEK_ASSERT(vkCreateRenderPass(device->Device(), &renderPassCreateInfo, nullptr, OUT &renderPass) == VK_SUCCESS,
                    "Failed to create render pass!")
    }
}