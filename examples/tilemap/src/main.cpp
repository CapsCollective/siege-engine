//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <window/Window.h>
#include <render/renderer/Renderer.h>
#include <render/renderer/camera/Camera.h>
#include <utils/math/vec/Vec3.h>
#include <utils/math/Projection.h>
#include <utils/math/Transform.h>

static const constexpr int WIDTH = 800;
static const constexpr int HEIGHT = 600;

int main()
{
    Siege::Window window("Tilemap Example", {WIDTH, HEIGHT});

    Siege::Renderer renderer(window);

    auto pixel = Siege::Vulkan::Font("assets/fonts/PublicPixel.ttf");
    auto tilemap = Siege::Vulkan::Texture2D("tilemap", "assets/textures/tilemap.png");

    Siege::Camera camera;

    camera.projection = Siege::Orthographic(0.f, window.GetWidth(), window.GetHeight(), 0.f, 0.1f, 1000.f);
    camera.view = Siege::LookAt(Siege::Vec3::Zero(), {0.f, 0.f, 1.f});

    renderer.SetCamera2D(camera);

    while (!window.WindowShouldClose())
    {
        window.Update();

        if(!renderer.StartFrame()) continue;

        renderer.DrawGrid2D(100.f, {.2f, .2f, .2f}, window.GetDPI());

        renderer.DrawText2D("Hello World!", pixel, {10.f, 10.f}, {64.f, 64.f}, 0.f, Siege::IColour::Black);

        renderer.DrawQuad({400.f, 300.f}, {256.f, 256.f}, Siege::IColour::White, 0.f, 0, &tilemap);

        renderer.EndFrame();
    }

    return 0;
}