#pragma once

#include "../../Core.h"

namespace SnekVk
{
    class Attachments
    {
    public:
        static VkAttachmentDescription CreateColorAttachment(VkFormat format);

        static VkAttachmentDescription CreateDepthAttachment(VkFormat format);

        static VkAttachmentDescription CreateAttachment(VkFormat format,
                                                        VkSampleCountFlagBits samples,
                                                        VkAttachmentLoadOp loadOp,
                                                        VkAttachmentStoreOp storeOp,
                                                        VkAttachmentLoadOp stencilLoadOp,
                                                        VkAttachmentStoreOp stencilStoreOp,
                                                        VkImageLayout initialLayout,
                                                        VkImageLayout finalLayout);

        static VkAttachmentReference CreateAttachmentReference(u32 attachment, VkImageLayout imageLayout);

        static VkAttachmentReference CreateDepthStencilAttachmentReference(u32 attachment);

        static VkAttachmentReference CreateColorAttachmentReference(u32 attachment);

        static VkSubpassDescription CreateSubpass(VkPipelineBindPoint bindPoint,
                                                  u32 colorAttachmentCount = 0,
                                                  const VkAttachmentReference* colorAttachments = VK_NULL_HANDLE,
                                                  const VkAttachmentReference* depthStencilAttachment = VK_NULL_HANDLE,
                                                  const VkAttachmentReference* resolveAttachments = VK_NULL_HANDLE,
                                                  u32 preserveAttachmentCount = 0,
                                                  const u32* preserveAttachments = VK_NULL_HANDLE,
                                                  u32 inputAttachmentCount = 0,
                                                  const VkAttachmentReference* inputAttachments = VK_NULL_HANDLE,
                                                  VkSubpassDescriptionFlags flags = 0);

        static VkSubpassDescription CreateGraphicsSubpass(u32 colorAttachmentCount = 0,
                                                  const VkAttachmentReference* colorAttachments = VK_NULL_HANDLE,
                                                  const VkAttachmentReference* depthStencilAttachment = VK_NULL_HANDLE,
                                                  const VkAttachmentReference* resolveAttachments = VK_NULL_HANDLE,
                                                  u32 preserveAttachmentCount = 0,
                                                  const u32* preserveAttachments = VK_NULL_HANDLE,
                                                  u32 inputAttachmentCount = 0,
                                                  const VkAttachmentReference* inputAttachments = VK_NULL_HANDLE,
                                                  VkSubpassDescriptionFlags flags = 0);

        static VkSubpassDependency CreateDependency(u32 srcSubpass,
                                                    u32 dstSubpass,
                                                    VkPipelineStageFlags srcStageMask,
                                                    VkPipelineStageFlags dstStageMask,
                                                    VkAccessFlags srcAccessMask,
                                                    VkAccessFlags dstAccessMask);
    };
}

