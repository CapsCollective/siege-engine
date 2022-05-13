#pragma once

#include "../../Core.h"

namespace SnekVk
{
    class Attachments
    {
    public:
        /**
         * @brief Creates a default Color Attachment.
         *
         * @param format The image format. This will be the standard image format that our swapchain supports
         * @return A VkAttachmentDescription with default color attachment settings
         */
        static VkAttachmentDescription CreateColorAttachment(VkFormat format);

        /**
         * @brief Creates a default Depth Attachment.
         *
         * @param format The image format. This will be a Depth Format
         * @return A VkAttachmentDescription with default depth attachment settings
         */
        static VkAttachmentDescription CreateDepthAttachment(VkFormat format);

        /**
         * @brief Creates an attachment with the provided attachment data. This is a utility function that creates a
         * VkAttachmentDescription.
         *
         * @param format The format we want to use for this attachment
         * @param samples The number of samples per pixel we want to use
         * @param loadOp Our load operation. This setting dictates what our expected starting state is when this operation occurs
         * @param storeOp Our Store operation. This dictates what we want to do with the results of this operation
         * @param stencilLoadOp If we have a stencil operation, this will dictate what our starting state is expeted to be
         * @param stencilStoreOp Dictates what we want to do with the end result of this stencil operation
         * @param initialLayout Specifies what layout we expect our image to be in when the operation starts
         * @param finalLayout Specifies the image layout we expect to see after the operation has concluded
         * @return
         */
        static VkAttachmentDescription CreateAttachment(VkFormat format,
                                                        VkSampleCountFlagBits samples,
                                                        VkAttachmentLoadOp loadOp,
                                                        VkAttachmentStoreOp storeOp,
                                                        VkAttachmentLoadOp stencilLoadOp,
                                                        VkAttachmentStoreOp stencilStoreOp,
                                                        VkImageLayout initialLayout,
                                                        VkImageLayout finalLayout);

        /**
         * @brief Creates an attachment reference.
         *
         * @param attachment The index of the attachment this reference is referring to
         * @param imageLayout The layout the attachment will be using during our subpass
         * @return A VkAttachmentReference struct
         */
        static VkAttachmentReference CreateAttachmentReference(u32 attachment, VkImageLayout imageLayout);

        /**
         * @brief Creates an attachment reference specifically configured for depth stencils.
         *
         * @param attachment the index of the attachment being referenced
         * @return a VkAttachmentReference struct configured for depth stencils
         */
        static VkAttachmentReference CreateDepthStencilAttachmentReference(u32 attachment);

        /**
         * @brief Creates an attachment reference specifically configured for rendering colors.
         *
         * @param attachment the index of the attachment being referenced
         * @return a VkAttachmentReference struct configured for colors
         */
        static VkAttachmentReference CreateColorAttachmentReference(u32 attachment);

        /**
         * @brief Creates a subpass for a binding with the configured attachments.
         *
         * @param bindPoint The type of instruction being referenced, for example a compute or graphics bind point
         * @param colorAttachmentCount The number of color attachments
         * @param colorAttachments An array of the color attachments being used
         * @param depthStencilAttachment The depth stencil attachment being used
         * @param resolveAttachments The resolve attachments being used
         * @param preserveAttachmentCount The amount of preserve attachments
         * @param preserveAttachments An array of our preserve attachments
         * @param inputAttachmentCount The amount of attachments being used
         * @param inputAttachments An array of the attachments being used
         * @param flags Any flags that have been configured
         *
         * @return A VkSubpassDescription description with our configured data
         */
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

        /**
         * @brief Creates a subpass which binds to a graphics process
         *
         * @param colorAttachmentCount The number of color attachments
         * @param colorAttachments An array of the color attachments being used
         * @param depthStencilAttachment The depth stencil attachment being used
         * @param resolveAttachments The resolve attachments being used
         * @param preserveAttachmentCount The amount of preserve attachments
         * @param preserveAttachments An array of our preserve attachments
         * @param inputAttachmentCount The amount of attachments being used
         * @param inputAttachments An array of the attachments being used
         * @param flags Any flags that have been configured
         *
         * @return A VkSubpassDescription description configured for graphics processes
         */
        static VkSubpassDescription CreateGraphicsSubpass(u32 colorAttachmentCount = 0,
                                                  const VkAttachmentReference* colorAttachments = VK_NULL_HANDLE,
                                                  const VkAttachmentReference* depthStencilAttachment = VK_NULL_HANDLE,
                                                  const VkAttachmentReference* resolveAttachments = VK_NULL_HANDLE,
                                                  u32 preserveAttachmentCount = 0,
                                                  const u32* preserveAttachments = VK_NULL_HANDLE,
                                                  u32 inputAttachmentCount = 0,
                                                  const VkAttachmentReference* inputAttachments = VK_NULL_HANDLE,
                                                  VkSubpassDescriptionFlags flags = 0);

        /**
         * @brief Creates a VkSubpassDependency object with the configured data
         *
         * @param srcSubpass The index of the first subpass
         * @param dstSubpass The index of the second subpass in the dependency
         * @param srcStageMask The graphics stage this subpass should start at
         * @param dstStageMask The graphics stage this subpass should end at
         * @param srcAccessMask The stage where memory should be accessed from initially
         * @param dstAccessMask The stage we should be in following the renderpass
         *
         * @return A VkSubpassDependency struct
         */
        static VkSubpassDependency CreateDependency(u32 srcSubpass,
                                                    u32 dstSubpass,
                                                    VkPipelineStageFlags srcStageMask,
                                                    VkPipelineStageFlags dstStageMask,
                                                    VkAccessFlags srcAccessMask,
                                                    VkAccessFlags dstAccessMask);
    };
}

