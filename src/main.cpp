#include <raylib-cpp.hpp>
#include <vector>
#include "Entity.h"
#include "Cube.h"

// Globals
std::vector<Entity*> entities;
raylib::Camera3D camera;

void beginPlay()
{
    // Instantiate world objects
    camera = raylib::Camera3D(
            raylib::Vector3(0.f, 10.f, 10.f),
            raylib::Vector3::Zero(),
            raylib::Vector3(0.f, 1.f, 0.f),
            45.f,
            CAMERA_PERSPECTIVE
    );
    entities.push_back(new Cube(raylib::Vector3::Zero()));
}

void updateWorld()
{
    // Update entities
    for (auto& entity : entities)
    {
        entity->onUpdate();
    }
    camera.Update();
}

void renderWorld()
{
    // Draw entities
    for (auto& entity : entities)
    {
        entity->onDraw();
    }

    DrawGrid(10, 1.0f);
}

void renderUI()
{
    // Draw UI
    DrawText("Welcome to the 3rd dimension", 10.f, 40.f, 20.f, DARKGRAY);
    DrawFPS(10.f, 10.f);
}

int main()
{
    // Initialization
    int screenWidth = 800;
    int screenHeight = 450;

    raylib::Color bg = RAYWHITE;
    raylib::Window w = raylib::Window(screenWidth, screenHeight, "Raylib C++ Starter Kit Example");

    w.SetTargetFPS(60);

    beginPlay();

    // Main game loop
    while (!w.ShouldClose()) // Detect window close button or ESC key
    {
        // Update
        updateWorld();

        // Draw
        w.BeginDrawing();
        bg.ClearBackground();
        camera.BeginMode3D();
        renderWorld();
        camera.EndMode3D();
        renderUI();
        w.EndDrawing();
    }

    return 0;
}