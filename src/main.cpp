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
    }
    
    while(!window.WindowShouldClose()) {

        window.Update();

        if (renderer.StartFrame()) 
        {
            for (int i = 0; i < 4; i++)
            {
                auto& triangle = triangles[i];
                auto model = triangle.GetModel();
                
                float sine = glm::sin(glfwGetTime());
                float position = static_cast<float>(i);

                triangle.SetTransform({-0.75f + ((position / 4) + (position / 4)), sine / 4});
                triangle.SetColor({0.0f, 0.0f, 0.2f + 0.2f * i});
                triangle.SetScale({sine, sine});
                triangle.SetRotation(
                    glm::mod(triangle.GetTransform().rotation + 0.01f, glm::two_pi<float>()));

                renderer.DrawModel(model);
            }

            renderer.EndFrame();
        }
    }

    renderer.ClearDeviceQueue();

    return 0;
}
