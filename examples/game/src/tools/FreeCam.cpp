//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "FreeCam.h"

#include <core/Statics.h>
#include <core/input/InputSystem.h>
#include <core/render/Camera.h>
#include <core/render/Window.h>

void FreeCam::OnUpdate()
{
    if (!camera) return;

    // TODO fix bug where camera resets on rmb click sometimes
    if (Siege::Statics::Input().MouseDown(Siege::Mouse::RIGHT))
    {
        // Begin the click, hide the cursor
        if (Siege::Statics::Input().MousePressed(Siege::Mouse::RIGHT))
            Siege::Statics::Input().DisableMouseCursor();

        // Calculate current mouse positional values
        Siege::Vec3 mousePosition = Siege::Statics::Input().GetMousePos();
        Siege::Vec3 mousePositionDelta = {
            mousePosition.x - previousMousePosition.x,
            mousePosition.y - previousMousePosition.y,
            0,
        };

        // Set previous mouse position
        previousMousePosition = mousePosition;

        // Calculate and set the rotational values
        Siege::Vec3 rotate = {
            mousePositionDelta.x,
            -mousePositionDelta.y,
            0.f,
        };
        float deltaTime = Siege::Window::GetDeltaTime();
        rotation += (rotate * lookSpeed * deltaTime);

        // Get movement as vector
        Siege::Vec3 move = {
            (float) (-Siege::Statics::Input().KeyDown(Siege::Key::A) +
                     Siege::Statics::Input().KeyDown(Siege::Key::D)),
            (float) (-Siege::Statics::Input().KeyDown(Siege::Key::Q) +
                     Siege::Statics::Input().KeyDown(Siege::Key::E)),
            (float) (-Siege::Statics::Input().KeyDown(Siege::Key::W) +
                     Siege::Statics::Input().KeyDown(Siege::Key::S)),
        };

        // Set the new position and look rotation of the camera
        Siege::Vec3 position(camera->GetPosition());
        position = position + Siege::Vec3::Normalise(move) * moveSpeed * deltaTime;
        Siege::Vec3 target(position + rotation);
        camera->SetPosition(position);
        camera->SetTarget(target);
    }
    else if (Siege::Statics::Input().MouseReleased(Siege::Mouse::RIGHT))
        Siege::Statics::Input().EnableMouseCursor();

    // TODO fix camera look issues beyond 90 degrees in either direction from origin
}
