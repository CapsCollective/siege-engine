#include "DevConsole.h"
#include "MessageDisplay.h"
#include "../../utils/SceneLoader.h"
#include "EditorController.h"
#include "Profiler.h"

void DevConsole::OnUpdate()
{
    if (!messageDisplay) return;

    if (IsKeyPressed(KEY_GRAVE))
    {
        // Toggle the console
        isActive = !isActive;
        if (!isActive) inputText = "";
        return;
    }

    if (!isActive) return;

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
                // Deselect all entities if in editor mode
                if (isEditorMode) ServiceLocator::GetEditorController()->SelectEntity(nullptr);

                // Try load the scene specified
                SceneLoader::QueueNextScene(argument);
            }
            else messageDisplay->DisplayMessage("Error: missing argument for " + command +  " command");
        }
        else if (command == "save")
        {
            if (isEditorMode)
            {
                // Save the scene as the current scene name (or untitled if argument blank)
                SceneLoader::SaveScene(argument);
                messageDisplay->DisplayMessage("Scene saved");
            }
            else messageDisplay->DisplayMessage("Cannot save from play mode");
        }
        else if (command == "new")
        {
            // Deselect all entities and open a new, untitled scene
            ServiceLocator::GetEditorController()->SelectEntity(nullptr);
            SceneLoader::NewScene();
            messageDisplay->DisplayMessage("Created new scene");
        }
        else if (command == "stats")
        {
            // Toggle the profiler
            profiler->ToggleActive();
        }
        else if (command == "add")
        {
            if (!argument.empty())
            {
                if (isEditorMode)
                {
                    // Try add the entity to the scene
                    if (EditorController::TryAddEntity(argument))
                    {
                        messageDisplay->DisplayMessage("Added entity to scene");
                    }
                    else
                    {
                        messageDisplay->DisplayMessage("Cannot add entity \"" + argument + "\"");
                    }
                }
                else messageDisplay->DisplayMessage("Cannot modify scenes from play mode");
            }
            else messageDisplay->DisplayMessage("Error: missing argument for " + command +  " command");
        }
        // TODO add setpos/setrot command
        // TODO add setmod/settex command
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

void DevConsole::OnUIDraw()
{
    if (isActive)
    {
        // Draw the console to the screen
        DrawRectangle(0, 0, GetScreenWidth(), 40, BLACK);
        DrawText(("~ " + inputText).c_str(), 10.f, 10.f, 20.f, WHITE);
    }
}
