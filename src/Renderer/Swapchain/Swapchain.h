#pragma once

#include "../Device/VulkanDevice.h"

namespace SnekVk
{
    class SwapChain
    {
        public:

        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

        SwapChain(VulkanDevice& device, VkExtent2D windowExtent);
        ~SwapChain() {};

        SwapChain(const SwapChain&) = delete;
        void operator=(const SwapChain*) = delete;
        void DestroySwapChain(SwapChain& swapChain);

        VkFormat GetSwapChainImageFormat() { return swapChainImageFormat; }
        VkExtent2D GetSwapChainExtent() { return swapChainExtent; }

        private:
        
        void CreateSwapChain();
        void CreateImageViews();
        void CreateRenderPass();
        void CreateDepthResources();
        void CreateFrameBuffers();

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(VkSurfaceFormatKHR* formats, size_t formatCount);
        VkPresentModeKHR ChoosePresentMode(VkPresentModeKHR* presentModes, size_t presentModeCount);
        VkExtent2D ChooseSwapExtent(VkSurfaceCapabilitiesKHR& capabilities);
        VkFormat FindDepthFormat();
        
        VulkanDevice& device;
        VkExtent2D windowExtent;

        VkFramebuffer* swapChainFrameBuffers;
        VkRenderPass renderPass;

        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

        // Depth image data.
        // NOTE: If these values do end up being changed dynamically, it might be a good idea to
        // wrap them in a vector.
        VkImage* depthImages;
        VkDeviceMemory* depthImageMemorys;
        VkImageView* depthImageViews;

        VkImage* swapChainImages;
        VkImageView* swapChainImageViews;
        u32 imageCount;

        VkSwapchainKHR swapChain;
    };
}