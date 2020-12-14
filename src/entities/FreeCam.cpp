//
// Created by Jonathan Moallem on 14/12/20.
//

#include "FreeCam.h"

void FreeCam::OnUpdate()
{
    if (!camera) return;

    // Calculate halfed screen size values
    int screenWidthHalf = GetScreenWidth()/2;
    int screenHeightHalf = GetScreenHeight()/2;

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        // Begin the click, hide the cursor
        HideCursor();
        SetMousePosition(screenWidthHalf, screenHeightHalf);
    }
    else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
    {
        // Calculate and set the rotation while click held
        raylib::Vector3 rotate = {
                (float) (GetMouseX() - screenWidthHalf),
                (float) -(GetMouseY() - screenHeightHalf),
                0.f
        };
        rotation += (rotate * lookSpeed * GetFrameTime());
    }
    else if (IsMouseButtonReleased(MOUSE_RIGHT_BUTTON))
    {
        // End the click, show the cursor
        ShowCursor();
    }

    // Get movement as vector
    raylib::Vector3 move = {
            -(float) IsKeyDown(KEY_A) + (float) IsKeyDown(KEY_D),
            -(float) IsKeyDown(KEY_Q) + (float) IsKeyDown(KEY_E),
            -(float) IsKeyDown(KEY_W) + (float) IsKeyDown(KEY_S),
    };

    // Set the new position and look rotation of the camera
    camera->SetPosition(raylib::Vector3(
            camera->GetPosition()) + move.Normalize() * moveSpeed * GetFrameTime());
    camera->SetTarget(raylib::Vector3(camera->GetPosition()) + rotation);
}
