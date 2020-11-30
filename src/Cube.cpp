//
// Created by Jonathan Moallem on 30/11/20.
//

#include "Cube.h"
#include "iostream"

void Cube::onUpdate()
{
    std::cout << "updating" << std::endl;

    float charSpeed = 10.f;
    raylib::Vector3 velocity = raylib::Vector3::Zero();

    if (IsKeyDown(KEY_W))
    {
        velocity.z -= 1;
    }

    if (IsKeyDown(KEY_S))
    {
        velocity.z += 1;
    }

    if (IsKeyDown(KEY_A))
    {
        velocity.x -= 1;
    }

    if (IsKeyDown(KEY_D))
    {
        velocity.x += 1;
    }

    position += velocity.Normalize() * charSpeed * GetFrameTime();
}

void Cube::onDraw()
{
    DrawCube(position, 2.f, 2.f, 2.f, RED);
    DrawCubeWires(position, 2.f, 2.f, 2.f, MAROON);
}