//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_FRAMEBUFFER_H
#define SIEGE_ENGINE_FRAMEBUFFER_H

#include "../Core.h"
#include "../image/FrameImages.h"

namespace Siege
{

/**
 * @brief A class representing a Framebuffer in Vulkan. This class will store the Vulkan framebuffer
 * and handle it's creation using a Config sub-object. This class is unique in that it creates a
 * framebuffer per image supported by the GPU. This value is represented by the IMAGE_COUNT static
 * variable.
 */
class Framebuffer
{
public:

    /**
     * @brief The Config class acts as a container of useful information that framebuffers require
     * to be built. This class follows the Builder pattern and is intended to utilise function
     * chaining for readability.
     */
    class Config
    {
    public:

        /**
         * @brief An empty default constructor.
         */
        Config() = default;

        /**
         * @brief An empty default destructor.
         */
        ~Config() = default;

        /**
         * @brief Adds a renderpass to the configuration. This renderpass will be used to create the
         * framebuffer.
         * @param targetRenderPass
         * @return the current builder object.
         */
        Config& WithRenderPass(VkRenderPass targetRenderPass);

        /**
         * @brief Adds a set of image views that correspond to our color attachment.
         * @param targetAttachments
         * @return the current builder object.
         */
        Config& WithColorAttachments(FrameImages* targetAttachments);

        /**
         * @brief Adds a set of image views that correspond to our depth attachment.
         * @param targetAttachments
         * @return the current builder object.
         */
        Config& WithDepthAttachments(FrameImages* targetAttachments);

        /**
         * @brief Specifies the framebuffer rendering size. This usually corresponds to the
         * swapchain image size.
         * @param targetWidth the width of the rendering space.
         * @param targerHeight the height of the rendering space.
         * @return the current builder object.
         */
        Config& WithImageDimensions(uint32_t targetWidth, uint32_t targerHeight);

        /**
         * @brief Specifies the number of layers this framebuffer will be operating over.
         * @param targetLayers the number of layers.
         * @return the current builder object.
         */
        Config& WithLayers(uint32_t targetLayers);

        /**
         * @brief Returns the renderpass being used for the framebuffer.
         * @return the Config's renderpass
         */
        const VkRenderPass& GetRenderPass() const
        {
            return renderPass;
        }

        /**
         * @brief Returns the attachments used by this framebuffer.
         * @return the Config's color attachments.
         */
        const FrameImages* GetColorAttachments() const
        {
            return colorAttachments;
        }

        /**
         * @brief Returns the depth attachments used by this framebuffer.
         * @return the Config's depth attachments.
         */
        const FrameImages* GetDepthAttachments() const
        {
            return depthAttachments;
        }

        /**
         * @brief Returns the number of attachments provided to the Config.
         * @return the number of attachments.
         */
        const uint32_t& GetAttachmentCount() const
        {
            return attachmentCount;
        }

        /**
         * @brief Returns the framebuffer's rendering area width.
         * @return the rendering area's width.
         */
        const uint32_t& GetWidth() const
        {
            return width;
        }

        /**
         * @brief Returns the framebuffer's rendering area height.
         * @return the rendering area's height.
         */
        const uint32_t& GetHeight() const
        {
            return height;
        }

        /**
         * @brief Gets the number of configured layers.
         * @return the number of layers to render to.
         */
        const uint32_t& GetLayers() const
        {
            return layers;
        }

    private:

        VkRenderPass renderPass {VK_NULL_HANDLE};
        FrameImages* colorAttachments {nullptr};
        FrameImages* depthAttachments {nullptr};
        uint32_t attachmentCount {0};
        uint32_t width {0};
        uint32_t height {0};
        uint32_t layers {0};
    };

    /**
     * @brief Sets the value of IMAGE_COUNT. This enforces that the number of allocated framebuffers
     * never exceeds the provided number of images.
     * @param imageCount the number of images available to the swapchain.
     */
    static void SetImageCount(const uint32_t& imageCount)
    {
        IMAGE_COUNT = imageCount;
    }

    /**
     * @brief A default empty constructor for the Framebuffer.
     */
    Framebuffer() = default;

    /**
     * @brief A constructor for Framebuffer which allows the Framebuffer to be initialised using a
     * Config object.
     * @param config
     * @param device
     */
    explicit Framebuffer(const Config& config, const VkDevice& device);

    /**
     * @brief A default empty destructor.
     */
    ~Framebuffer();

    Framebuffer& operator=(Framebuffer&& other) noexcept;

    /**
     * @brief Initialises the Framebuffer using the Configs and Device objects.
     * @param config the configuration data which will be used to create the Framebuffer.
     * @param device the device used to allocate the framebuffer.
     */
    void Initialise(const Config& config, const VkDevice& device);

    /**
     * @brief Returns a VkFramebuffer object corresponding to the provided index.
     * @param index the frame index for the framebuffer.
     * @return the corresponding VkFramebuffer.
     */
    VkFramebuffer GetFramebuffer(const uint32_t& index)
    {
        return framebuffers[index];
    }

    /**
     * @brief Deallocates the memory used by the framebuffers.
     * @param device the GPU used to allocate this data.
     */
    void DestroyFramebuffer(const VkDevice& device);

private:

    // A static count of allowed images.
    static uint32_t IMAGE_COUNT;
    HeapArray<VkFramebuffer> framebuffers;
};

} // namespace Siege

#endif // SIEGE_ENGINE_FRAMEBUFFER_H
