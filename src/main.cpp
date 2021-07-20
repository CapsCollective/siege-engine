#include "Window/Window.h"
#include "Renderer/Device/VulkanDevice.h"
#include "Renderer/Pipeline/Pipeline.h"

#if (defined(_WIN32) || defined(_WIN64)) && defined(DEBUG)
#include <windows.h>
#endif

int main() 
{
    #if (defined(_WIN32) || defined(_WIN64)) && defined(DEBUG) 
    AllocConsole();
    freopen("CON", "w", stdout);
    freopen("CON", "w", stderr);
    freopen("CON", "r", stdin);
    #endif

    // Window initialisation
    SnekVk::Window window("Snek", 800, 600);

    SnekVk::VulkanDevice device(window);

    SnekVk::Pipeline pipeline("shaders/simpleShader.vert.spv", "shaders/simpleShader.frag.spv");

    // Main loop
    while(!window.WindowShouldClose()) {
        window.Update();
    }

    // Cleanup
    SnekVk::VulkanDevice::DestroyVulkanDevice(device);
    window.DestroyWindow();
    return 0;
}