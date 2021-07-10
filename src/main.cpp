#include "engine/collision/CollisionSystem.h"
#include "engine/resource/ResourceManager.h"
#include "engine/entity/EntityStorage.h"
#include "engine/scene/SceneManager.h"

#include "entities/tools/EditorController.h"
#include "entities/tools/MessageDisplay.h"
#include "entities/tools/DevConsole.h"
#include "entities/tools/Profiler.h"
#include "entities/tools/FreeCam.h"

#include "ServiceLocator.h"

#include <Camera3D.hpp>
#include <Vector3.hpp>
#include <Window.hpp>
#include <Color.hpp>

int main(int argc, char* argv[])
{
    // Check for editor flag
    bool isEditorMode = argc > 1 && std::string(argv[1]) == "--editor";

    // Initialise window
    raylib::Color bg = RAYWHITE;
    raylib::Window window = raylib::Window(800, 450, "A Dark Discomfort");
    window.SetTargetFPS(60);

    // Deactivate the exit key
    SetExitKey(-1);

    // Create main camera
    raylib::Camera3D camera = raylib::Camera3D(
            raylib::Vector3(0.f, 10.f, 10.f),
            raylib::Vector3(0.f, 0.f, 0.f),
            raylib::Vector3(0.f, 1.f, 0.f),
            45.f,
            CAMERA_PERSPECTIVE
    );
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
        window.BeginDrawing();
        bg.ClearBackground();
        camera.BeginMode3D();

        // Draw entities
        for (auto& entity : EntityStorage::GetEntities()) entity->OnDraw();
        for (auto& entity : EntityStorage::GetTools()) entity->OnDraw();

        camera.EndMode3D();

        // UI Draw entities
        for (auto& entity : EntityStorage::GetEntities()) entity->OnDraw2D();
        for (auto& entity : EntityStorage::GetTools()) entity->OnDraw2D();

        window.EndDrawing();

        // Remove all entities at the end of the frame
        ResourceManager::FreeResources();
        CollisionSystem::FreeEntities();
        EntityStorage::FreeEntities();
        SceneManager::LoadNextScene();
    }

    return 0;
}
