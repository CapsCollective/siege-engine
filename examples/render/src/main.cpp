//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <render/input/Input.h>
#include <render/renderer/Renderer.h>
#include <render/renderer/material/Material.h>
#include <render/renderer/model/Model.h>
#include <render/renderer/shader/Shader.h>
#include <render/window/Window.h>
#include <utils/HeapArray.h>
#include <utils/math/Float.h>

#include <chrono>

#include "Camera.h"
#include "GameObject.h"

#if (defined(_WIN32) || defined(_WIN64)) && defined(DEBUG)
#include <windows.h>

#define WINDOWS_ATTACH_CONSOLE            \
    AttachConsole(ATTACH_PARENT_PROCESS); \
    freopen("CON", "w", stdout);          \
    freopen("CON", "w", stderr);          \
    freopen("CON", "r", stdin);
#else
#define WINDOWS_ATTACH_CONSOLE
#endif

static const constexpr int WIDTH = 800;
static const constexpr int HEIGHT = 600;

Siege::Vertex2D triangleVerts[] = {{{0.f, -1.f}, {1.f, 0.f, 0.f}},
                                   {{1.f, 1.f}, {0.f, 1.f, 0.f}},
                                   {{-1.f, 1.f}, {0.f, 0.f, 1.f}}};

Siege::Mesh::MeshData triangleMeshData {
    sizeof(Siege::Vertex2D),
    triangleVerts, // Vertex array
    3, // 3 vertices
    0, // no indices
    0 // no indices specified
};

Siege::Vertex2D squareVerts[] = {
    {{1.f, 1.f}, {1.f, 0.f, 0.f}}, // top right
    {{1.f, -1.f}, {1.f, 0.f, 0.f}}, // bottom right
    {{-1.f, -1.f}, {1.f, 0.f, 0.f}}, // bottom left
    {{-1.f, 1.f}, {1.f, 0.f, 0.f}}, // top left
};

uint32_t squareIndices[] = {0, 1, 3, 1, 2, 3};

Siege::Mesh::MeshData squareMeshData {sizeof(Siege::Vertex2D), squareVerts, 4, squareIndices, 6};

int main()
{
    WINDOWS_ATTACH_CONSOLE

    Siege::Window window("Render Example", WIDTH, HEIGHT);

    window.DisableCursor();

    Siege::Input::SetWindowPointer(&window);

    Siege::Renderer renderer(window);

    Camera camera;

    // Shader Declaration

    // Vertex shaders

    auto diffuseShader =
        Siege::Shader::BuildShader()
            .FromShader("assets/shaders/simpleShader.vert.spv")
            .WithStage(Siege::PipelineConfig::VERTEX)
            .WithVertexType(sizeof(Siege::Vertex))
            .WithVertexAttribute(offsetof(Siege::Vertex, position), Siege::VertexDescription::VEC3)
            .WithVertexAttribute(offsetof(Siege::Vertex, color), Siege::VertexDescription::VEC3)
            .WithVertexAttribute(offsetof(Siege::Vertex, normal), Siege::VertexDescription::VEC3)
            .WithVertexAttribute(offsetof(Siege::Vertex, uv), Siege::VertexDescription::VEC2)
            .WithStorage(0, "objectBuffer", sizeof(Siege::Model::Transform), 1000)
            .WithUniform(1, "globalData", sizeof(Siege::Renderer3D::GlobalData), 1);

    auto spriteShader =
        Siege::Shader::BuildShader()
            .FromShader("assets/shaders/simpleShader2D.vert.spv")
            .WithStage(Siege::PipelineConfig::VERTEX)
            .WithVertexType(sizeof(Siege::Vertex2D))
            .WithVertexAttribute(offsetof(Siege::Vertex2D, position),
                                 Siege::VertexDescription::VEC2)
            .WithVertexAttribute(offsetof(Siege::Vertex2D, color), Siege::VertexDescription::VEC3)
            .WithStorage(0, "objectBuffer", sizeof(Siege::Model::Transform2D), 1000)
            .WithUniform(1, "globalData", sizeof(Siege::Renderer2D::GlobalData));

    // Fragment shaders

    auto fragShader = Siege::Shader::BuildShader()
                          .FromShader("assets/shaders/simpleShader.frag.spv")
                          .WithStage(Siege::PipelineConfig::FRAGMENT);

    auto diffuseFragShader =
        Siege::Shader::BuildShader()
            .FromShader("assets/shaders/diffuseFragShader.frag.spv")
            .WithStage(Siege::PipelineConfig::FRAGMENT)
            .WithUniform(1,
                         "globalData",
                         sizeof(Siege::Renderer3D::GlobalData)); // TIL: bindings must be unique
                                                                 // across all available shaders

    // Material Declaration
    // vertex       // fragment
    Siege::Material diffuseMat(&diffuseShader, &diffuseFragShader); // 3D diffuse material
    Siege::Material spriteMat(&spriteShader, &fragShader); // 2D sprite material

    // shader

    Siege::Material::BuildMaterials({&diffuseMat, &spriteMat});

    // Generate models

    // Generating models from raw vertices

    Siege::Model triangleModel(triangleMeshData);
    Siege::Model squareModel(squareMeshData);

    // Generating models from .obj files

    Siege::Model cubeObjModel("assets/models/cube.obj");
    Siege::Model vaseObjModel("assets/models/smooth_vase.obj");

    // Set 2D sprite material
    triangleModel.SetMaterial(&spriteMat);
    squareModel.SetMaterial(&spriteMat);

    // Set 3D diffuse material
    cubeObjModel.SetMaterial(&diffuseMat);
    vaseObjModel.SetMaterial(&diffuseMat);

    // Create shapes for use
    Siege::HeapArray<GameObject> objects3D = {GameObject(&cubeObjModel),
                                              GameObject(&cubeObjModel),
                                              GameObject(&vaseObjModel)};

    // TODO(Aryeh): create a separate object for representing 2D shapes
    Siege::HeapArray<GameObject> objects2D = {GameObject(&triangleModel), GameObject(&squareModel)};

    objects3D[0].SetPosition({0.f, -.5f, 0.f});
    objects3D[0].SetScale({.5f, .5f, .5f});
    objects3D[0].SetColor({.5f, 0.f, 0.f});

    objects3D[1].SetPosition({0.f, 0.f, 0.f});
    objects3D[1].SetScale({3.f, 3.f, 0.01f});
    objects3D[1].SetColor({.5f, 0.f, 0.f});
    objects3D[1].SetRotationX(1.570796f);

    objects3D[2].SetPosition({0.f, -1.f, 0.f});
    objects3D[2].SetScale({2.f, 2.f, 2.f});
    objects3D[2].SetColor({.5f, 0.f, 0.f});

    objects2D[0].SetPosition2D({1.5f, -1.f});
    objects2D[0].SetScale2D({.5f, 0.5f});
    objects2D[0].SetZIndex(0.f);

    objects2D[1].SetPosition2D({-1.5f, -1.f});
    objects2D[1].SetScale2D({.5f, 0.5f});
    objects2D[1].SetZIndex(0.f);

    camera.SetPosition({0.f, -1.f, -2.5f});

    auto currentTime = std::chrono::high_resolution_clock::now();

    bool inputEnabled = true;

    while (!window.WindowShouldClose())
    {
        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime =
            std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime)
                .count();
        currentTime = newTime;

        auto alpha = std::clamp<float>(abs(sin(glfwGetTime())), 0.001f, 1.f);

        window.Update();

        if (Siege::Input::IsKeyJustPressed(KEY_ESCAPE))
        {
            inputEnabled = !inputEnabled;
            window.ToggleCursor(inputEnabled);
        }

        float aspect = renderer.GetAspectRatio();

        camera.UpdatePerspectiveProjection(Siege::Float::Radians(50.f), aspect, 0.1f, 100.f);

        if (inputEnabled)
        {
            camera.MoveCamera(frameTime);
        }

        renderer.SetProjection(camera.GetProjection(), camera.GetView());

        if (!renderer.StartFrame()) continue;

        camera.Update();

        for (auto it = objects3D.CreateIterator(); it; ++it)
        {
            GameObject obj = *it;
            Siege::Renderer3D::DrawModel(obj.GetModel(),
                                         obj.GetPosition(),
                                         obj.GetScale(),
                                         obj.GetRotation());
        }

        // TODO(Aryeh): This will eventually need to take in multiple lights.
        Siege::Renderer3D::DrawPointLight({0.0f, -1.f, -1.5f},
                                          0.05f,
                                          {1.f, 0.f, 0.f, alpha},
                                          {1.f, 1.f, 1.f, .02f});

        Siege::Renderer3D::DrawBillboard({-1.f, -2.5f, 0.f}, {1.f, 1.f}, {1.f, 1.f, 1.f, 1.f});
        Siege::Renderer3D::DrawBillboard({1.f, -2.5f, 0.f}, {1.f, 1.f}, {1.f, 0.f, 0.f, 1.f});

        Siege::Renderer3D::DrawLine({0.0f, -1.f, -1.5f}, {0.f, -1.f, 0.f}, {1.f, 1.f, 1.f, 1.f});

        for (auto it = objects2D.CreateIterator(); it; ++it)
        {
            GameObject obj = *it;
            Siege::Renderer2D::DrawModel(obj.GetModel(),
                                         obj.GetPosition2D(),
                                         obj.GetScale2D(),
                                         obj.GetRotation2D(),
                                         obj.GetZIndex());
        }

        renderer.EndFrame();
    }

    renderer.ClearDeviceQueue();

    return 0;
}
