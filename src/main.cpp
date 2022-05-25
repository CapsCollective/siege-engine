#define VOLK_IMPLEMENTATION

#include "Window/Window.h"
#include "Renderer/Renderer.h"
#include "Renderer/Model/Model.h"
#include "Components/Shape.h"

#include <glm/gtc/constants.hpp>

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

    SnekVk::Model triangleModel(SnekVk::Renderer::GetDevice(), triangleVerts, 3);

    Components::Shape triangles[4];

    for (size_t i = 0; i < 4; i++) 
    {
        triangles[i] = Components::Shape(&triangleModel);
        renderer.SubmitModel(triangles[i].GetModel());
    }
    
    while(!window.WindowShouldClose()) {

        window.Update();
        renderer.ClearDeviceQueue();

        for (int i = 0; i < 4; i++)
        {
            float sine = glm::sin(glfwGetTime()) + 0.5f / 2;
            triangles[i].SetTransform({0.0f, sine});
            triangles[i].SetColor({0.0f, 0.0f, 0.2f + 0.2f * i});
            triangles[i].SetScale({sine, sine});
            triangles[i].SetRotation(
                glm::mod(triangles[i].GetTransform().rotation + 0.01f, glm::two_pi<float>()));
        }

        renderer.DrawFrame();
    }

    renderer.ClearDeviceQueue();

    return 0;
}
