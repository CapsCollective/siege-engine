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

        private:
        
        void CreateSwapChain();

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(VkSurfaceFormatKHR* formats, size_t formatCount);
        VkPresentModeKHR ChoosePresentMode(VkPresentModeKHR* presentModes, size_t presentModeCount);
        VkExtent2D ChooseSwapExtent(VkSurfaceCapabilitiesKHR& capabilities);
        
        VulkanDevice& device;
        VkExtent2D windowExtent;

        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

        VkImage* swapChainImages;
        u32 imageCount;

        VkSwapchainKHR swapChain;
    };
}