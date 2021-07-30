#include "Window/Window.h"
#include "Renderer/Device/VulkanDevice.h"

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

    // Main loop
    while(!window.WindowShouldClose()) {
        window.Update();
    }
    return 0;
}