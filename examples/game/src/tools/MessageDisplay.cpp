//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "MessageDisplay.h"

#include <core/Statics.h>
#include "../ServiceLocator.h"

void MessageDisplay::OnDraw2D()
{
    auto pixel = Siege::Vulkan::Font("assets/fonts/PublicPixel.ttf");

    // Draw the display message to the screen while the display time is valid
    if (displayTime > 0.f)
    {
        ServiceLocator::GetRenderer()->DrawText2D(displayMessage,
                                                           pixel,
                                                           {25.f, 25.f},
                                                           {50.f, 50.f},
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
