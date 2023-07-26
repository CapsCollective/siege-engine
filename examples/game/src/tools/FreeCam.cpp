//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "FreeCam.h"

#include <core/Statics.h>
#include <window/Input.h>
#include <core/Ticker.h>

void FreeCam::OnUpdate()
{
    if (!camera) return;

    // TODO fix bug where camera resets on rmb click sometimes
    if (Siege::Input::IsKeyDown(Siege::Mouse::MOUSE_BUTTON_RIGHT))
    {
        // Begin the click, hide the cursor
        if (Siege::Input::IsKeyJustPressed(Siege::Mouse::MOUSE_BUTTON_RIGHT))
        {
            //Siege::Statics::Input().DisableMouseCursor();
        }

        // Calculate current mouse positional values
        Siege::Vec2 cursorPos = Siege::Input::GetCursorPosition();
        Siege::MousePosition mousePosition {cursorPos.x, cursorPos.y};
        Siege::MousePosition mousePositionDelta = {
            mousePosition.x - previousMousePosition.x,
            mousePosition.y - previousMousePosition.y
        };

        // Set previous mouse position
        previousMousePosition = mousePosition;

        // Calculate and set the rotational values
        Siege::Vec3 rotate = {
            static_cast<float>(mousePositionDelta.x),
            static_cast<float>(-mousePositionDelta.y),
            0.f,
        };
        float deltaTime = ServiceLocator::GetTicker()->GetDeltaTime();
        transform.SetRotation(transform.GetRotation() + (rotate * lookSpeed * deltaTime));

        // Get movement as vector
        Siege::Vec3 move = {
            (float) (-Siege::Input::IsKeyDown(Siege::Key::KEY_A) +
                     Siege::Input::IsKeyDown(Siege::Key::KEY_D)),
            (float) (-Siege::Input::IsKeyDown(Siege::Key::KEY_Q) +
                     Siege::Input::IsKeyDown(Siege::Key::KEY_E)),
            (float) (-Siege::Input::IsKeyDown(Siege::Key::KEY_W) +
                     Siege::Input::IsKeyDown(Siege::Key::KEY_S)),
        };

        // Set the new position and look rotation of the camera
        Siege::Vec3 position(camera->GetPosition());
        position = position + Siege::Vec3::Normalise(move) * moveSpeed * deltaTime;
        Siege::Vec3 target(position + rotation);
        //camera->SetPosition(position);
        //camera->SetTarget(target);
    }

    // TODO fix camera look issues beyond 90 degrees in either direction from origin
}
void FreeCam::OnDraw3D()
{
    //camera->SetView({0.f, -1.f, -2.5f}, {0.1f, 0.1f, 0.1f});
}
