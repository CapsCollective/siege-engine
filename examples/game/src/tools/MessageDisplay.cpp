//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "MessageDisplay.h"

#include "../ServiceLocator.h"

void MessageDisplay::OnDraw2D()
{
    // Draw the display message to the screen while the display time is valid
    if (displayTime > 0.f)
    {
        ServiceLocator::GetRenderer()->DrawText2D(displayMessage,
                                                  ServiceLocator::GetRenderResources()->GetFont(),
                                                  {25.f, 25.f},
                                                  {16.f, 16.f},
                                                  0.f,
                                                  Siege::IColour::Pink,
                                                  0);
        displayTime -= 0.1f;
    }
}

void MessageDisplay::DisplayMessage(const Siege::String& message, float time)
{
    displayMessage = message;
    displayTime = time;
}
