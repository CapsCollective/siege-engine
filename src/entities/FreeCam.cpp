//
// Created by Jonathan Moallem on 14/12/20.
//

#include "FreeCam.h"

void FreeCam::OnUpdate()
{
    if (!camera) return;

    // Get movement as vector
    raylib::Vector3 move = {
            -(float)IsKeyDown(KEY_A) + (float)IsKeyDown(KEY_D),
            -(float)IsKeyDown(KEY_Q) + (float)IsKeyDown(KEY_E),
            -(float)IsKeyDown(KEY_W) + (float)IsKeyDown(KEY_S),
    };

    // Update the camera object in raylib
    camera->Update();

    // Set the new position of the camera
    raylib::Vector3 initialPosition = camera->GetPosition();
    camera->SetPosition(initialPosition + move.Normalize() * speed * GetFrameTime());
//    initialPosition = camera->GetPosition();
//    camera->SetTarget(raylib::Vector3(0.f, 0.f, 0.f));
}
