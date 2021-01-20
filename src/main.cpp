#include "entities/tools/EditorController.h"
#include "entities/tools/MessageDisplay.h"
#include "systems/entity/EntityStorage.h"
#include "systems/resource/ResourceManager.h"
#include "entities/tools/DevConsole.h"
#include "entities/tools/Profiler.h"
#include "entities/tools/FreeCam.h"
#include "utils/ServiceLocator.h"
#include "systems/scene/SceneManager.h"
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

    // Initialise and register the message display
    auto display = new MessageDisplay();
    ServiceLocator::Provide(display);
    EntityStorage::Register(display, true);

    // Initialise and register the profiler
    auto profiler = new Profiler(isEditorMode);
    ServiceLocator::Provide(profiler);
    EntityStorage::Register(profiler, true);

    // Initialise and register the dev console
    EntityStorage::Register(new DevConsole(isEditorMode), true);

    // Instantiate world objects as per mode options
    if (isEditorMode)
    {
        // Start the editor controller
        auto editor = new EditorController();
        ServiceLocator::Provide(editor);
        EntityStorage::Register(editor, true);
        EntityStorage::Register(new FreeCam(), true);
    }
    else
    {
        // Start the game splash screen
        SceneManager::QueueNextScene("splash");
    }

    // Run main game loop until close button or ESC key
    while (!window.ShouldClose())
    {
        // Update tool entities
        for (auto& entity : EntityStorage::GetTools())
        {
            entity->OnUpdate();
        }

        // Update game entities
        if (!isEditorMode) {
            for (auto& entity : EntityStorage::GetEntities())
            {
                entity->OnUpdate();
            }
        }

        // Entity creation is deferred until after the update loop
        EntityStorage::RegisterEntities();

        // Begin drawing to screen
        window.BeginDrawing();
        bg.ClearBackground();
        camera.BeginMode3D();

        // Draw entities
        // TODO refactor to only receive Entity3Ds
        for (auto& entity : EntityStorage::GetAllEntities())
        {
            auto entity3D = dynamic_cast<Entity3D* const>(entity);
            if (!entity3D) continue;
            entity3D->OnDraw();
        }

        camera.EndMode3D();

        // UI Draw entities
        // TODO refactor to only receive Entity2Ds
        for (auto& entity : EntityStorage::GetAllEntities())
        {
            auto entity2D = dynamic_cast<Entity2D* const>(entity);
            if (!entity2D) continue;
            entity2D->OnDraw();
        }

        // Remove all entities at the end of the frame
        EntityStorage::FreeEntities();
        ResourceManager::FreeAllResources();
        SceneManager::LoadNextScene();

        window.EndDrawing();
    }

    return 0;
}
