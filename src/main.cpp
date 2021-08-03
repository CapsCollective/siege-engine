#define VOLK_IMPLEMENTATION

#include "Window/Window.h"
#include "Renderer/Renderer.h"
#include "Renderer/Model/Model.h"

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

SnekVk::Model::Vertex triangleVerts[] = {
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}, 
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

SnekVk::Model::Vertex triangleVerts2[] = {
    {{0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
    {{1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}}, 
    {{-1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}}
};

int main() 
{
    WINDOWS_ATTACH_CONSOLE

    SnekVk::Window window("Snek", WIDTH, HEIGHT);

    SnekVk::Renderer renderer(window);

    renderer.SetClearValue(.1f, .1f, .1f, 1.f);

    SnekVk::Model triangle(renderer.GetDevice(), triangleVerts, 3);
    SnekVk::Model triangle2(renderer.GetDevice(), triangleVerts2, 3);

    renderer.SubmitModel(&triangle);
    renderer.SubmitModel(&triangle2);
    
    while(!window.WindowShouldClose()) {
        window.Update();
        renderer.ClearDeviceQueue();
        renderer.DrawFrame();
    }

    renderer.ClearDeviceQueue();

    return 0;
}
