//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <render/input/Input.h>
#include <render/renderer/Renderer.h>
#include <render/renderer/model/Model.h>
#include <render/renderer/platform/vulkan/Material.h>
#include <render/renderer/platform/vulkan/Shader.h>
#include <render/renderer/platform/vulkan/Texture2D.h>
#include <render/window/Window.h>
#include <utils/math/Float.h>

#include <chrono>

#include "Camera.h"
#include "GameObject.h"
#include "utils/collections/HeapArray.h"

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

Siege::Vertex2D triangleVerts[] = {{{0.f, -1.f}, {1.f, 0.f, 0.f, 1.f}},
                                   {{1.f, 1.f}, {0.f, 1.f, 0.f, 1.f}},
                                   {{-1.f, 1.f}, {0.f, 0.f, 1.f, 1.f}}};

Siege::Mesh::MeshData triangleMeshData {
    sizeof(Siege::Vertex2D),
    triangleVerts, // Vertex array
    3, // 3 vertices
    0, // no indices
    0 // no indices specified
};

Siege::Vertex2D squareVerts[] = {
    {{1.f, 1.f}, {1.f, 0.f, 0.f, 1.f}}, // top right
    {{1.f, -1.f}, {1.f, 0.f, 0.f, 1.f}}, // bottom right
    {{-1.f, -1.f}, {1.f, 0.f, 0.f, 1.f}}, // bottom left
    {{-1.f, 1.f}, {1.f, 0.f, 0.f, 1.f}}, // top left
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

    auto aryehthulu = Siege::Vulkan::Texture2D("Aryehthulu", "assets/textures/aryehthulu.jpg");
    auto defaultTex = Siege::Vulkan::Texture2D("default");

    // Shader Declaration

    using Siege::Vulkan::Shader;
    using Siege::Vulkan::Material;

    auto texturedQuadMat = Material(Shader::Builder()
                                                .FromVertexShader("assets/shaders/texturedQuad.vert.spv")
                                                .WithVertexBinding(
                                                   Shader::VertexBinding()
                                                       .AddFloatVec2Attribute()
                                                       .AddFloatVec4Attribute()
                                                       .AddFloatVec2Attribute()
                                                       .AddU32Attribute())
                                                .WithTransform2DStorage(0, 1000)
                                                .WithGlobalData2DUniform()
                                                .Build(),
                                                Shader::Builder()
                                                .FromFragmentShader("assets/shaders/texturedQuad.frag.spv")
                                                .WithTexture("quadTexture", 0, 16)
                                                .WithDefaultTexture(&defaultTex)
                                                .Build());

    auto testMaterial =
        Siege::Vulkan::Material(Shader::Builder()
                                    .FromVertexShader("assets/shaders/simpleShader.vert.spv")
                                    .WithVertexBinding(Shader::VertexBinding()
                                                           .AddFloatVec3Attribute()
                                                           .AddFloatVec4Attribute()
                                                           .AddFloatVec3Attribute()
                                                           .AddFloatVec2Attribute())
                                    .WithTransform3DStorage(0, 1000)
                                    .WithGlobalData3DUniform()
                                    .Build(),
                                Shader::Builder()
                                    .FromFragmentShader("assets/shaders/diffuseFragShader.frag.spv")
                                    .WithGlobalData3DUniform()
                                    .Build());

    auto testMaterial2D = Siege::Vulkan::Material(
        Shader::Builder()
            .FromVertexShader("assets/shaders/simpleShader2D.vert.spv")
            .WithVertexBinding(
                Shader::VertexBinding().AddFloatVec2Attribute().AddFloatVec4Attribute())
            .WithTransform2DStorage(0, 1000)
            .WithGlobalData2DUniform()
            .Build(),
        Shader::Builder().FromFragmentShader("assets/shaders/simpleShader.frag.spv").Build());

    // Generate models

    // Generating models from raw vertices

    Siege::Model triangleModel(triangleMeshData);
    Siege::Model squareModel(squareMeshData);

    // Load textures

    // Generating models from .obj files

    Siege::Model cubeObjModel("assets/models/cube.obj");
    Siege::Model vaseObjModel("assets/models/smooth_vase.obj");

    // Set 2D sprite material
    triangleModel.SetMaterial(&testMaterial2D);
    squareModel.SetMaterial(&testMaterial2D);

    // Set 3D diffuse material
    cubeObjModel.SetMaterial(&testMaterial);
    vaseObjModel.SetMaterial(&testMaterial);

    // Create shapes for use
    Siege::Utils::MHArray<GameObject> objects3D = {GameObject(&cubeObjModel),
                                                   GameObject(&cubeObjModel),
                                                   GameObject(&vaseObjModel)};

    // TODO(Aryeh): create a separate object for representing 2D shapes
    Siege::Utils::MHArray<GameObject> objects2D = {GameObject(&triangleModel),
                                                   GameObject(&squareModel)};

    objects3D[0].SetPosition({0.f, -.5f, 0.f});
    objects3D[0].SetScale({.5f, .5f, .5f});
    objects3D[0].SetColour({128, 0, 0, 255});

    objects3D[1].SetPosition({0.f, 0.f, 0.f});
    objects3D[1].SetScale({3.f, 3.f, 0.1f});
    objects3D[1].SetColour({128, 0, 0, 255});
    objects3D[1].SetRotationX(1.570796f);

    objects3D[2].SetPosition({0.f, -1.f, 0.f});
    objects3D[2].SetScale({2.f, 2.f, 2.f});
    objects3D[2].SetColour({128, 0, 0, 255});

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

        for (auto it = objects3D.CreateFIterator(); it; ++it)
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
                                          {255, 0, 0, (uint8_t) (alpha * 255.f)},
                                          {0, 0, 255, 5});

        Siege::Renderer3D::DrawBillboard({-1.f, -2.5f, 0.f}, {1.f, 1.f}, {255, 255, 255, 255});
        Siege::Renderer3D::DrawBillboard({1.f, -2.5f, 0.f}, {1.f, 1.f}, {255, 0, 0, 255});

        Siege::Renderer3D::DrawLine({0.f, -1.f, -1.5f}, {0.f, -1.f, 0.f}, {255, 255, 255, 255});

        for (auto it = objects2D.CreateFIterator(); it; ++it)
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
