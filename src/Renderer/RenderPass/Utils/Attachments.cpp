#include "Attachments.h"

namespace SnekVk
{
    VkAttachmentDescription Attachments::CreateColorAttachment(VkFormat format)
    {
        return CreateAttachment(format,
                                VK_SAMPLE_COUNT_1_BIT,
                                VK_ATTACHMENT_LOAD_OP_CLEAR,
                                VK_ATTACHMENT_STORE_OP_STORE, // We do care about the results of the render pass, so keep it stored
                                VK_ATTACHMENT_LOAD_OP_DONT_CARE, // No stencil data, so we don't care what happens here
                                VK_ATTACHMENT_STORE_OP_DONT_CARE, // We don't care about the results of our stencil operations either
                                VK_IMAGE_LAYOUT_UNDEFINED,
                                VK_IMAGE_LAYOUT_PRESENT_SRC_KHR); // In the end we want this image to end up ready for presentation
    }

    VkAttachmentDescription Attachments::CreateDepthAttachment(VkFormat format)
    {
        return CreateAttachment(format,
                                VK_SAMPLE_COUNT_1_BIT, // We'll only use one sample per pixel for now
                                VK_ATTACHMENT_LOAD_OP_CLEAR, // When starting the render pass, clear the entire render area
                                VK_ATTACHMENT_STORE_OP_DONT_CARE, // Discard the end data from a render pass - we don't need to re-use it
                                VK_ATTACHMENT_LOAD_OP_DONT_CARE, // We don't care what happens with stencil data
                                VK_ATTACHMENT_STORE_OP_DONT_CARE, // We don't care what happens to the results of a stencil render pass
                                VK_IMAGE_LAYOUT_UNDEFINED, // Layout will start empty
                                VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL); // layout will become a depth stencil
    }

    VkAttachmentDescription
    Attachments::CreateAttachment(VkFormat format,
                                  VkSampleCountFlagBits samples,
                                  VkAttachmentLoadOp loadOp,
                                  VkAttachmentStoreOp storeOp,
                                  VkAttachmentLoadOp stencilLoadOp,
                                  VkAttachmentStoreOp stencilStoreOp,
                                  VkImageLayout initialLayout,
                                  VkImageLayout finalLayout)
    {
        return
        {
                0, format, samples, loadOp, storeOp, stencilLoadOp, stencilStoreOp, initialLayout, finalLayout
        };
    }

    VkSubpassDependency Attachments::CreateDependency(u32 srcSubpass,
                                                      u32 dstSubpass,
                                                      VkPipelineStageFlags srcStageMask,
                                                      VkPipelineStageFlags dstStageMask,
                                                      VkAccessFlags srcAccessMask,
                                                      VkAccessFlags dstAccessMask)
    {
        return
        {
            srcSubpass, dstSubpass, srcStageMask, dstStageMask, srcAccessMask, dstAccessMask
        };
    }

    VkAttachmentReference Attachments::CreateAttachmentReference(u32 attachment, VkImageLayout imageLayout)
    {
        return { attachment, imageLayout };
    }

    VkSubpassDescription Attachments::CreateSubpass(VkPipelineBindPoint bindPoint,
                                              u32 colorAttachmentCount,
                                              const VkAttachmentReference* colorAttachments,
                                              const VkAttachmentReference* depthStencilAttachment,
                                                const VkAttachmentReference* resolveAttachments,
                                              u32 preserveAttachmentCount,
                                              const u32* preserveAttachments,
                                              u32 inputAttachmentCount,
                                              const VkAttachmentReference* inputAttachments,
                                              VkSubpassDescriptionFlags flags)
    {
        return
        {
            flags,
            bindPoint,
            inputAttachmentCount,
            inputAttachments,
            colorAttachmentCount,
            colorAttachments,
            resolveAttachments,
            depthStencilAttachment,
            preserveAttachmentCount,
            preserveAttachments
        };
    }

    VkSubpassDescription
    Attachments::CreateGraphicsSubpass(u32 colorAttachmentCount,
                                       const VkAttachmentReference *colorAttachments,
                                       const VkAttachmentReference *depthStencilAttachment,
                                       const VkAttachmentReference *resolveAttachments,
                                       u32 preserveAttachmentCount,
                                       const u32 *preserveAttachments,
                                       u32 inputAttachmentCount,
                                       const VkAttachmentReference *inputAttachments,
                                       VkSubpassDescriptionFlags flags)
    {
        return CreateSubpass(VK_PIPELINE_BIND_POINT_GRAPHICS,
                      colorAttachmentCount,
                      colorAttachments,
                      depthStencilAttachment,
                      resolveAttachments,
                      preserveAttachmentCount,
                      preserveAttachments,
                      inputAttachmentCount,
                      inputAttachments,
                      flags);
    }

    VkAttachmentReference Attachments::CreateDepthStencilAttachmentReference(u32 attachment)
    {
        return CreateAttachmentReference(attachment, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    }

    VkAttachmentReference Attachments::CreateColorAttachmentReference(u32 attachment)
    {
        return CreateAttachmentReference(attachment, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    }
}