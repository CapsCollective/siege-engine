//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "DevConsole.h"

#include <core/Statics.h>
#include <core/scene/SceneSystem.h>
#include <utils/math/vec/Format.h>
#include <window/Input.h>

#include <stdexcept>

#include "EditorController.h"

void DevConsole::OnUpdate()
{
    if (!messageDisplay) return;

    if (Siege::Input::IsKeyJustPressed(Siege::Key::KEY_GRAVE_ACCENT))
    {
        // Toggle the console
        isActive = !isActive;
        if (!isActive) inputText = "";
        if (isEditorMode) ServiceLocator::GetEditorController()->SetIsHandlingInput(!isActive);
        return;
    }

    if (!isActive) return;

    // Get input from the keyboard and input it
    char key;
    while ((key = (char) Siege::Input::GetLatestChar()) > 0)
    {
        if ((key >= 32) && (key <= 125) &&
            !(key == Siege::Key::KEY_GRAVE_ACCENT && inputText.IsEmpty()))
            inputText += key;
    }

    int latestKey = Siege::Input::GetLatestKey();

    // Remove characters on backspace
    if (latestKey == Siege::Key::KEY_BACKSPACE && !inputText.IsEmpty()) inputText.PopBack();

    // Get the last command you ran - only works once.
    if (latestKey == Siege::Key::KEY_UP && !lastInput.IsEmpty()) inputText = lastInput;

    // Process the command on enter
    if (Siege::Input::IsKeyJustPressed(Siege::Key::KEY_ENTER))
    {
        // Process the input into command and argument format
        auto args = inputText.Split(' ');
        Siege::String command(!args.empty() ? args[0] : nullptr);
        Siege::String argument(args.size() > 1 ? args[1] : nullptr);
        // Run the appropriate instructions for specified command
        if (command == "load")
        {
            if (CheckArgs("load", argument))
            {
                // Deselect all entities if in editor mode
                if (isEditorMode) ServiceLocator::GetEditorController()->SelectEntity(nullptr);

                // Try load the scene specified
                Siege::Statics::Scene().QueueNextScene(argument);
            }
        }
        else if (command == "save")
        {
            if (CheckEditorMode())
            {
                // Save the scene as the current scene name (or untitled if argument blank)
                Siege::Statics::Scene().SaveScene(argument);
                messageDisplay->DisplayMessage("Scene saved");
            }
        }
        else if (command == "new")
        {
            // Deselect all entities and open a new, untitled scene
            ServiceLocator::GetEditorController()->SelectEntity(nullptr);
            Siege::Statics::Scene().NewScene();
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
                Siege::Vec3 position;
                if (Siege::FromString(position, argument))
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
                // Try to convert the argument to float, and set the entity's rotation
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
        ServiceLocator::GetEditorController()->SetIsHandlingInput(true);
    }
}

void DevConsole::OnDraw2D()
{
    if (!isActive) return;

    Siege::Window* window = ServiceLocator::GetWindow();

    // Draw the console to the screen
    ServiceLocator::GetRenderer()->DrawQuad({0.f, -1.f},
                                            {((float) window->GetWidth()) / 2, 25.f},
                                            Siege::IColour::Black,
                                            0,
                                            1);
    ServiceLocator::GetRenderer()->DrawText2D("> " + inputText,
                                              ServiceLocator::GetRenderResources()->GetFont(),
                                              {10.f, 10.f},
                                              {20.f, 20.f},
                                              0.f,
                                              Siege::IColour::White,
                                              0);
}

bool DevConsole::CheckEditorMode()
{
    if (!isEditorMode) messageDisplay->DisplayMessage("Error: command not valid in editor mode");
    return isEditorMode;
}

bool DevConsole::CheckArgs(const Siege::String& command, const Siege::String& args)
{
    if (args.IsEmpty())
    {
        messageDisplay->DisplayMessage("Error: missing argument for " + command + " command");
    }
    return !args.IsEmpty();
}