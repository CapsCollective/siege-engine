#include "DevConsole.h"
#include "MessageDisplay.h"
#include "../../utils/SceneLoader.h"
#include "../../utils/StringHelpers.h"
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
            if (CheckArgs("load", argument))
            {
                // Deselect all entities if in editor mode
                if (isEditorMode) ServiceLocator::GetEditorController()->SelectEntity(nullptr);

                // Try load the scene specified
                SceneLoader::QueueNextScene(argument);
            }
        }
        else if (command == "save")
        {
            if (CheckEditorMode())
            {
                // Save the scene as the current scene name (or untitled if argument blank)
                SceneLoader::SaveScene(argument);
                messageDisplay->DisplayMessage("Scene saved");
            }
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
            if (CheckEditorMode() && CheckArgs("add", argument))
            {
                // Try add the entity to the scene
                if (EditorController::TryAddEntity(argument))
                {
                    messageDisplay->DisplayMessage("Added entity to scene");
                }
                else messageDisplay->DisplayMessage("Error: Cannot add entity \"" + argument + "\"");
            }
        }
        else if (command == "setpos")
        {
            if (CheckEditorMode() && CheckArgs("setpos", argument))
            {
                // Try convert the argument to a Vector3, and set the entity's position
                try {
                    raylib::Vector3 position = StringHelpers::StringToVector(argument);
                    if (ServiceLocator::GetEditorController()->TrySetPos(position))
                    {
                        messageDisplay->DisplayMessage("Entity position set to " + argument);
                    }
                    else messageDisplay->DisplayMessage("Error: No entity selected");
                }
                catch (const std::invalid_argument& err) {
                    messageDisplay->DisplayMessage("Error: Invalid type of Vector3 components, should be float");
                }
                catch (const std::length_error& err) {
                    messageDisplay->DisplayMessage("Error: Invalid number of Vector3 components, should be 3");
                }
            }
        }
        else if (command == "setrot")
        {
            if (CheckEditorMode() && CheckArgs("setrot", argument))
            {
                // Try convert the argument to float, and set the entity's rotation
                try {
                    if (ServiceLocator::GetEditorController()->TrySetRot(std::stof(argument)))
                    {
                        messageDisplay->DisplayMessage("Entity rotation set to " + argument + "°");
                    }
                    else messageDisplay->DisplayMessage("Error: No entity selected");
                }
                catch (const std::invalid_argument& err) {
                    messageDisplay->DisplayMessage("Error: Invalid float input");
                }
            }
        }
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
    if (!isActive) return;

    // Draw the console to the screen
    DrawRectangle(0, 0, GetScreenWidth(), 40, BLACK);
    DrawText(("~ " + inputText).c_str(), 10.f, 10.f, 20.f, WHITE);
}

bool DevConsole::CheckEditorMode()
{
    if (!isEditorMode) messageDisplay->DisplayMessage("Error: command not valid in editor mode");
    return isEditorMode;
}

bool DevConsole::CheckArgs(const std::string& command, const std::string& args)
{
    if (args.empty()) messageDisplay->DisplayMessage("Error: missing argument for " + command +  " command");
    return !args.empty();
}