#include "Player.h"

void Player::OnUpdate()
{
    // Get move axes as vector
    raylib::Vector3 move = {
            (float)(-IsKeyDown(KEY_LEFT) + IsKeyDown(KEY_RIGHT)),
            0.f,
            (float)(-IsKeyDown(KEY_UP) + IsKeyDown(KEY_DOWN)),
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

void Player::OnUIDraw()
{
    DrawText("Move with the arrow keys", 10.f, GetScreenHeight() - 30, 20.f, DARKGRAY);
}

BoundingBox Player::GetBoundingBox()
{
    return BoundingBox{
            position - raylib::Vector3::One(),
            position + raylib::Vector3::One(),
    };
}
