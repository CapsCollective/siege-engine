#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <iostream>
#include "Window/Window.h"
#include "Renderer/VulkanDevice.h"

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
    SnekVk::Window window = SnekVk::Window("Snek", 800, 600);

    SnekVk::VulkanDevice device = SnekVk::VulkanDevice(window);

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::cout << extensionCount << " extensions supported\n";

    // Main loop
    while(!window.WindowShouldClose()) {
        window.Update();
    }

    // Cleanup
    window.DestroyWindow();
}