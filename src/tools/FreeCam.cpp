#include "FreeCam.h"
#include <render/Camera.h>

void FreeCam::OnUpdate()
{
    if (!camera) return;

    // TODO fix bug where camera resets on rmb click sometimes
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
    {
        // Begin the click, hide the cursor
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) DisableCursor();

        // Calculate current mouse positional values
        Vector2 mousePosition = GetMousePosition();
        Vector2 mousePositionDelta = {
                mousePosition.x - previousMousePosition.x,
                mousePosition.y - previousMousePosition.y,
        };

        // Set previous mouse position
        previousMousePosition = mousePosition;

        // Calculate and set the rotational values
        Vec3 rotate = {
                mousePositionDelta.x,
                -mousePositionDelta.y,
                0.f
        };
        rotation += (rotate * lookSpeed * GetFrameTime());

        // Get movement as vector
        Vec3 move = {
                (float) (-IsKeyDown(KEY_A) + IsKeyDown(KEY_D)),
                (float) (-IsKeyDown(KEY_Q) + IsKeyDown(KEY_E)),
                (float) (-IsKeyDown(KEY_W) + IsKeyDown(KEY_S)),
        };

        // Set the new position and look rotation of the camera
        Vec3 position(camera->GetPosition());
        position = position + move.Normalise() * moveSpeed * GetFrameTime();
        Vec3 target(position + rotation);
        camera->SetPosition(position);
        camera->SetTarget(target);
    }
    else if (IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) EnableCursor(); // End the click, show the cursor

    // TODO fix camera look issues beyond 90 degrees in either direction from origin
//    std::cout << "rotation: " << rotation.x << " " << rotation.y << std::endl;
//    std::cout << "target: " << camera->GetTarget().x << " " << camera->GetTarget().y << std::endl;
}
