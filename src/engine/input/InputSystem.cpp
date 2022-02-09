#include "InputSystem.h"

#include <utils/TransitionAdapter.h>

#include <raylib/raylib-cpp.hpp>

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

Vec3 InputSystem::GetMousePos()
{
    return ToVec3(GetMousePosition());
}
