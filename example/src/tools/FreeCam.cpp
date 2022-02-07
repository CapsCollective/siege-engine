#include "FreeCam.h"

#include <engine/input/InputSystem.h>
#include <engine/render/Camera.h>
#include <engine/render/Window.h>
#include <engine/utils/Statics.h>

void FreeCam::OnUpdate()
{
    if (!camera) return;

    // TODO fix bug where camera resets on rmb click sometimes
    if (Statics::Input().MouseDown(Mouse::RIGHT))
    {
        // Begin the click, hide the cursor
        if (Statics::Input().MousePressed(Mouse::RIGHT)) Statics::Input().DisableMouseCursor();

        // Calculate current mouse positional values
        Vec3 mousePosition = Statics::Input().GetMousePos();
        Vec3 mousePositionDelta = {
            mousePosition.x - previousMousePosition.x,
            mousePosition.y - previousMousePosition.y,
            0,
        };

        // Set previous mouse position
        previousMousePosition = mousePosition;

        // Calculate and set the rotational values
        Vec3 rotate = {
            mousePositionDelta.x,
            -mousePositionDelta.y,
            0.f,
        };
        float deltaTime = Window::GetDeltaTime();
        rotation += (rotate * lookSpeed * deltaTime);

        // Get movement as vector
        Vec3 move = {
            (float) (-Statics::Input().KeyDown(Key::A) + Statics::Input().KeyDown(Key::D)),
            (float) (-Statics::Input().KeyDown(Key::Q) + Statics::Input().KeyDown(Key::E)),
            (float) (-Statics::Input().KeyDown(Key::W) + Statics::Input().KeyDown(Key::S)),
        };

        // Set the new position and look rotation of the camera
        Vec3 position(camera->GetPosition());
        position = position + move.Normalise() * moveSpeed * deltaTime;
        Vec3 target(position + rotation);
        camera->SetPosition(position);
        camera->SetTarget(target);
    }
    else if (Statics::Input().MouseReleased(Mouse::RIGHT)) Statics::Input().EnableMouseCursor();

    // TODO fix camera look issues beyond 90 degrees in either direction from origin
}
