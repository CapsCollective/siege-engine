#define VOLK_IMPLEMENTATION

#include "Window/Window.h"
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
    
    // Main loop
    while(!window.WindowShouldClose()) {
        window.Update();
        renderer.DrawFrame();
    }

    renderer.ClearDeviceQueue();

    return 0;
}
