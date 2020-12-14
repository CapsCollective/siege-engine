//
// Created by Jonathan Moallem on 14/12/20.
//

#include "FreeCam.h"

void FreeCam::OnUpdate()
{
    if (!camera) return;

    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
    {
        // TODO some camera look code
    }

    // Get movement as vector
    raylib::Vector3 move = {
            -(float)IsKeyDown(KEY_A) + (float)IsKeyDown(KEY_D),
            -(float)IsKeyDown(KEY_Q) + (float)IsKeyDown(KEY_E),
            -(float)IsKeyDown(KEY_W) + (float)IsKeyDown(KEY_S),
    };

    // Calculate the rotation
    raylib::Vector3 rotation = raylib::Vector3(0.f, -1.f, -1.f);

    // Set the new position and look rotation of the camera
    camera->SetPosition(raylib::Vector3(camera->GetPosition()) + move.Normalize() * speed * GetFrameTime());
    camera->SetTarget(raylib::Vector3(camera->GetPosition()) + rotation);
}
