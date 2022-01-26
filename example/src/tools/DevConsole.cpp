#include "DevConsole.h"

#include <input/Input.h>
#include <render/RenderSystem.h>
#include <render/Window.h>
#include <scene/SceneManager.h>

#include <stdexcept>

#include "EditorController.h"
#include "MessageDisplay.h"

void DevConsole::OnUpdate()
{
    if (!messageDisplay) return;

    if (Input::KeyPressed(Input::KEY_GRAVE))
    {
        // Toggle the console
        isActive = !isActive;
        if (!isActive) inputText = "";
        return;
    }

    if (!isActive) return;

    // Get input from the keyboard and input it
    char key;
    while ((key = (char) Input::GetKeyChar()) > 0)
    {
        if ((key >= 32) && (key <= 125)) inputText += key;
    }

    // Remove characters on backspace
    if (Input::KeyPressed(Input::KEY_BACKSPACE) && !inputText.empty()) inputText.pop_back();

    // Get the last command you ran - only works once.
    if (Input::KeyPressed(Input::KEY_UP) && !lastInput.empty()) inputText = lastInput;

    // Process the command on enter
    if (Input::KeyPressed(Input::KEY_ENTER))
    {
        // Process the input into command and argument format
        std::string command;
        std::string argument;
        size_t separatorPos = inputText.find(' ');
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
                SceneManager::QueueNextScene(argument);
            }
        }
        else if (command == "save")
        {
            if (CheckEditorMode())
            {
                // Save the scene as the current scene name (or untitled if argument blank)
                SceneManager::SaveScene(argument);
                messageDisplay->DisplayMessage("Scene saved");
            }
        }
        else if (command == "new")
        {
            // Deselect all entities and open a new, untitled scene
            ServiceLocator::GetEditorController()->SelectEntity(nullptr);
            SceneManager::NewScene();
            messageDisplay->DisplayMessage("Created new scene");
        }
        else if (command == "add")
        {
            if (CheckEditorMode() && CheckArgs("add", argument))
            {
                // Try add the entity to the scene
                if (EditorController::TryAddEntity(argument.c_str()))
                {
                    messageDisplay->DisplayMessage("Added entity to scene");
                }
                else
                {
                    messageDisplay->DisplayMessage("Error: Cannot add entity \"" + argument + "\"");
                }
            }
        }
        else if (command == "setpos")
        {
            if (CheckEditorMode() && CheckArgs("setpos", argument))
            {
                // Try to convert the argument to a Vector3, and set the entity's position
                Vec3 position;
                if (Vec3::FromString(position, argument.c_str()))
                {
                    if (ServiceLocator::GetEditorController()->TrySetPos(position))
                    {
                        messageDisplay->DisplayMessage("Entity position set to " + argument);
                    }
                    else messageDisplay->DisplayMessage("Error: No entity selected");
                }
                else messageDisplay->DisplayMessage("Error: Invalid Vector3 input");
            }
        }
        else if (command == "setrot")
        {
            if (CheckEditorMode() && CheckArgs("setrot", argument))
            {
                // Try convert the argument to float, and set the entity's rotation
                try
                {
                    // TODO remove stof
                    if (ServiceLocator::GetEditorController()->TrySetRot(std::stof(argument)))
                    {
                        messageDisplay->DisplayMessage("Entity rotation set to " + argument + "°");
                    }
                    else messageDisplay->DisplayMessage("Error: No entity selected");
                }
                catch (const std::invalid_argument& err)
                {
                    messageDisplay->DisplayMessage("Error: Invalid float input");
                }
            }
        }
        else messageDisplay->DisplayMessage("Error: unknown command \"" + command + "\"");

        // Deactivate the console
        isActive = false;
        lastInput = inputText;
        inputText = "";
    }
}

void DevConsole::OnDraw2D()
{
    if (!isActive) return;

    Window* window = ServiceLocator::GetWindow();

    // Draw the console to the screen
    RenderSystem::DrawRectangle2D(0, 0, window->GetWidth(), 40, Colour::Black);
    RenderSystem::DrawText2D("~ " + inputText, 10.f, 10.f, 20.f, Colour::White);
}

bool DevConsole::CheckEditorMode()
{
    if (!isEditorMode) messageDisplay->DisplayMessage("Error: command not valid in editor mode");
    return isEditorMode;
}

bool DevConsole::CheckArgs(const std::string& command, const std::string& args)
{
    if (args.empty())
    {
        messageDisplay->DisplayMessage("Error: missing argument for " + command + " command");
    }
    return !args.empty();
}