//
// Created by Jonathan Moallem on 9/12/20.
//

#include "PlayerController.h"

PlayerController::PlayerController()
{
    // Set default fields
    speed = 1.5;
    velocity = raylib::Vector3::Zero();
    pawn = nullptr;
}

void PlayerController::onUpdate()
{
    if (!pawn) { return; }

    // Get move axes as vector
    raylib::Vector3 move = {
            -(float)IsKeyDown(KEY_LEFT) + (float)IsKeyDown(KEY_RIGHT),
            0.f,
            -(float)IsKeyDown(KEY_UP) + (float)IsKeyDown(KEY_DOWN),
    };

    // Normalise and apply move to velocity
    velocity += move.Normalize() * speed * GetFrameTime();

    // Update pawn position based on velocity
    pawn->setPosition(pawn->getPosition() + velocity);

    // Dampen velocity
    velocity = velocity * 0.9f;
}

void PlayerController::onDraw()
{

}

void PlayerController::setPawn(Entity* entity)
{
    pawn = entity;
}
