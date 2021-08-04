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

int main() 
{
    WINDOWS_ATTACH_CONSOLE

    SnekVk::Window window("Snek", WIDTH, HEIGHT);

    SnekVk::Renderer renderer(window);

    renderer.SetClearValue(.1f, .1f, .1f, 1.f);

    SnekVk::Model models[] = {
        SnekVk::Model(renderer.GetDevice(), triangleVerts, 3),
        SnekVk::Model(renderer.GetDevice(), triangleVerts, 3),
        SnekVk::Model(renderer.GetDevice(), triangleVerts, 3),
        SnekVk::Model(renderer.GetDevice(), triangleVerts, 3)
    };

    for (size_t i = 0; i < 4; i++) renderer.SubmitModel(&models[i]);
    
    while(!window.WindowShouldClose()) {
        static int frame = 0;
        frame = (frame + 1) % 200;

        window.Update();
        renderer.ClearDeviceQueue();

        for (int i = 0; i < 4; i++)
        {
            models[i].SetPushConstant(
                {{-0.5f + frame * 0.01f, -0.4f + i * 0.25f}, 
                {0.0f, 0.0f, 0.2f + 0.2f * i}}
            );
        }

        renderer.DrawFrame();
    }

    renderer.ClearDeviceQueue();

    return 0;
}
