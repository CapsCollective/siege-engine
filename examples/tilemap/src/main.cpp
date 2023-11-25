//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <render/renderer/Renderer.h>
#include <render/renderer/camera/Camera.h>
#include <render/renderer/platform/vulkan/TextureAtlas.h>
#include <utils/math/Projection.h>
#include <utils/math/Transform.h>
#include <utils/math/vec/Vec3.h>
#include <window/Window.h>

static const constexpr int WIDTH = 800;
static const constexpr int HEIGHT = 600;

int main()
{
    Siege::Window window("Tilemap Example", {WIDTH, HEIGHT});

    Siege::Renderer renderer(window);

    auto pixel = Siege::Vulkan::Font("assets/fonts/PublicPixel.ttf");
    auto tilemap = Siege::Vulkan::TextureAtlas("tilemap",
                                               "assets/textures/tilemap.png",
                                               {.5f, .5f},
                                               Siege::Vulkan::Utils::TEXTURE_FILTER_NEAREST);

    Siege::Camera camera;

    while (!window.WindowShouldClose())
    {
        window.Update();

        camera.projection =
            Siege::Orthographic(0.f, window.GetWidth(), window.GetHeight(), 0.f, 0.1f, 1000.f);
        camera.view = Siege::LookAt(Siege::Vec3::Zero(), Siege::Vec3::Forward());
        renderer.SetCamera2D(camera);

        if (!renderer.StartFrame()) continue;

        renderer.DrawGrid2D(100.f, {.2f, .2f, .2f}, window.GetDPI());

        renderer.DrawText2D("Full Texture",
                            pixel,
                            {window.GetWidth() / 3.8f, 20.f},
                            {32.f, 32.f},
                            0.f,
                            Siege::IColour::Black);

        renderer.DrawQuad({(window.GetWidth() / 2.f) - 64.f, 100.f},
                          {64.f, 64.f},
                          Siege::IColour::White,
                          0.f,
                          0,
                          &tilemap.GetTexture());

        renderer.DrawText2D("Texture 0",
                            pixel,
                            {(window.GetWidth() / 2.f) * 0.1f, 275.f},
                            {16.f, 16.f},
                            0.f,
                            Siege::IColour::Black);

        renderer.DrawText2D("Texture 1",
                            pixel,
                            {(window.GetWidth() / 2.f) * 0.6f, 275.f},
                            {16.f, 16.f},
                            0.f,
                            Siege::IColour::Black);

        renderer.DrawText2D("Texture 2",
                            pixel,
                            {(window.GetWidth() / 2.f) * 1.1f, 275.f},
                            {16.f, 16.f},
                            0.f,
                            Siege::IColour::Black);

        renderer.DrawText2D("Texture 3",
                            pixel,
                            {(window.GetWidth() / 2.f) * 1.6f, 275.f},
                            {16.f, 16.f},
                            0.f,
                            Siege::IColour::Black);

        renderer.DrawQuad({(window.GetWidth() / 2.f) * 0.1f, 325.f},
                          tilemap[0],
                          {64.f, 64.f},
                          Siege::IColour::White,
                          0.f,
                          0);
        renderer.DrawQuad({(window.GetWidth() / 2.f) * 0.6f, 325.f},
                          tilemap[1],
                          {64.f, 64.f},
                          Siege::IColour::White,
                          0.f,
                          0);
        renderer.DrawQuad({(window.GetWidth() / 2.f) * 1.1f, 325.f},
                          tilemap[2],
                          {64.f, 64.f},
                          Siege::IColour::White,
                          0.f,
                          0);
        renderer.DrawQuad({(window.GetWidth() / 2.f) * 1.6f, 325.f},
                          tilemap[3],
                          {64.f, 64.f},
                          Siege::IColour::White,
                          0.f,
                          0);

        renderer.EndFrame();
    }

    renderer.ClearQueues();

    return 0;
}