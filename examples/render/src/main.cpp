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
#include <render/renderer/platform/vulkan/Mesh.h>
#include <render/renderer/platform/vulkan/Shader.h>
#include <render/renderer/platform/vulkan/Texture2D.h>
#include <render/window/Window.h>
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

int main()
{
    WINDOWS_ATTACH_CONSOLE

    Siege::Window window("Render Example", WIDTH, HEIGHT);

    window.DisableCursor();

    Siege::Input::SetWindowPointer(&window);

    Siege::Renderer renderer(window, "assets/fonts/PublicPixel.ttf");

    Camera camera;

    // Load textures
    auto aryehthulu = Siege::Vulkan::Texture2D("Aryehthulu", "assets/textures/aryehthulu.jpg");
    auto cappy = Siege::Vulkan::Texture2D("Cthulhu", "assets/textures/cappy.png");

    auto meslo = Siege::Vulkan::Font("assets/fonts/meslo-lg/MesloLGS-Regular.ttf");

    // Shader Declaration

    using Siege::Vulkan::Material;
    using Siege::Vulkan::Shader;
    using Siege::Vulkan::Texture2D;

    // Shader Declaration

    auto testMaterial =
        Material(Shader::Builder()
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

    // Generate models

    // Generating models from .obj files

    Siege::Model cubeObjModel("assets/models/cube.obj");
    Siege::Model vaseObjModel("assets/models/smooth_vase.obj");

    // Set 3D diffuse material
    cubeObjModel.SetMaterial(&testMaterial);
    vaseObjModel.SetMaterial(&testMaterial);

    // Create shapes for use
    Siege::MHArray<GameObject> objects3D = {GameObject(&cubeObjModel),
                                            GameObject(&cubeObjModel),
                                            GameObject(&cubeObjModel),
                                            GameObject(&cubeObjModel),
                                            GameObject(&vaseObjModel)};

    objects3D[0].SetPosition({0.f, -.5f, 0.f});
    objects3D[0].SetScale({.5f, .5f, .5f});
    objects3D[0].SetColour({128, 0, 0, 255});

    objects3D[1].SetPosition({0.f, 0.f, 0.f});
    objects3D[1].SetScale({3.f, 3.f, 0.1f});
    objects3D[1].SetColour({128, 0, 0, 255});
    objects3D[1].SetRotationX(1.570796f);

    objects3D[2].SetPosition({0.f, -2.f, 3.f});
    objects3D[2].SetScale({3.f, 2.f, 0.001f});
    objects3D[2].SetColour({128, 0, 0, 255});
    objects3D[2].SetRotationX(0.f);

    objects3D[3].SetPosition({3.f, -2.f, 0.f});
    objects3D[3].SetScale({3.f, 2.f, 0.001f});
    objects3D[3].SetColour({128, 0, 0, 255});
    objects3D[3].SetRotationY(1.5707963268f);

    objects3D[4].SetPosition({0.f, -1.f, 0.f});
    objects3D[4].SetScale({2.f, 2.f, 2.f});
    objects3D[4].SetColour({128, 0, 0, 255});

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

        Siege::Renderer2D::DrawQuad({-1.95f, -1.5f},
                                    {1.f, .5f},
                                    {255, 255, 255, 255},
                                    0.f,
                                    2.95f,
                                    &aryehthulu);
        Siege::Renderer2D::DrawQuad({0.f, -1.5f}, {.5f, .5f}, {0, 255, 0, 255}, 0.f, 2.95f);
        Siege::Renderer2D::DrawQuad({2.f, -1.5f},
                                    {.5f, .5f},
                                    {255, 255, 255, 255},
                                    0.f,
                                    2.95f,
                                    &cappy);

        Siege::Renderer3D::DrawText3D("Random Vase",
                                      {0.f, -.85f, -.51f},
                                      {},
                                      {.13f, .13f},
                                      Siege::IColour::White);
        Siege::Renderer3D::DrawText3D("AryehThulu",
                                      {-1.95f, -.75f, 2.95f},
                                      {},
                                      {.25f, .25f},
                                      Siege::IColour::Green,
                                      &meslo);
        Siege::Renderer3D::DrawText3D("Cappy",
                                      {2.f, -.75f, 2.95f},
                                      {},
                                      {.25f, .25f},
                                      Siege::IColour::Blue);
        Siege::Renderer3D::DrawText3D("Default Texture",
                                      {0.f, -.8f, 2.95f},
                                      {},
                                      {.1f, .1f},
                                      Siege::IColour::Red);

        Siege::Renderer3D::DrawText3D("Wow, isn't it great that we can finally render text?",
                                      {2.99f, -3.f, 0.f},
                                      {0.f, 1.5707963268f, 0.f},
                                      {.25f, .25f},
                                      Siege::IColour::Green,
                                      &meslo);
        Siege::Renderer3D::DrawText3D("It only took you a few months...",
                                      {2.99f, -2.4f, 0.f},
                                      {0.f, 1.5707963268f, 0.1f},
                                      {.25f, .25f},
                                      Siege::IColour::Red);
        Siege::Renderer3D::DrawText3D("Yeah but it was like... hard",
                                      {2.99f, -2.f, 0.f},
                                      {0.f, 1.5707963268f, 0.f},
                                      {.25f, .25f},
                                      Siege::IColour::Green,
                                      &meslo);
        Siege::Renderer3D::DrawText3D("I had to learn instanced rendering",
                                      {2.99f, -1.5f, 0.f},
                                      {0.f, 1.5707963268f, 0.f},
                                      {.25f, .25f},
                                      Siege::IColour::Green,
                                      &meslo);
        Siege::Renderer3D::DrawText3D("Which, btw, is not that",
                                      {2.99f, -1.f, 0.f},
                                      {0.f, 1.5707963268f, -0.1f},
                                      {.25f, .25f},
                                      Siege::IColour::Green,
                                      &meslo);
        Siege::Renderer3D::DrawText3D("Easy",
                                      {2.99f, -1.2f, -1.7f},
                                      {0.f, 1.5707963268f, -0.1f},
                                      {.25f, .25f},
                                      Siege::IColour::Blue);

        Siege::Renderer3D::DrawText3D("But hey, at least it's done, right?",
                                      {2.99f, -.5f, 0.f},
                                      {0.f, 1.5707963268f, -0.1f},
                                      {.25f, .25f},
                                      Siege::IColour::White);

        renderer.EndFrame();
    }

    renderer.ClearDeviceQueue();

    return 0;
}
