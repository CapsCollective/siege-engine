#include <collision/CollisionSystem.h>
#include <resource/ResourceManager.h>
#include <entity/EntityStorage.h>
#include <scene/SceneManager.h>

#include <render/RenderSystem.h>
#include <render/Camera.h>
#include <window/Window.h>

#include "tools/EditorController.h"
#include "tools/MessageDisplay.h"
#include "tools/DevConsole.h"
#include "tools/Profiler.h"
#include "tools/FreeCam.h"
#include "ServiceLocator.h"

int main(int argc, char* argv[])
{
    // Check for editor flag
    bool isEditorMode = argc > 1 && std::string(argv[1]) == "--editor";

    // Create a window and main camera
    Window window("A Dark Discomfort", 800, 450);
    Cam camera;
    ServiceLocator::Provide(&camera);

    // Initialise the message display
    auto display = MessageDisplay();
    ServiceLocator::Provide(&display);

    // Initialise the profiler
    auto profiler = Profiler(isEditorMode);
    ServiceLocator::Provide(&profiler);

    // Initialise the dev console
    auto devConsole = DevConsole(isEditorMode);

    // Batch register all initialised tools (including the dev console)
    EntityStorage::Add({
        &display,
        &profiler,
        &devConsole
    }, true);

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
        SceneManager::QueueNextScene("splash");
    }

    // Run main game loop until close button or ESC key
    while (!window.ShouldClose())
    {
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
        RenderSystem::DrawFrame2D();
        for (auto& entity : EntityStorage::GetTools()) entity->OnDraw2D();

        window.EndDraw();

        // Remove all entities at the end of the frame
        ResourceManager::FreeResources();
        CollisionSystem::FreeEntities();
        EntityStorage::FreeEntities();
        SceneManager::LoadNextScene();
    }

    return 0;
}
