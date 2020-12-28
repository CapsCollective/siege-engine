#include <raylib-cpp.hpp>
#include "entities/Geometry.h"
#include "entities/tools/EditorController.h"
#include "entity_system/EntityStorage.h"
#include "entities/tools/FreeCam.h"
#include "utils/SceneLoader.h"
#include "entities/tools/DevConsole.h"

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

    // Instantiate world objects as per mode options
    auto console = new DevConsole(isEditorMode);
    EntityStorage::Register(console);
    if (isEditorMode)
    {
        EntityStorage::Register(new EditorController(&camera));
        EntityStorage::Register(new FreeCam(&camera));
    }
    else
    {
        SceneLoader::LoadScene("main");
    }

    raylib::Model model = raylib::Model("assets/models/cube/cube.obj");
    raylib::Texture2D texture = raylib::Texture2D("assets/models/cube/CubeTexture.png");

    model.materials[0].maps[MAP_DIFFUSE].texture = texture;

    // Run main game loop until close button or ESC key
    while (!window.ShouldClose())
    {
        // Update entities
        for (auto& entity : EntityStorage::GetEntities())
        {
            // Turn off updating if the console is active
            if (console->IsActive() && entity != console) continue;
            entity->OnUpdate();
        }

        // Entity creation is deferred until after the update loop
        EntityStorage::RegisterEntities();

        // TODO THIS IS A TEST METHOD - PLEASE REMOVE WHEN ENTITY REMOVAL IS FULLY OPERATIONAL.
        if (isEditorMode && IsKeyPressed(KEY_EQUAL)) {
            EntityStorage::Register(new Geometry(
                    raylib::Vector3::Zero(),
                    raylib::Vector3(5.f, 0.1f, 5.f)));
        }

        // Begin drawing to screen
        window.BeginDrawing();
        bg.ClearBackground();
        camera.BeginMode3D();

        // Draw entities
        for (auto& entity : EntityStorage::GetEntities())
        {
            entity->OnDraw();
        }

        DrawModel(model, raylib::Vector3(0.0f, 0.0f, 0.0f), 1.0f, RED);

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
