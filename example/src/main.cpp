#include <entity/EntityStorage.h>
#include <physics/CollisionSystem.h>
#include <render/Camera.h>
#include <render/RenderSystem.h>
#include <render/Window.h>
#include <resource/ResourceManagerFacade.h>
#include <scene/SceneManager.h>

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
    EntityStorage::Add({&display, &devConsole}, true);

    // Set the default directories
    ResourceManagerFacade::SetBaseDirectory("example/assets/");

    // Instantiate world objects as per mode options
    if (isEditorMode)
    {
        // Start the editor controller
        auto editor = new EditorController();
        ServiceLocator::Provide(editor);

        // Batch register the editor and freeCam
        EntityStorage::Add({editor, new FreeCam()}, true);
    }
    else
    {
        // Load the game entrypoint
        SceneManager::QueueNextScene("scenes/main");
    }

    // Run main game loop until close button or ESC key
    while (!window.ShouldClose())
    {
        // Update timestep
        Window::UpdateTime();

        // Update game entities
        if (!isEditorMode)
        {
            for (auto& entity : EntityStorage::GetEntities()) entity->OnUpdate();
        }

        // Update tool entities
        for (auto& entity : EntityStorage::GetTools()) entity->OnUpdate();

        // Entity creation is deferred until after the update loop
        EntityStorage::RegisterEntities();
        CollisionSystem::RegisterEntities();

        // Begin drawing to screen
        window.BeginDraw();
        camera.Begin3D();

        // Draw entities
        RenderSystem::DrawFrame();

        camera.End3D();

        // UI Draw entities
        for (auto& entity : EntityStorage::GetTools()) entity->OnDraw2D();

        window.EndDraw();

        // Remove all entities at the end of the frame
        ResourceManagerFacade::FreeResources();
        CollisionSystem::FreeEntities();
        EntityStorage::FreeEntities();
        SceneManager::LoadNextScene();
    }

    return 0;
}
