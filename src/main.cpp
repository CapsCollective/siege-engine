#include <vector>
#include <Vector3.hpp>
#include <Window.hpp>
#include <Color.hpp>
#include <Camera3D.hpp>
#include "entities/Entity.h"
#include "entities/Cube.h"
#include "entities/PlayerController.h"

int main()
{
    // Initialise window
    int screenWidth = 800;
    int screenHeight = 450;
    raylib::Color bg = RAYWHITE;
    raylib::Window w = raylib::Window(screenWidth, screenHeight, "Raylib C++ Starter Kit Example");
    w.SetTargetFPS(60);

    // Define entity container
    std::vector<Entity*> entities;
    raylib::Camera3D camera;

    // Instantiate world objects
    auto cube = new Cube();
    entities.push_back(cube);
    auto playerController = new PlayerController();
    playerController->setPawn(cube);
    entities.push_back(playerController);
    camera = raylib::Camera3D(
            raylib::Vector3(0.f, 10.f, 10.f),
            raylib::Vector3::Zero(),
            raylib::Vector3(0.f, 1.f, 0.f),
            45.f,
            CAMERA_PERSPECTIVE
    );

    // Run main game loop until close button or ESC key
    while (!w.ShouldClose())
    {
        // Update entities
        for (auto& entity : entities)
        {
            entity->onUpdate();
        }
        camera.Update();

        // Begin drawing to screen
        w.BeginDrawing();
        bg.ClearBackground();
        camera.BeginMode3D();

        // Draw entities
        for (auto& entity : entities)
        {
            entity->onDraw();
        }
        DrawGrid(10, 1.0f);

        camera.EndMode3D();

        // Draw UI
        DrawText("Move with the arrow keys", 10.f, 40.f, 20.f, DARKGRAY);
        DrawFPS(10.f, 10.f);

        w.EndDrawing();
    }

    return 0;
}