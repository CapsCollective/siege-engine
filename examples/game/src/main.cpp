//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <core/Statics.h>
#include <core/entity/EntitySystem.h>
#include <core/physics/CollisionSystem.h>
#include <core/render/Camera.h>
#include <core/render/RenderSystem.h>
#include <core/render/ResourceSystem.h>
#include <core/render/Window.h>
#include <core/scene/SceneSystem.h>

#include "ServiceLocator.h"
#include "tools/DevConsole.h"
#include "tools/EditorController.h"
#include "tools/FreeCam.h"
#include "tools/MessageDisplay.h"

int main(int argc, char* argv[])
{
    // Check for editor flag
    bool isEditorMode = argc > 1 && Siege::String(argv[1]) == "--editor";

    // Create a window and main camera
    Siege::Window window("Game Example", 800, 450);
    ServiceLocator::Provide(&window);

    Siege::Cam camera;
    ServiceLocator::Provide(&camera);

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

    // Run main game loop until close button or ESC key
    while (!window.ShouldClose())
    {
        // Update time-step
        Siege::Window::UpdateTime();

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

        // Begin drawing to screen
        window.BeginDraw();
        camera.Begin3D();

        // Draw entities
        Siege::Statics::Render().DrawFrame();

        camera.End3D();

        // UI Draw entities
        for (auto& entity : Siege::Statics::Tool().GetEntities()) entity->OnDraw2D();

        window.EndDraw();

        // Remove all entities at the end of the frame
        Siege::Statics::Resource().FreeResources();
        Siege::Statics::Collision().FreeEntities();
        Siege::Statics::Entity().FreeEntities();
        Siege::Statics::Tool().FreeEntities();
        Siege::Statics::Scene().LoadNextScene();
    }

    return 0;
}
