#include "Window/Window.h"
#include "Renderer/Device/VulkanDevice.h"
#include "Renderer/Pipeline/Pipeline.h"
#include "Renderer/Swapchain/Swapchain.h"
#include "Renderer/Renderer.h"

#if (defined(_WIN32) || defined(_WIN64)) && defined(DEBUG)
#include <windows.h>

#define WINDOWS_ATTACH_CONSOLE\
    AttachConsole(ATTACH_PARENT_PROCESS);\
    freopen("CON", "w", stdout);\
    freopen("CON", "w", stderr);\
    freopen("CON", "r", stdin);
#else
    #define WINDOWS_ATTACH_CONSOLE
#endif

static const constexpr int WIDTH = 800;
static const constexpr int HEIGHT = 600;

int main() 
{
    WINDOWS_ATTACH_CONSOLE

    // Window initialisation
    SnekVk::Window window("Snek", WIDTH, HEIGHT);

    SnekVk::Renderer renderer(window);

    // Graphics pipeline creation
    // TODO: Edit this to allow pipelines to be configured and passed into renderer.
    VkPipelineLayout pipelineLayout;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; 
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    SNEK_ASSERT(vkCreatePipelineLayout(renderer.GetDevice().Device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) == VK_SUCCESS, 
        "Failed to create pipeline layout!");

    auto pipelineConfig = SnekVk::Pipeline::DefaultPipelineConfig(renderer.GetSwapChain().GetWidth(), renderer.GetSwapChain().GetHeight());
    pipelineConfig.renderPass = renderer.GetSwapChain().GetRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;

    SnekVk::Pipeline pipeline(renderer.GetDevice(), "bin/shaders/simpleShader.vert.spv", "bin/shaders/simpleShader.frag.spv", pipelineConfig);

    renderer.CreateCommandBuffers(pipeline);
    
    // Main loop
    while(!window.WindowShouldClose()) {
        window.Update();
        renderer.DrawFrame();
    }

    vkDeviceWaitIdle(renderer.GetDevice().Device());

    vkDestroyPipelineLayout(renderer.GetDevice().Device(), pipelineLayout, nullptr);

    return 0;
}
