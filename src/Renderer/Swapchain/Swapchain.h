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

        private:
        
        void CreateSwapChain();
        void CreateImageViews();
        void CreateRenderPass();

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(VkSurfaceFormatKHR* formats, size_t formatCount);
        VkPresentModeKHR ChoosePresentMode(VkPresentModeKHR* presentModes, size_t presentModeCount);
        VkExtent2D ChooseSwapExtent(VkSurfaceCapabilitiesKHR& capabilities);
        VkFormat FindDepthFormat();
        
        VulkanDevice& device;
        VkExtent2D windowExtent;

        VkRenderPass renderPass;

        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

        VkImage* swapChainImages;
        VkImageView* swapChainImageViews;
        u32 imageCount;

        VkSwapchainKHR swapChain;
    };
}