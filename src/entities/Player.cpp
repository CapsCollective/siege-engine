#include "Player.h"
#include "../entity_system/EntityStorage.h"
#include <iostream>

void Player::OnUpdate()
{
    // Get move axes as vector
    raylib::Vector3 move = {
            -(float)IsKeyDown(KEY_LEFT) + (float)IsKeyDown(KEY_RIGHT),
            0.f,
            -(float)IsKeyDown(KEY_UP) + (float)IsKeyDown(KEY_DOWN),
    };

    // Normalise and apply move to velocity
    position += velocity += move.Normalize() * speed * GetFrameTime();

    // Dampen velocity
    velocity = velocity * 0.9f;
}

void Player::OnDraw()
{
    // Draw player to current position
    DrawCube(position, 2.f, 2.f, 2.f, RED);
    DrawCubeWires(position, 2.f, 2.f, 2.f, MAROON);
}

BoundingBox Player::GetBoundingBox()
{
    return BoundingBox{
            position - raylib::Vector3::One(),
            position + raylib::Vector3::One(),
    };
}

void Player::QueueFree() {

    std::cout << "Freeing the player!" << std::endl;
    EntityStorage::Instance()->QueueFree(this);
}
