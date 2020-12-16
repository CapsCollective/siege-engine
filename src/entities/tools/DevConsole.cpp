//
// Created by Jonathan Moallem on 16/12/20.
//

#include <iostream>
#include "DevConsole.h"

void DevConsole::OnUpdate()
{
    if (isActive)
    {
        // Get input from the keyboard and input it
        int key;
        while ((key = GetKeyPressed()) > 0)
        {
            if ((key >= 32) && (key <= 125)) inputText += (char) key;
        }
    }

    // Remove characters on backspace
    if (IsKeyPressed(KEY_BACKSPACE) && !inputText.empty()) inputText.pop_back();

    // Process the command on enter
    if (IsKeyPressed(KEY_ENTER))
    {
        // Run the appropriate command
        std::string command = inputText.substr(0, inputText.find(' '));
        if (command == "load")
        {
            std::cout << "loading level..." << std::endl;
        }
        else
        {
            std::cout << "Error: unknown command \"" + command +  "\"" << std::endl;
        }

        // Deactivate the console
        isActive = false;
        inputText = "";
    }

    // Toggle the console
    if (IsKeyPressed(KEY_GRAVE))
    {
        isActive = !isActive;
        if (!isActive) inputText = "";
    }
}

void DevConsole::OnUIDraw()
{
    if (!isActive) return;

    // Draw the console to the screen
    DrawRectangle(0, 0, GetScreenWidth(), 40, BLACK);
    DrawText(("~ " + inputText).c_str(), 10.f, 10.f, 20.f, WHITE);
}

bool DevConsole::IsActive() const
{
    return isActive;
}
