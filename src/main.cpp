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
    {{0.0f, -0.5f, 0.f}},
    {{0.5f, 0.5f, 0.f}}, 
    {{-0.5f, 0.5f, 0.f}}
};

SnekVk::Model::Vertex squareVerts[] = {
    {{-0.5f, -0.5f, 0.f}},
    {{0.5f, 0.5f, 0.f}},
    {{-0.5f, 0.5f, 0.f}},
    {{-0.5f, -0.5f, 0.f}},
    {{0.5f, -0.5f, 0.f}},
    {{0.5f, 0.5f, 0.f}}, 
};

SnekVk::Utils::Array<SnekVk::Model::Vertex> GenerateCubeVertices() {
  SnekVk::Utils::Array<SnekVk::Model::Vertex> vertices{
 
      // left face (white)
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
 
      // right face (yellow)
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
 
      // top face (orange, remember y axis points down)
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
 
      // bottom face (red)
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
 
      // nose face (blue)
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
 
      // tail face (green)
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
 
  };

  return vertices;
}

std::vector<SnekVk::Model::Vertex> GenerateCircleVertices(u32 numSides)
{
    std::vector<SnekVk::Model::Vertex> uniqueVertices {};
    for (size_t i = 0 ; i < numSides; i++)
    {
        float angle = i * glm::two_pi<float>() / numSides;
        uniqueVertices.push_back({{glm::cos(angle), glm::sin(angle), 0.f}});
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

    auto circleVertices = GenerateCircleVertices(64);

    auto cubeVertices = GenerateCubeVertices();

    std::cout << cubeVertices.Size() << std::endl;
    
    SnekVk::Window window("Snek", WIDTH, HEIGHT);

    SnekVk::Renderer renderer(window);

    // Generate models

    SnekVk::Model triangleModel(SnekVk::Renderer::GetDevice(), triangleVerts, 3);

    SnekVk::Model squareModel(SnekVk::Renderer::GetDevice(), squareVerts, 6);

    SnekVk::Model circleModel(SnekVk::Renderer::GetDevice(), circleVertices.data(), circleVertices.size());

    SnekVk::Model cubeModel(SnekVk::Renderer::GetDevice(), cubeVertices.Data(), cubeVertices.Size());

    // Create shapes for use

    std::vector<Components::Shape> shapes = 
    {
        Components::Shape(&cubeModel)
    };

    shapes[0].SetPosition({0.f, 0.f, 0.5f});
    shapes[0].SetScale({.5f, .5f, .5f});

    while(!window.WindowShouldClose()) {

        window.Update();

        if (renderer.StartFrame()) 
        {
            for (auto& shape : shapes)
            {
                shape.SetRotationY(glm::mod(shape.GetRotation().y + 0.01f, glm::two_pi<float>()));
                shape.SetRotationX(glm::mod(shape.GetRotation().x + 0.005f, glm::two_pi<float>()));

                renderer.DrawModel(shape.GetModel(), shape.GetTransform(), shape.GetColor());
            }
            renderer.EndFrame();
        }
    }

    renderer.ClearDeviceQueue();

    return 0;
}
