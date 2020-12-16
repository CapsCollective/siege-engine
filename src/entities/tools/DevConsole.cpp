//
// Created by Jonathan Moallem on 16/12/20.
//

#include <iostream>
#include "DevConsole.h"
#include "../../utils/SceneLoader.h"

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
        // Process the input into command and argument format
        std::string command;
        std::string argument;
        int separatorPos = inputText.find(' ');
        if (separatorPos != -1)
        {
            command = inputText.substr(0, separatorPos);
            argument = inputText.substr(command.size() + 1, inputText.size());
        }
        else command = inputText;

        // Run the appropriate command
        if (command == "load")
        {
            if (argument.empty())
            {
                std::cout << "Error: missing argument for " + command +  " command" << std::endl;
            }
            else
            {
                // TODO clear the current scene
                SceneLoader::LoadScene(argument);
            }
        }
        else if (command == "save")
        {
            SceneLoader::SaveScene(argument.empty() ? "untitled" : argument);
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
