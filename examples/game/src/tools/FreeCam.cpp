//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "FreeCam.h"

#include <window/Input.h>

void FreeCam::OnUpdate()
{
    using namespace Siege;
    if (!camera) return;

    if (Input::IsMouseButtonJustPressed(MOUSE_BUTTON_RIGHT)) Siege::Input::DisableCursor();

    if (Input::IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        camera->MoveCamera(ServiceLocator::GetTicker()->GetDeltaTime());
    } else if (Input::IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) Input::EnableCursor();

}
void FreeCam::OnDraw3D()
{}
