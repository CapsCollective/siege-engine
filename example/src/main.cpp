#include <physics/CollisionSystem.h>
#include <entity/EntityStorage.h>
#include <scene/SceneManager.h>

#include <render/RenderSystem.h>
#include <render/Camera.h>
#include <render/Window.h>
#include <resource/ResourceManagerFacade.h>

#include "tools/EditorController.h"
#include "tools/MessageDisplay.h"
#include "tools/DevConsole.h"
#include "tools/FreeCam.h"
#include "ServiceLocator.h"

int main(int argc, char* argv[])
{
    // Check for editor flag
    bool isEditorMode = argc > 1 && std::string(argv[1]) == "--editor";

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
    EntityStorage::Add({
        &display,
        &devConsole
    }, true);

    // Set the default directories
    SceneManager::SetBaseDirectory("example/assets/scenes/");
    ResourceManagerFacade::SetBaseDirectory("example/assets/");

    // Instantiate world objects as per mode options
    if (isEditorMode)
    {
        // Start the editor controller
        auto editor = new EditorController();
        ServiceLocator::Provide(editor);

        // Batch register the editor and freeCam
        EntityStorage::Add({
            editor,
            new FreeCam()
        }, true);
    }
    else
    {
        // Start the game splash screen
        SceneManager::QueueNextScene("main");
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
