#include <vector>
#include <Vector3.hpp>
#include <Window.hpp>
#include <Color.hpp>
#include <Camera3D.hpp>
#include "entities/Player.h"
#include "entities/Geometry.h"
#include "entities/EditorController.h"
#include "entity_system/EntityStorage.h"
#include "entities/FreeCam.h"

int main(int argc, char* argv[])
{
    // Check for editor flag
    bool isEditorMode = argc > 1 && std::string(argv[1]) == "--editor";

    // Initialise window
    int screenWidth = 800;
    int screenHeight = 450;
    raylib::Color bg = RAYWHITE;
    raylib::Window window = raylib::Window(screenWidth, screenHeight, "A Dark Discomfort");
    window.SetTargetFPS(60);
    SetExitKey(-1);

    // Create main camera
    raylib::Camera3D camera = raylib::Camera3D(
            raylib::Vector3(0.f, 10.f, 10.f),
            raylib::Vector3(0.f, 0.f, 0.f),
            raylib::Vector3(0.f, 1.f, 0.f),
            45.f,
            CAMERA_PERSPECTIVE
    );

    // Instantiate world objects
    if (isEditorMode)
    {
        EntityStorage::Instance()->Register(new EditorController(&camera));
        EntityStorage::Instance()->Register(new FreeCam(&camera));
    }
    else
    {
        EntityStorage::Instance()->Register(new Player());
    }
    EntityStorage::Instance()->Register(new Geometry(
            raylib::Vector3::Zero(),
            raylib::Vector3(5.f, 0.1f, 5.f))
    );

    // Run main game loop until close button or ESC key
    while (!window.ShouldClose())
    {
        // Update entities
        for (auto& entity : EntityStorage::GetEntities())
        {
            entity->OnUpdate();
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

        DrawGrid(10, 1.0f);

        camera.EndMode3D();

        // Begin drawing UI to screen
        DrawFPS(10.f, 10.f);

        // UI Draw entities
        for (auto& entity : EntityStorage::GetEntities())
        {
            entity->OnUIDraw();
        }

        window.EndDrawing();
    }

    return 0;
}
