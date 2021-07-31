#include "Window/Window.h"
#include "Renderer/Device/VulkanDevice.h"
#include "Renderer/Pipeline/Pipeline.h"

#if (defined(_WIN32) || defined(_WIN64)) && defined(DEBUG)
#include <windows.h>
#endif

static const constexpr int WIDTH = 800;
static const constexpr int HEIGHT = 600;

int main() 
{
    #if (defined(_WIN32) || defined(_WIN64)) && defined(DEBUG) 
    AllocConsole();
    freopen("CON", "w", stdout);
    freopen("CON", "w", stderr);
    freopen("CON", "r", stdin);
    #endif

    // Window initialisation
    SnekVk::Window window("Snek", WIDTH, HEIGHT);

    SnekVk::VulkanDevice device(window);

    SnekVk::Pipeline pipeline(device, "shaders/simpleShader.vert.spv", "shaders/simpleShader.frag.spv", 
        SnekVk::Pipeline::DefaultPipelineConfig(WIDTH, HEIGHT));

    // Main loop
    while(!window.WindowShouldClose()) {
        window.Update();
    }
    return 0;
}
