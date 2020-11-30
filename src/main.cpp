#include <raylib-cpp.hpp>
#include <vector>
#include "Entity.h"
#include "Cube.h"

int main() {
    
    // Initialization
    int screenWidth = 800;
    int screenHeight = 450;

    raylib::Color bg = RAYWHITE;
    raylib::Window w = raylib::Window(screenWidth, screenHeight, "Raylib C++ Starter Kit Example");

    raylib::Camera3D camera = raylib::Camera3D(
            raylib::Vector3(0.f, 10.f, 10.f),
            raylib::Vector3::Zero(),
            raylib::Vector3(0.f, 1.f, 0.f),
            45.f,
            CAMERA_PERSPECTIVE
    );

    w.SetTargetFPS(60);

    std::vector<Entity*> entities;
    entities.push_back(new Cube(raylib::Vector3::Zero()));

    // Main game loop
    while (!w.ShouldClose()) // Detect window close button or ESC key
    {
        // Update

        for (auto& entity : entities)
        {
            entity->onUpdate();
        }

        camera.Update();

        // Draw
        w.BeginDrawing();
            bg.ClearBackground();

            camera.BeginMode3D();

                for (auto& entity : entities)
                {
                    entity->onDraw();
                }

                DrawGrid(10, 1.0f);

            camera.EndMode3D();

            DrawText("Welcome to the 3rd dimension", 10.f, 40.f, 20.f, DARKGRAY);
            DrawFPS(10.f, 10.f);

        w.EndDrawing();
    }

    return 0;
}