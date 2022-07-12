//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "InputSystem.h"

#include <raylib/raylib-cpp.hpp>

#include "../TransitionAdapter.h"

bool InputSystem::KeyDown(Key::Keyboard key)
{
    return IsKeyDown(key);
}

bool InputSystem::KeyPressed(Key::Keyboard key)
{
    return IsKeyPressed(key);
}

bool InputSystem::MouseDown(Mouse::Button button)
{
    return IsMouseButtonDown(button);
}

bool InputSystem::MousePressed(Mouse::Button button)
{
    return IsMouseButtonPressed(button);
}

bool InputSystem::MouseReleased(Mouse::Button button)
{
    return IsMouseButtonReleased(button);
}

char InputSystem::GetKeyChar()
{
    return (char) GetCharPressed();
}

void InputSystem::EnableMouseCursor()
{
    EnableCursor();
}

void InputSystem::DisableMouseCursor()
{
    DisableCursor();
}

Siege::Vec3 InputSystem::GetMousePos()
{
    return ToVec3(GetMousePosition());
}
