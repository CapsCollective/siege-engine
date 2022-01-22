#include "MessageDisplay.h"

#include <render/RenderSystem.h>

void MessageDisplay::OnDraw2D()
{
    // Draw the display message to the screen while the display time is valid
    if (displayTime > 0.f)
    {
        RenderSystem::DrawText2D(displayMessage, 10, 10, 20, Colour::Pink);
        displayTime -= 0.1f;
    }
}

void MessageDisplay::DisplayMessage(const std::string& message, float time)
{
    displayMessage = message;
    displayTime = time;
}
