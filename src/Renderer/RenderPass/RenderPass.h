#pragma once

#include "../Core.h"
#include "../Device/VulkanDevice.h"
#include "Utils/Attachments.h"

namespace SnekVk
{

    class RenderPass
    {
    public:

        RenderPass();

        void Build(VulkanDevice* vulkanDevice,
                   VkAttachmentDescription* attachments,
                   u32 attachmentCount,
                   VkSubpassDescription* subpasses,
                   u32 subpassCount,
                   VkSubpassDependency* dependencies,
                   u32 dependencyCount);

        static void Begin(VkRenderPass renderPass,
                          VkCommandBuffer commandBuffer,
                          VkFramebuffer frameBuffer,
                          VkOffset2D offset,
                          VkExtent2D extent,
                          VkClearValue* clearValues,
                          u32 clearValueCount);

        static void End(VkCommandBuffer commandBuffer);

        VkRenderPass GetRenderPass() { return renderPass; }

        ~RenderPass();

        void DestroyRenderPass();

    private:
        VkRenderPass renderPass {VK_NULL_HANDLE};
        VulkanDevice* device {nullptr};
    };
}

