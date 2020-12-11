#include "Cube.h"

void Cube::onUpdate()
{
    // Get move axes as vector
    raylib::Vector3 move = {
            -(float)IsKeyDown(KEY_LEFT) + (float)IsKeyDown(KEY_RIGHT),
            0.f,
            -(float)IsKeyDown(KEY_UP) + (float)IsKeyDown(KEY_DOWN),
    };

    // Normalise and apply move to velocity, update position
    position += velocity += move.Normalize() * speed * GetFrameTime();

    // Dampen velocity
    velocity = velocity * 0.9f;
}

void Cube::onDraw()
{
    // Draw cube to current position
    DrawCube(position, 2.f, 2.f, 2.f, RED);
    DrawCubeWires(position, 2.f, 2.f, 2.f, MAROON);
}
