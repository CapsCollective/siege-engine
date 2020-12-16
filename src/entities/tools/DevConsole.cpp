#include <iostream>
#include "DevConsole.h"
#include "../../utils/SceneLoader.h"

void DevConsole::OnUpdate()
{
    if (IsKeyDown(KEY_LEFT_SUPER) && IsKeyPressed(KEY_S))
    {
        // Save the current scene
        SceneLoader::SaveScene(currentScene);
        DisplayMessage("Scene saved", 10.f);
    }

    if (isActive)
    {
        // Get input from the keyboard and input it
        int key;
        while ((key = GetKeyPressed()) > 0)
        {
            if ((key >= 32) && (key <= 125)) inputText += (char) key;
        }

        // Get the last command you ran - only works once.
        if (IsKeyPressed(KEY_UP) && !lastInput.empty()) inputText = lastInput;
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
                currentScene = argument;
                SceneLoader::LoadScene(currentScene);
                DisplayMessage("Scene loaded", 10.f);
            }
        }
        else if (command == "save")
        {
            currentScene = argument.empty() ? "untitled" : argument;
            SceneLoader::SaveScene(currentScene);
            DisplayMessage("Scene saved", 10.f);
        }
        else
        {
            std::cout << "Error: unknown command \"" + command +  "\"" << std::endl;
        }

        // Deactivate the console
        isActive = false;
        lastInput = inputText;
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
    if (isActive)
    {
        // Draw the console to the screen
        DrawRectangle(0, 0, GetScreenWidth(), 40, BLACK);
        DrawText(("~ " + inputText).c_str(), 10.f, 10.f, 20.f, WHITE);
    }

    // Draw the display message to the screen while the display time is valid
    if (displayTime > 0.f)
    {
        DrawText(displayMessage.c_str(), 10, 40, 20, PINK);
        displayTime -= 0.1f;
    }
}

void DevConsole::DisplayMessage(const std::string &message, float time)
{
    displayMessage = message;
    displayTime = time;
}

bool DevConsole::IsActive() const
{
    return isActive;
}
