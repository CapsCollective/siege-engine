#define VOLK_IMPLEMENTATION

#include "Window/Window.h"
#include "Renderer/Renderer.h"
#include "Renderer/Model/Model.h"
#include "Components/Shape.h"

#include <glm/gtc/constants.hpp>
#include <vector>

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
    {{0.0f, -0.5f}},
    {{0.5f, 0.5f}}, 
    {{-0.5f, 0.5f}}
};

SnekVk::Model::Vertex squareVerts[] = {
    {{-0.5f, -0.5f}},
    {{0.5f, 0.5f}},
    {{-0.5f, 0.5f}},
    {{-0.5f, -0.5f}},
    {{0.5f, -0.5f}},
    {{0.5f, 0.5f}}, 
};

std::vector<SnekVk::Model::Vertex> GenerateCircleVertices(u32 numSides)
{
    std::vector<SnekVk::Model::Vertex> uniqueVertices {};
    for (size_t i = 0 ; i < numSides; i++)
    {
        float angle = i * glm::two_pi<float>() / numSides;
        uniqueVertices.push_back({{glm::cos(angle), glm::sin(angle)}});
    }

    uniqueVertices.push_back({});
    std::vector<SnekVk::Model::Vertex> vertices {};
    for (size_t i = 0 ; i < numSides; i++)
    {
        vertices.push_back(uniqueVertices[i]);
        vertices.push_back(uniqueVertices[(i + 1) % numSides]);
        vertices.push_back(uniqueVertices[numSides]);
    }
    return vertices;
}

int main() 
{
    WINDOWS_ATTACH_CONSOLE
    
    SnekVk::Window window("Snek", WIDTH, HEIGHT);

    SnekVk::Renderer renderer(window);

    SnekVk::Model triangleModel(SnekVk::Renderer::GetDevice(), triangleVerts, 3);

    SnekVk::Model squareModel(SnekVk::Renderer::GetDevice(), squareVerts, 6);

    auto circleVertices = GenerateCircleVertices(64);

    SnekVk::Model circleModel(SnekVk::Renderer::GetDevice(), circleVertices.data(), circleVertices.size());

    Components::Shape shapes[4] = {
        Components::Shape(&triangleModel),
        Components::Shape(&squareModel),
        Components::Shape(&circleModel),
        Components::Shape(&squareModel)
    };
    
    while(!window.WindowShouldClose()) {

        window.Update();

        if (renderer.StartFrame()) 
        {
            for (int i = 0; i < 4; i++)
            {
                auto& shape = shapes[i];
                auto model = shape.GetModel();
                
                float sine = glm::sin(glfwGetTime());
                float position = static_cast<float>(i);

                shape.SetTransform({-0.75f + ((position / 4) + (position / 4)), sine / 4});
                shape.SetColor({0.0f, 0.0f, 0.2f + 0.2f * i});
                shape.SetScale({sine, sine});
                shape.SetRotation(
                    glm::mod(shape.GetTransform().rotation + 0.01f, glm::two_pi<float>()));

                renderer.DrawModel(model);
            }

            renderer.EndFrame();
        }
    }

    renderer.ClearDeviceQueue();

    return 0;
}
