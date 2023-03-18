//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDER_PASS_H
#define SIEGE_ENGINE_RENDER_PASS_H

#include "../Core.h"
#include "render/renderer/platform/vulkan/CommandBuffer.h"
#include "utils/Attachments.h"

namespace Siege
{
/**
 * @brief The RenderPass class is a utilitly class which encapsulates the Vulkan RenderPass
 * capabilities. The class holds an instance of a VkRenderPass while handling Vulkan renderpass
 * initialisation, destruction, and recording stages.
 *
 * In Vulkan, a RenderPass represents a single set of rendering instructions. It may be useful to
 * think of renderpasses as different stages in the rendering process where one pass may render
 * objects, another may render shadows, and a third for compute shaders. Renderpasses allow us to
 * segment our operations into logical chunks.
 */
class RenderPass
{
public:

    /**
     * @brief The config class is a container for storing all of our render pass information before
     creating it.
     * This class is to be used for storing render pass attachments and subpass information. This
     class follows the
     * builder pattern. It is intended to be created via the chaining of functions such as:
     * ```
     * RenderPass::CreateConfig().WithAttachment(Attachments::CreateColorAttachment(swapChainImageFormat))
                                 .WithSubPass(Attachments::CreateSubPass(...)
                                 .WithDependency(Attachments::CreateSubPassDependency();
     * ```
     */
    class Config
    {
    public:

        /**
         * @brief A default constructor for the class. Currently does not accept any parameters.
         */
        Config() = default;

        /**
         * @brief A default destructor for the class. This class will not be containing any
         * allocated memory, so a destructor is not needed.
         */
        ~Config() = default;

        /**
         * @brief Specifies the maximum number of attachments that can be configured.
         */
        static constexpr uint32_t MAX_ATTACHMENTS = 2;

        /**
         * @brief Specifies the maximum number of subpasses that can be configured.
         */
        static constexpr uint32_t MAX_SUBPASSES = 1;

        /**
         * @brief Specifies the maximum number of subpass dependencies that can be configured.
         */
        static constexpr uint32_t MAX_DEPENDENCIES = 1;

        /**
         * @brief Adds a render pass attachment to the Config class. The number of attachments
         * allowed is based off the MAX_ATTACHMENTS static variable.
         * @param attachment the VkAttachmentDescription being added.
         * @return the current object.
         */
        Config& WithAttachment(const VkAttachmentDescription& attachment);

        /**
         * @brief Adds a subpass to the Config class. The number of subpasses allowed is based off
         * the MAX_SUBPASSES static variable.
         * @param subpass the VkSubpassDescription to be added.
         * @return the current object.
         */
        Config& WithSubPass(const VkSubpassDescription& subpass);

        /**
         * @brief Adds a subpass dependency to the Config class. The number of dependencies allowed
         * is based off the MAX_DEPENDENCIES static variable.
         * @param dependency the VkSubpassDependency to be added.
         * @return the current object.
         */
        Config& WithDependency(const VkSubpassDependency& dependency);

        /**
         * @brief Returns the array storing our attachments.
         * @return a MSArray with our renderpass attachments.
         */
        const MSArray<VkAttachmentDescription, MAX_ATTACHMENTS>& GetAttachments() const
        {
            return attachments;
        }

        /**
         * @brief Returns the array storing our subpasses.
         * @return a MSArray with our renderpass subpasses.
         */
        const MSArray<VkSubpassDescription, MAX_SUBPASSES>& GetSubPasses() const
        {
            return subpasses;
        }

        /**
         * @brief Returns an array storing our subpass dependencies.
         * @return a MSArray with our subpass dependencies.
         */
        const MSArray<VkSubpassDependency, MAX_DEPENDENCIES>& GetDependencies() const
        {
            return dependencies;
        }

    private:

        MSArray<VkAttachmentDescription, MAX_ATTACHMENTS> attachments;
        MSArray<VkSubpassDescription, MAX_SUBPASSES> subpasses;
        MSArray<VkSubpassDependency, MAX_DEPENDENCIES> dependencies;
    };

    /**
     * @brief An empty constructor. The RenderPass class relies on the Build() method to initialize
     * the Vulkan RenderPass object.
     */
    RenderPass();

    /**
     * @brief Initialises the RenderPass. Creating a RenderPass requires us to explicitly state what
     * operations the RenderPass will be responsible for, along with any graphics stages the
     * renderpass may rely on.
     */
    RenderPass(const Config& config)
    {
        Initialise(config);
    }

    inline RenderPass(RenderPass&& other)
    {
        Swap(other);
    }

    /**
     * @brief Initialises the RenderPass. Creating a RenderPass requires us to explicitly state what
     * operations the RenderPass will be responsible for, along with any graphics stages the
     * renderpass may rely on.
     *
     * @param config Config variables used to create the renderpass
     */
    void Initialise(const Config& config);

    /**
     * @brief A shorthand function for creating an empty Config.
     * @return an empty Config object.
     */
    static Config CreateConfig()
    {
        return Config {};
    }

    /**
     * @brief A static function for creating a renderpass. Does the same thing as the renderpass'
     * Initialise function.
     * @param renderpass the RenderPass to be configured.
     * @param config the RenderPass' configuration options.
     */
    static void Initialise(RenderPass& renderpass, const Config& config);

    /**
     * @brief Begins a RenderPass. Any rendering operations that occur will utilise the attachments
     * and operations we specified when initialising the RenderPass. Vulkan will continue to use
     * this RenderPass for rendering images until we instruct it to stop by using the End()
     * function.
     *
     * @param renderPass The renderpass we wish to render with
     * @param commandBuffer The command buffer used to record the commands
     * @param frameBuffer The framebuffer used to store our rendering data
     * @param offset Specifies any offsets to be used to the positions of rendered objects
     * @param extent The size of the rendered area.
     * @param clearValues A list of clear values. When a frame is cleared, Vulkan will fill the
     * space with the colors specified here
     * @param clearValueCount The number of clear values provided
     */
    static void Begin(VkRenderPass renderPass,
                      Vulkan::CommandBuffer& commandBuffer,
                      VkFramebuffer frameBuffer,
                      VkOffset2D offset,
                      VkExtent2D extent,
                      const VkClearValue* clearValues,
                      uint32_t clearValueCount);

    /**
     * @brief Ends the RenderPass. Calling this will consolidate all the rendering data into the
     * RenderPass and allow a new RenderPass to be recorded.
     *
     * @param commandBuffer The command buffer currently being recorded to
     */
    static void End(Vulkan::CommandBuffer& commandBuffer);

    inline RenderPass& operator=(RenderPass&& other) noexcept
    {
        Swap(other);
        return *this;
    }

    /**
     * @brief Returns the Vulkan RenderPass object.
     *
     * @return the VkRenderPass object held by this instance
     */
    VkRenderPass GetRenderPass()
    {
        return renderPass;
    }

    /**
     * @brief Cleans up the Vulkan RenderPass object.
     */
    ~RenderPass();

    /**
     * @brief Function for explicitly destroying the Vulkan RenderPass object. Should only be used
     * when the destruction sequence of objects needs to be manually handled.
     */
    void DestroyRenderPass();

private:

    void Swap(RenderPass& other);

    VkRenderPass renderPass {VK_NULL_HANDLE};
};
} // namespace Siege

#endif // SIEGE_ENGINE_RENDER_PASS_H
