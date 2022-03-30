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
    bool isEditorMode = argc > 1 && String(argv[1]) == "--editor";

    // Create a window and main camera
    Window window("A Dark Discomfort", 800, 450);
    ServiceLocator::Provide(&window);

    Cam camera;
    ServiceLocator::Provide(&camera);

    // Initialise the message display
    auto display = MessageDisplay();
    ServiceLocator::Provide(&display);

    // Initialise the dev console
    auto devConsole = DevConsole(isEditorMode);

    // Batch register all initialised tools (including the dev console)
    Statics::Tool().SetAllowDeregistration(false);
    Statics::Tool().Add({&display, &devConsole});

    // Set the default directories
    Statics::Resource().SetBaseDirectory("examples/game/assets/");

    // Instantiate world objects as per mode options
    if (isEditorMode)
    {
        // Start the editor controller
        auto editor = new EditorController();
        ServiceLocator::Provide(editor);

        // Batch register the editor and freeCam
        Statics::Tool().Add({editor, new FreeCam()});
    }
    else
    {
        // Load the game entrypoint
        Statics::Scene().QueueNextScene("scenes/main");
    }

    // Run main game loop until close button or ESC key
    while (!window.ShouldClose())
    {
        // Update time-step
        Window::UpdateTime();

        // Update game entities
        if (!isEditorMode)
        {
            for (auto& entity : Statics::Entity().GetEntities()) entity->OnUpdate();
        }

        // Update tool entities
        for (auto& entity : Statics::Tool().GetEntities()) entity->OnUpdate();

        // Entity creation is deferred until after the update loop
        Statics::Tool().RegisterEntities();
        Statics::Entity().RegisterEntities();
        Statics::Collision().RegisterEntities();

        // Begin drawing to screen
        window.BeginDraw();
        camera.Begin3D();

        // Draw entities
        Statics::Render().DrawFrame();

        camera.End3D();

        // UI Draw entities
        for (auto& entity : Statics::Tool().GetEntities()) entity->OnDraw2D();

        window.EndDraw();

        // Remove all entities at the end of the frame
        Statics::Resource().FreeResources();
        Statics::Collision().FreeEntities();
        Statics::Entity().FreeEntities();
        Statics::Tool().FreeEntities();
        Statics::Scene().LoadNextScene();
    }

    return 0;
}
