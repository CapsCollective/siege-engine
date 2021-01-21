#include "MessageDisplay.h"

void MessageDisplay::OnDraw2D()
{
    // Draw the display message to the screen while the display time is valid
    if (displayTime > 0.f)
    {
        DrawText(displayMessage.c_str(), 10, 10, 20, PINK);
        displayTime -= 0.1f;
    }
}

void MessageDisplay::DisplayMessage(const std::string &message, float time)
{
    displayMessage = message;
    displayTime = time;
}
