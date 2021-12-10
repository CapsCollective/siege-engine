#include "FreeCam.h"
#include <render/Camera.h>
#include <input/Input.h>
#include <render/Window.h>

void FreeCam::OnUpdate()
{
    if (!camera) return;

    // TODO fix bug where camera resets on rmb click sometimes
    if (Input::MouseDown(Input::MOUSE_RIGHT_BUTTON))
    {
        // Begin the click, hide the cursor
        if (Input::MousePressed(Input::MOUSE_RIGHT_BUTTON)) Input::DisableMouseCursor();

        // Calculate current mouse positional values
        Vec3 mousePosition = Input::GetMousePos();
        Vec3 mousePositionDelta = {
                mousePosition.x - previousMousePosition.x,
                mousePosition.y - previousMousePosition.y,
                0
        };

        // Set previous mouse position
        previousMousePosition = mousePosition;

        // Calculate and set the rotational values
        Vec3 rotate = {
                mousePositionDelta.x,
                -mousePositionDelta.y,
                0.f
        };
        float deltaTime = Window::GetDeltaTime();
        rotation += (rotate * lookSpeed * deltaTime);

        // Get movement as vector
        Vec3 move = {
                (float) (-Input::KeyDown(Input::KEY_A) + Input::KeyDown(Input::KEY_D)),
                (float) (-Input::KeyDown(Input::KEY_Q) + Input::KeyDown(Input::KEY_E)),
                (float) (-Input::KeyDown(Input::KEY_W) + Input::KeyDown(Input::KEY_S)),
        };

        // Set the new position and look rotation of the camera
        Vec3 position(camera->GetPosition());
        position = position + move.Normalise() * moveSpeed * deltaTime;
        Vec3 target(position + rotation);
        camera->SetPosition(position);
        camera->SetTarget(target);
    }
    else if (Input::MouseReleased(Input::MOUSE_RIGHT_BUTTON)) Input::EnableMouseCursor();

    // TODO fix camera look issues beyond 90 degrees in either direction from origin
//    std::cout << "rotation: " << rotation.x << " " << rotation.y << std::endl;
//    std::cout << "target: " << camera->GetTarget().x << " " << camera->GetTarget().y << std::endl;
}
