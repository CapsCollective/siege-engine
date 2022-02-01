#include "Input.h"

#include <raylib/raylib-cpp.hpp>

#include "../utils/TransitionAdapter.h"

bool Input::KeyDown(Key::Keyboard key)
{
    return IsKeyDown(key);
}

bool Input::KeyPressed(Key::Keyboard key)
{
    return IsKeyPressed(key);
}

bool Input::MouseDown(Mouse::Button button)
{
    return IsMouseButtonDown(button);
}

bool Input::MousePressed(Mouse::Button button)
{
    return IsMouseButtonPressed(button);
}

bool Input::MouseReleased(Mouse::Button button)
{
    return IsMouseButtonReleased(button);
}

char Input::GetKeyChar()
{
    return (char) GetCharPressed();
}

void Input::EnableMouseCursor()
{
    EnableCursor();
}

void Input::DisableMouseCursor()
{
    DisableCursor();
}

Vec3 Input::GetMousePos()
{
    return ToVec3(GetMousePosition());
}
