//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <render/renderer/Renderer.h>
#include <render/renderer/model/Model.h>
#include <render/renderer/platform/vulkan/Material.h>
#include <render/renderer/platform/vulkan/Shader.h>
#include <render/renderer/platform/vulkan/Texture2D.h>
#include <utils/math/Float.h>
#include <window/Input.h>
#include <window/Window.h>

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

    Siege::Window window("Render Example", {WIDTH, HEIGHT});

    window.DisableCursor();

    Siege::Input::SetInputWindowSource(reinterpret_cast<GLFWwindow*>(window.GetRawWindow()));

    Siege::Renderer renderer(window);

    Camera camera;
    Camera camera2;

    // Load textures
    auto aryehthulu = Siege::Vulkan::Texture2D("Aryehthulu", "assets/textures/aryehthulu.jpg");
    auto cappy = Siege::Vulkan::Texture2D("Cthulhu", "assets/textures/cappy.png");

    auto meslo = Siege::Vulkan::Font("assets/fonts/meslo-lg/MesloLGS-Regular.ttf");
    auto pixel = Siege::Vulkan::Font("assets/fonts/PublicPixel.ttf");

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
    camera2.SetPosition({0.f, -1.f, -2.5f});

    auto currentTime = std::chrono::high_resolution_clock::now();

    bool inputEnabled = true;
    bool isPanelOpen {false};
    bool isPanelJustOpened {false};

    size_t panelInputCharCount = 1;

    const size_t PANEL_INPUT_CHAR_MAX = 15;
    Siege::String panelInput = "_";

    while (!window.WindowShouldClose())
    {
        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime =
            std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime)
                .count();
        currentTime = newTime;

        auto time = std::time(nullptr);
        auto alpha = std::clamp<float>(abs(sin(time)), 0.001f, 1.f);
        auto blink = std::clamp<float>(abs(sin(time * 2.f)), 0.001f, 1.f);

        window.Update();

        if (Siege::Input::IsKeyJustPressed(Siege::Key::KEY_ESCAPE))
        {
            inputEnabled = !inputEnabled;
            window.ToggleCursor(inputEnabled);
        }

        float aspect = renderer.GetAspectRatio();

        float panelWidth = window.GetWidth();
        float panelHeight = 50.f;

        camera.UpdatePerspectiveProjection(Siege::Float::Radians(50.f), aspect, 0.1f, 100.f);

        camera2
            .UpdateOrthographicProjection(0, window.GetWidth(), 0, window.GetHeight(), 0.1f, 100.f);

        if (inputEnabled && !isPanelOpen)
        {
            camera.MoveCamera(frameTime);
        }

        renderer.SetProjection(camera.GetProjection(), camera.GetView());
        renderer.SetUiProjection(camera2.GetProjection(), camera2.GetView());

        if (!renderer.StartFrame()) continue;

        camera.Update();
        camera2.Update();

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
                                          {255, 0, 0, (uint8_t) 255.f},
                                          {0, 0, 255, 5});

        Siege::Renderer3D::DrawBillboard({-1.f, -2.5f, 0.f}, {1.f, 1.f}, {255, 255, 255, 255});
        Siege::Renderer3D::DrawBillboard({1.f, -2.5f, 0.f}, {1.f, 1.f}, {255, 0, 0, 255});

        Siege::Renderer3D::DrawLine({0.f, -1.f, -1.5f}, {0.f, -1.f, 0.f}, {255, 255, 255, 255});

        Siege::Renderer3D::DrawQuad({-1.95f, -1.5f, 2.95f},
                                    {1.f, .5f},
                                    Siege::Vec3::Zero,
                                    Siege::IColour::White,
                                    &aryehthulu);
        Siege::Renderer3D::DrawQuad({0.f, -1.5f, 2.95f},
                                    {.5f, .5f},
                                    Siege::Vec3::Zero,
                                    Siege::IColour::Green);
        Siege::Renderer3D::DrawQuad({2.f, -1.5f, 2.95f},
                                    {.5f, .5f},
                                    Siege::Vec3::Zero,
                                    Siege::IColour::White,
                                    &cappy);

        Siege::Renderer3D::DrawText3D("Random Vase",
                                      {0.f, -.85f, -.5175f},
                                      {},
                                      {.075f, .075f},
                                      Siege::IColour::White,
                                      &pixel);
        Siege::Renderer3D::DrawText3D("AryehThulu",
                                      {-1.95f, -.8f, 2.95f},
                                      {},
                                      {.15f, .15f},
                                      Siege::IColour::Green,
                                      &meslo);
        Siege::Renderer3D::DrawText3D("Cappy",
                                      {2.f, -.75f, 2.95f},
                                      {},
                                      {.2f, .2f},
                                      Siege::IColour::Blue,
                                      &pixel);
        Siege::Renderer3D::DrawText3D("Default Texture",
                                      {0.f, -.8f, 2.95f},
                                      {},
                                      {.1f, .1f},
                                      Siege::IColour::Red,
                                      &pixel);
        Siege::Renderer3D::DrawText3D("Wow, isn't it great that we can finally render text?",
                                      {2.98f, -3.f, 0.f},
                                      {0.f, 1.5707963268f, 0.f},
                                      {.175f, .175f},
                                      Siege::IColour::Green,
                                      &meslo);
        Siege::Renderer3D::DrawText3D("It only took you a few months...",
                                      {2.98f, -2.4f, 0.f},
                                      {0.f, 1.5707963268f, 0.1f},
                                      {.175f, .175f},
                                      Siege::IColour::Red,
                                      &pixel);
        Siege::Renderer3D::DrawText3D("Yeah but it was like... hard",
                                      {2.98f, -2.f, 0.f},
                                      {0.f, 1.5707963268f, 0.f},
                                      {.175f, .175f},
                                      Siege::IColour::Green,
                                      &meslo);
        Siege::Renderer3D::DrawText3D("I had to learn instanced rendering",
                                      {2.98f, -1.5f, 0.f},
                                      {0.f, 1.5707963268f, 0.f},
                                      {.175f, .175f},
                                      Siege::IColour::Green,
                                      &meslo);
        Siege::Renderer3D::DrawText3D("Which, btw, is not that",
                                      {2.98f, -1.f, 0.f},
                                      {0.f, 1.5707963268f, -0.1f},
                                      {.175f, .175f},
                                      Siege::IColour::Green,
                                      &meslo);
        Siege::Renderer3D::DrawText3D("Easy",
                                      {2.98f, -1.2f, -1.7f},
                                      {0.f, 1.5707963268f, -0.1f},
                                      {.175f, .175f},
                                      Siege::IColour::Blue,
                                      &pixel);
        Siege::Renderer3D::DrawText3D("But hey, at least it's done, right?",
                                      {2.98f, -.5f, 0.f},
                                      {0.f, 1.5707963268f, -0.1f},
                                      {.17f, .17f},
                                      Siege::IColour::White,
                                      &pixel);

        if (Siege::Input::IsKeyJustPressed(Siege::Key::KEY_GRAVE_ACCENT))
        {
            isPanelOpen = !isPanelOpen;
            isPanelJustOpened = isPanelOpen;
        }

        if (isPanelOpen)
        {
            renderer.DrawQuad({0.f, -1.f},
                              {panelWidth / 2, panelHeight},
                              Siege::IColour::Black,
                              0,
                              1);

            renderer.DrawText2D(panelInput.Str(),
                                pixel,
                                {25.f, 25.f},
                                {50.f, 50.f},
                                0.f,
                                Siege::IColour::White,
                                0);

            renderer.DrawGrid2D(100.f, {.2f, .2f, .2f}, window.GetDPI());

            int latestChar = Siege::Input::GetLatestChar();

            if (latestChar != -1 && panelInputCharCount < PANEL_INPUT_CHAR_MAX &&
                !isPanelJustOpened)
            {
                panelInput[panelInputCharCount - 1] = static_cast<char>(latestChar);
                panelInput[panelInputCharCount] += '_';
                panelInputCharCount++;
            }

            if (Siege::Input::GetLatestKey() == Siege::KEY_BACKSPACE && panelInputCharCount > 1)
            {
                panelInput[panelInputCharCount - 1] = '\0';
                panelInput[panelInputCharCount - 2] = '_';

                panelInputCharCount--;
            }

            isPanelJustOpened = false;
        }

        renderer.DrawQuad({0, window.GetHeight() - 100.f},
                          {50, 50},
                          Siege::IColour::White,
                          0,
                          0,
                          &cappy);
        renderer.DrawQuad({window.GetWidth() - 200.f, window.GetHeight() - 100.f},
                          {100, 50},
                          Siege::IColour::White,
                          0,
                          0,
                          &aryehthulu);

        renderer.EndFrame();
    }

    renderer.ClearDeviceQueue();

    return 0;
}
