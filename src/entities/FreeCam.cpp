//
// Created by Jonathan Moallem on 14/12/20.
//

#include <iostream>
#include "FreeCam.h"

void FreeCam::OnUpdate()
{
    if (!camera) return;

    // Calculate current mouse positional values
    Vector2 mousePosition = GetMousePosition();
    Vector2 mousePositionDelta = {
            mousePosition.x - previousMousePosition.x,
            mousePosition.y - previousMousePosition.y,
    };

    // Set previous mouse position
    previousMousePosition = mousePosition;

    if (IsKeyDown(KEY_LEFT_ALT))
    {
        // Begin the click, hide the cursor
        if (IsKeyPressed(KEY_LEFT_ALT)) DisableCursor();

        // Calculate and set the rotational values
        raylib::Vector3 rotate = {
                mousePositionDelta.x,
                -mousePositionDelta.y,
                0.f
        };
        rotation += (rotate * lookSpeed * GetFrameTime());
    }
    else if (IsKeyReleased(KEY_LEFT_ALT)) EnableCursor(); // End the click, show the cursor

    // Get movement as vector
    raylib::Vector3 move = {
            (float) (-IsKeyDown(KEY_A) + IsKeyDown(KEY_D)),
            (float) (-IsKeyDown(KEY_Q) + IsKeyDown(KEY_E)),
            (float) (-IsKeyDown(KEY_W) + IsKeyDown(KEY_S)),
    };

    // Set the new position and look rotation of the camera
    camera->SetPosition(raylib::Vector3(
            camera->GetPosition()) + move.Normalize() * moveSpeed * GetFrameTime());
    camera->SetTarget(raylib::Vector3(camera->GetPosition()) + rotation);

    // TODO fix camera look issues beyond 90 degrees in either direction from origin
    std::cout << "rotation: " << rotation.x << " " << rotation.y << std::endl;
    std::cout << "target: " << camera->GetTarget().x << " " << camera->GetTarget().y << std::endl;
}
