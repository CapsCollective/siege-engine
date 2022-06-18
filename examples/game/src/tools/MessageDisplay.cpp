//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "MessageDisplay.h"

#include <core/Statics.h>
#include <core/render/RenderSystem.h>

void MessageDisplay::OnDraw2D()
{
    // Draw the display message to the screen while the display time is valid
    if (displayTime > 0.f)
    {
        Statics::Render().DrawText2D(displayMessage, 10, 10, 20, Colour::Pink);
        displayTime -= 0.1f;
    }
}

void MessageDisplay::DisplayMessage(const String& message, float time)
{
    displayMessage = message;
    displayTime = time;
}
