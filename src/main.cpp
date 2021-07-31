#include "Window/Window.h"
#include "Renderer/Device/VulkanDevice.h"
#include "Renderer/Pipeline/Pipeline.h"
#include "Renderer/Swapchain/Swapchain.h"
#include "Renderer/Renderer.h"

#if (defined(_WIN32) || defined(_WIN64)) && defined(DEBUG)
#include <windows.h>
#endif

static const constexpr int WIDTH = 800;
static const constexpr int HEIGHT = 600;

int main() 
{
    #if (defined(_WIN32) || defined(_WIN64)) && defined(DEBUG) 
    AttachConsole(ATTACH_PARENT_PROCESS);
    freopen("CON", "w", stdout);
    freopen("CON", "w", stderr);
    freopen("CON", "r", stdin);
    #endif

    // Window initialisation
    SnekVk::Window window("Snek", WIDTH, HEIGHT);

    SnekVk::VulkanDevice device(window);

    SnekVk::SwapChain swapChain(device, window.GetExtent());

    // Graphics pipeline creation
    // TODO: Edit this to allow pipelines to be configured and passed into renderer.
    VkPipelineLayout pipelineLayout;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; 
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    SNEK_ASSERT(vkCreatePipelineLayout(device.Device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) == VK_SUCCESS, 
        "Failed to create pipeline layout!");

    auto pipelineConfig = SnekVk::Pipeline::DefaultPipelineConfig(swapChain.GetWidth(), swapChain.GetHeight());
    pipelineConfig.renderPass = swapChain.GetRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;

    SnekVk::Pipeline pipeline(device, "bin/shaders/simpleShader.vert.spv", "bin/shaders/simpleShader.frag.spv", pipelineConfig);

    SnekVk::Renderer renderer(&swapChain);

    renderer.CreateCommandBuffers(device, pipeline);
    
    // Main loop
    while(!window.WindowShouldClose()) {
        window.Update();
        renderer.DrawFrame();
    }

    vkDestroyPipelineLayout(device.Device(), pipelineLayout, nullptr);
    vkDeviceWaitIdle(device.Device());

    return 0;
}
