#include "entities/tools/EditorController.h"
#include "entity_system/EntityStorage.h"
#include "entities/tools/FreeCam.h"
#include "utils/SceneLoader.h"
#include "entities/tools/DevConsole.h"
#include "utils/ServiceLocator.h"
#include "utils/SystemStatics.h"
#include <Camera3D.hpp>
#include <Vector3.hpp>
#include <Window.hpp>
#include <Color.hpp>

int main(int argc, char* argv[])
{
    // Check for editor flag
    bool isEditorMode = argc > 1 && std::string(argv[1]) == "--editor";
    SystemStatics::SetIsEditorMode(isEditorMode);

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
    MessageDisplay display = MessageDisplay();
    ServiceLocator::Provide(&display);
    EntityStorage::Register(&display);

    // Initialise and register the dev console
    DevConsole console = DevConsole();
    EntityStorage::Register(&console);

    // Instantiate world objects as per mode options
    if (isEditorMode)
    {
        auto editor = new EditorController();
        ServiceLocator::Provide(editor);
        EntityStorage::Register(editor);
        EntityStorage::Register(new FreeCam(&camera));
    }
    else
    {
        SceneLoader::LoadScene("main");
    }

    // Run main game loop until close button or ESC key
    while (!window.ShouldClose())
    {
        // Update entities
        for (auto& entity : EntityStorage::GetEntities())
        {
            // Run editor tool updates
            entity->OnToolUpdate();

            // Turn off standard updates in editor mode
            if (isEditorMode) continue;
            entity->OnUpdate();
        }

        // Entity creation is deferred until after the update loop
        EntityStorage::RegisterEntities();

        // Begin drawing to screen
        window.BeginDrawing();
        bg.ClearBackground();
        camera.BeginMode3D();

        // Draw entities
        for (auto& entity : EntityStorage::GetEntities())
        {
            entity->OnDraw();
        }

        camera.EndMode3D();

        // Begin drawing UI to screen
        DrawFPS(10.f, 10.f);

        // UI Draw entities
        for (auto& entity : EntityStorage::GetEntities())
        {
            entity->OnUIDraw();
        }

        EntityStorage::Instance()->FreeEntities();

        window.EndDrawing();
    }

    return 0;
}
