#pragma once

#include "../Device/VulkanDevice.h"
#include "../RenderPass/RenderPass.h"

namespace SnekVk
{
    /**
     * @brief The Swapchain class defines the creation and usage of a graphics swapchain. 
     * Swapchains define how images are rendered to screen. Generally, rendered objects are
     * drawn to images, which are contained within command buffers. Each frame, an image is drawn
     * to a command buffer which is then presented for rendering.
     * 
     * When an image takes longer than the given time for a frame, it runs the risk of overlapping
     * with the image being written in the following frame. This often results in screen tearing, 
     * where two images are overlain on top of each other. 
     * 
     * The swapchain is therefore responsible for buffering these images by writing to a new commandbuffer 
     * each frame. The new commandbuffer is then swapped out each frame depending on the method used. This
     * results in smooth image transitions and safer memory usage, as frames no longer compete to render the same data. 
     * 
     * Swapchains are essential for computer graphics processing. 
     */
    class SwapChain
    {
        public:

        /** 
         *  When swapping out images for our frames, we can have multiple frames 'in flight', 
         *  meaning frames that act as additional memory buffers. In our case we allow for two additional
         *  frames in flight for added image buffering.
         **/
        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

        // 'Structors 

        /**
         * @brief Creates a new Swapchain object to buffer our images. 
         * 
         * @param device a reference to a device. Devices are always destroyed last, so we can 
         * guarantee that object destruction will occur at the correct time. 
         * @param windowExtent the window resolution. Images need to be scaled to an extent, this usually
         * must follow the extents of the window. 
         */
        SwapChain(VulkanDevice& device, VkExtent2D windowExtent);
        ~SwapChain();

        // Delete copy constructors. 
        SwapChain(const SwapChain&) = delete;
        void operator=(const SwapChain*) = delete;

        // Getters

        /**
         * @brief Images can be formatted in a number of ways. The renderer currently 
         * searches for a specific format to render to. This function returns the format
         * currently being used by the renderer.
         * 
         * @return VkFormat the format being used by the renderer. 
         */
        VkFormat GetSwapChainImageFormat() { return swapChainImageFormat; }

        /**
         * @brief Returns the image width and height that images are being rendered to. 
         * 
         * @return VkExtent2D a 2D representation of the height and width of the screen. 
         */
        VkExtent2D GetSwapChainExtent() { return swapChainExtent; }
        
        /**
         * @brief Get a frame buffer object corresponding to index i. The swapchain object stores multiple
         * frame buffers equal to the number of images that can be held at once. 
         * 
         * @param i the index of the required framebuffer. NOTE: this can return a segfault if an incorrect index is provided. Please ensure that the index provided
         * is between 0 and the swapchain's max image count. 
         * @return VkFramebuffer the framebuffer in index i
         */
        VkFramebuffer GetFrameBuffer(u32 i) { return swapChainFrameBuffers[i]; }

        /**
         * @brief Get the number of images that can be active at once. 
         * 
         * @return u32 representing the number of possible active simultaneous images. 
         */
        u32 GetImageCount() { return imageCount; }

        /**
         * @brief The the raw Vulkan swapchain struct. 
         * 
         * @return VkSwapchainKHR 
         */
        VkSwapchainKHR GetSwapChain() { return swapChain; }

        /**
         * @brief Get the swapchain image width.
         * 
         * @return u32 representing the width of the image. 
         */
        u32 GetWidth() { return swapChainExtent.width; }

        /**
         * @brief Get the swapchain image height.
         * 
         * @return u32 representing the image height.
         */
        u32 GetHeight() { return swapChainExtent.height; }

        /**
         * @brief Get the current render pass object. 
         * 
         * @return RenderPass - the raw vulkan render pass object.
         */
        RenderPass* GetRenderPass() { return &renderPass; }

        /**
         * @brief Loads in the next image to be written to in the Swapchain. 
         * 
         * @param imageIndex - the index of the next image. 
         * @return VkResult - the result of acquiring the next image. 
         */
        VkResult AcquireNextImage(u32* imageIndex);

        /**
         * @brief Submits a command buffer for drawing.
         * 
         * @param buffers - an array of command buffers. 
         * @param imageIndex - the index of the image being drawn. 
         * @return VkResult - the result of submitting the buffer. 
         */
        VkResult SubmitCommandBuffers(const VkCommandBuffer* buffers, u32* imageIndex);

        private:
        
        /**
         * @brief Create a Vulkan swapchain object. 
         */
        void CreateSwapChain();

        /**
         * @brief Create image views for our images. An image view represents the subresources
         * that images must have to be valid. An image view must be created for each image being drawn.
         */
        void CreateImageViews();

        /**
         * @brief Creates a render pass. In Vulkan, render passes are objects that represent 
         * the state that rendering resources must be in to render to an image. All image rendering
         * must occur within a render pass, while compute operations do not. 
         */
        void CreateRenderPass();

        /**
         * @brief Create a Depth Resources object. Depth resources represent how object depth is calculated.
         * These resources are written to their own resource views. 
         */
        void CreateDepthResources();

        /**
         * @brief Creates the swapchain's frame buffers. A frame buffer represents all the data needed to be submitted 
         * to the GPU in a frame. 
         */
        void CreateFrameBuffers();

        /**
         * @brief Initialises all thread synchronisation objects. Vulkan is multi-threaded and submits images
         * asynchronously. This can be dangerous when submitting images to the GPU since Vulkan does not guarantee
         * safe execution. Therefore we must handle our own thread synchronisation. 
         */
        void CreateSyncObjects();

        /**
         * @brief Specifies which color format we want images to be written to. Accepts a set of formats
         * and populates them with requisite data. 
         * 
         * @param formats - an array of possible formats. 
         * @param formatCount - the number of formats provided. 
         * @return VkSurfaceFormatKHR - the chosen 
         */
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(VkSurfaceFormatKHR* formats, size_t formatCount);

        /**
         * @brief Chooses the presentation mode that the window surface will use for images. 
         * 
         * @param presentModes - an array of possible presentation modes. 
         * @param presentModeCount  - the number of presentation modes in the array. 
         * @return VkPresentModeKHR - the chosen presentation mode. 
         */
        VkPresentModeKHR ChoosePresentMode(VkPresentModeKHR* presentModes, size_t presentModeCount);

        /**
         * @brief Chooses an image size extent that's supported by the swapchain.
         * 
         * @param capabilities a reference to a struct containing our swapchain capabilities. 
         * @return VkExtent2D - an object containing the swapchain's supported extents. 
         */
        VkExtent2D ChooseSwapExtent(VkSurfaceCapabilitiesKHR& capabilities);

        /**
         * @brief Finds the depth format supported by this swapchain. 
         * 
         * @return VkFormat - the supported image format. 
         */
        VkFormat FindDepthFormat();
        
        // Device and window data
        VulkanDevice& device;
        VkExtent2D windowExtent;

        // frame buffers and renderpasses
        VkFramebuffer* swapChainFrameBuffers;
        RenderPass renderPass;

        // Swapchain image data
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

        // Depth image data.
        // NOTE: If these values do end up being changed dynamically, it might be a good idea to
        // wrap them in a vector.
        VkImage* depthImages;
        VkDeviceMemory* depthImageMemorys;
        VkImageView* depthImageViews;

        // Images and image views supported by the swapchain
        VkImage* swapChainImages;
        VkImageView* swapChainImageViews;
        u32 imageCount;

        // The raw Vulkan swapchain object
        VkSwapchainKHR swapChain;

        // Synchronisation objects
        VkSemaphore* imageAvailableSemaphores;
        VkSemaphore* renderFinishedSemaphores;
        VkFence* inFlightFences;
        VkFence* imagesInFlight;
        size_t currentFrame = 0;
    };
}