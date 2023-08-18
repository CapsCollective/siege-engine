//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <core/ResourceSystem.h>
#include <core/Statics.h>
#include <core/Ticker.h>
#include <core/physics/CollisionSystem.h>
#include <core/scene/SceneSystem.h>
#include <render/renderer/Renderer.h>
#include <window/Input.h>
#include <window/Window.h>

#include "ServiceLocator.h"
#include "entities/FPSCamera.h"
#include "entities/OrthoCamera.h"
#include "tools/DevConsole.h"
#include "tools/EditorController.h"
#include "tools/FreeCam.h"
#include "tools/MessageDisplay.h"

int main(int argc, char* argv[])
{
    // Check for editor flag
    bool isEditorMode = argc > 1 && Siege::String(argv[1]) == "--editor";

    // Create a window and main camera
    Siege::Window window("Game Example", {800, 450});
    ServiceLocator::Provide(&window);

    Siege::Input::SetInputWindowSource(reinterpret_cast<GLFWwindow*>(window.GetRawWindow()));

    Siege::Renderer renderer(window);
    ServiceLocator::Provide(&renderer);

    FPSCamera camera3d = FPSCamera({0.f, -5.f, -10.f},
                                   {0.f, 0.f, 1.f},
                                   Siege::Float::Radians(60.f),
                                   window.GetWidth(),
                                   window.GetHeight());
    camera3d.LookAt(Siege::Vec3::Zero());
    OrthoCamera camera2d = OrthoCamera({0.f, -1.f, -2.5f}, {0.f, -1.f, 1.f});
    ServiceLocator::Provide(&camera3d);

    RenderResources resources;
    ServiceLocator::Provide(&resources);

    // Initialise the message display
    auto display = MessageDisplay();
    ServiceLocator::Provide(&display);

    // Initialise the dev console
    auto devConsole = DevConsole(isEditorMode);

    // Batch register all initialised tools (including the dev console)
    Siege::Statics::Tool().SetAllowDeregistration(false);
    Siege::Statics::Tool().Add({&display, &devConsole});

    // Set the default directories
    Siege::Statics::Resource().SetBaseDirectory("assets/");

    // Instantiate world objects as per mode options
    if (isEditorMode)
    {
        // Start the editor controller
        auto editor = new EditorController();
        ServiceLocator::Provide(editor);

        // Batch register the editor and freeCam
        Siege::Statics::Tool().Add({editor, new FreeCam()});
    }
    else
    {
        // Load the game entrypoint
        Siege::Statics::Scene().QueueNextScene("scenes/main");
    }

    // Create and initialise ticker
    Siege::Ticker ticker;
    ServiceLocator::Provide(&ticker);

    // Run main game loop until close button or ESC key
    while (!window.WindowShouldClose())
    {
        // Update time-step and window events
        ticker.UpdateTime();
        window.Update();

        // Update game entities
        if (!isEditorMode)
        {
            for (auto& entity : Siege::Statics::Entity().GetEntities()) entity->OnUpdate();
        }

        // Update tool entities
        for (auto& entity : Siege::Statics::Tool().GetEntities()) entity->OnUpdate();

        // Entity creation is deferred until after the update loop
        Siege::Statics::Tool().RegisterEntities();
        Siege::Statics::Entity().RegisterEntities();
        Siege::Statics::Collision().RegisterEntities();

        float aspect = window.AspectRatio();

        camera3d.SetAspectRatio(aspect);
        camera2d.Update(0.f, window.GetWidth(), 0.f, window.GetHeight(), 0.1f, 1000.f);

        renderer.SetCamera3D(camera3d.GetCamera());
        renderer.SetCamera2D(camera2d.GetCamera());

        // Begin drawing to screen
        if (!renderer.StartFrame()) continue;

        // Draw entities
        for (auto& entity : Siege::Statics::Tool().GetEntities()) entity->OnDraw3D();
        for (auto& entity : Siege::Statics::Entity().GetEntities()) entity->OnDraw3D();
        for (auto& entity : Siege::Statics::Tool().GetEntities()) entity->OnDraw2D();
        for (auto& entity : Siege::Statics::Entity().GetEntities()) entity->OnDraw2D();

        renderer.EndFrame();

        // Remove all entities at the end of the frame
        Siege::Statics::Resource().FreeResources();
        Siege::Statics::Collision().FreeEntities();
        Siege::Statics::Entity().FreeEntities();
        Siege::Statics::Tool().FreeEntities();
        Siege::Statics::Scene().LoadNextScene();
    }

    renderer.ClearDeviceQueue();

    return 0;
}
