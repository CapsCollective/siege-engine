#include "DevConsole.h"

#include <input/InputSystem.h>
#include <render/RenderSystem.h>
#include <render/Window.h>
#include <scene/SceneSystem.h>
#include <utils/Statics.h>

#include <stdexcept>

#include "EditorController.h"
#include "MessageDisplay.h"

void DevConsole::OnUpdate()
{
    if (!messageDisplay) return;

    if (Statics::Input().KeyPressed(Key::GRAVE))
    {
        // Toggle the console
        isActive = !isActive;
        if (!isActive) inputText = "";
        return;
    }

    if (!isActive) return;

    // Get input from the keyboard and input it
    char key;
    while ((key = (char) Statics::Input().GetKeyChar()) > 0)
    {
        if ((key >= 32) && (key <= 125)) inputText += key;
    }

    // Remove characters on backspace
    if (Statics::Input().KeyPressed(Key::BACKSPACE) && !inputText.IsEmpty()) inputText.PopBack();

    // Get the last command you ran - only works once.
    if (Statics::Input().KeyPressed(Key::UP) && !lastInput.IsEmpty()) inputText = lastInput;

    // Process the command on enter
    if (Statics::Input().KeyPressed(Key::ENTER))
    {
        // Process the input into command and argument format
        auto args = inputText.Split(' ');
        String command(!args.empty() ? args[0] : nullptr);
        String argument(args.size() > 1 ? args[1] : nullptr);

        // Run the appropriate instructions for specified command
        if (command == "load")
        {
            if (CheckArgs("load", argument))
            {
                // Deselect all entities if in editor mode
                if (isEditorMode) ServiceLocator::GetEditorController()->SelectEntity(nullptr);

                // Try load the scene specified
                Statics::Scene().QueueNextScene(argument);
            }
        }
        else if (command == "save")
        {
            if (CheckEditorMode())
            {
                // Save the scene as the current scene name (or untitled if argument blank)
                Statics::Scene().SaveScene(argument);
                messageDisplay->DisplayMessage("Scene saved");
            }
        }
        else if (command == "new")
        {
            // Deselect all entities and open a new, untitled scene
            ServiceLocator::GetEditorController()->SelectEntity(nullptr);
            Statics::Scene().NewScene();
            messageDisplay->DisplayMessage("Created new scene");
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
                if (Vec3::FromString(position, argument))
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
                    float rotation;
                    if (argument.GetFloat(rotation) &&
                        ServiceLocator::GetEditorController()->TrySetRot(rotation))
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
    Statics::Render().DrawRectangle2D(0, 0, window->GetWidth(), 40, Colour::Black);
    Statics::Render().DrawText2D("~ " + inputText, 10.f, 10.f, 20.f, Colour::White);
}

bool DevConsole::CheckEditorMode()
{
    if (!isEditorMode) messageDisplay->DisplayMessage("Error: command not valid in editor mode");
    return isEditorMode;
}

bool DevConsole::CheckArgs(const String& command, const String& args)
{
    if (args.IsEmpty())
    {
        messageDisplay->DisplayMessage("Error: missing argument for " + command + " command");
    }
    return !args.IsEmpty();
}