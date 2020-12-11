#include <vector>
#include <Vector3.hpp>
#include <Window.hpp>
#include <Color.hpp>
#include <Camera3D.hpp>
#include "entities/Player.h"
#include "entities/Geometry.h"
#include "entities/EditorController.h"
#include "entity_system/EntityStorage.h"

int main()
{
    // Initialise window
    int screenWidth = 800;
    int screenHeight = 450;
    raylib::Color bg = RAYWHITE;
    raylib::Window window = raylib::Window(screenWidth, screenHeight, "Raylib C++ Starter Kit Example");
    window.SetTargetFPS(60);

    // Create camera
    raylib::Camera3D camera;
    camera = raylib::Camera3D(
            raylib::Vector3(0.f, 10.f, 10.f),
            raylib::Vector3::Zero(),
            raylib::Vector3(0.f, 1.f, 0.f),
            45.f,
            CAMERA_PERSPECTIVE
    );

    // Instantiate world objects
    EntityStorage::Instance()->Register(new Player());
    EntityStorage::Instance()->Register(new Geometry(
            raylib::Vector3::Zero(),
            raylib::Vector3(5.f, 0.1f, 5.f))
    );
    EntityStorage::Instance()->Register(new EditorController(&camera));

    // Run main game loop until close button or ESC key
    while (!window.ShouldClose())
    {
        // Update entities
        for (auto& entity : EntityStorage::GetEntities())
        {
            entity->OnUpdate();
        }
        camera.Update();

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
        DrawText("Move with the arrow keys", 10.f, 40.f, 20.f, DARKGRAY);

        // UI Draw entities
        for (auto& entity : EntityStorage::GetEntities())
        {
            entity->OnUIDraw();
        }

        window.EndDrawing();
    }

    return 0;
}
