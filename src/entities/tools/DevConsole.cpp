#include "DevConsole.h"
#include "../../utils/SceneLoader.h"

void DevConsole::OnToolUpdate()
{
    // TODO move scene loading responsibilities out of dev console
    if (IsKeyDown(KEY_LEFT_SUPER) && IsKeyPressed(KEY_S))
    {
        // Save the current scene
        SceneLoader::SaveScene(currentScene);
        messageDisplay->DisplayMessage("Scene saved");
    }

    if (isActive)
    {
        // Get input from the keyboard and input it
        int key;
        while ((key = GetKeyPressed()) > 0)
        {
            if ((key >= 32) && (key <= 125)) inputText += (char) key;
        }

        // Remove characters on backspace
        if (IsKeyPressed(KEY_BACKSPACE) && !inputText.empty()) inputText.pop_back();

        // Get the last command you ran - only works once.
        if (IsKeyPressed(KEY_UP) && !lastInput.empty()) inputText = lastInput;

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

            // Run the appropriate instructions for specified command
            if (command == "load")
            {
                if (!argument.empty())
                {
                    // Try load the scene specified
                    currentScene = argument;
                    if (SceneLoader::LoadScene(currentScene))
                    {
                        messageDisplay->DisplayMessage(currentScene + ".scene loaded");
                    }
                    else messageDisplay->DisplayMessage(currentScene + ".scene not found");
                }
                else messageDisplay->DisplayMessage("Error: missing argument for " + command +  " command");
            }
            else if (command == "save")
            {
                if (isEditorMode)
                {
                    // Save the scene as the current scene name (or untitled if argument blank)
                    currentScene = argument.empty() ? "untitled" : argument;
                    SceneLoader::SaveScene(currentScene);
                    messageDisplay->DisplayMessage("Scene saved");
                }
                else messageDisplay->DisplayMessage("Cannot save from play mode");
            }
            else
            {
                messageDisplay->DisplayMessage("Error: unknown command \"" + command +  "\"");
            }

            // Deactivate the console
            isActive = false;
            lastInput = inputText;
            inputText = "";
        }
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
}

bool DevConsole::IsActive() const
{
    return isActive;
}
