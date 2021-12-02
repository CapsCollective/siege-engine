#include "Input.h"
#include "../utils/TransitionAdapter.h"
#include <raylib/raylib-cpp.hpp>

bool Input::KeyDown(KeyboardKey key)
{
    return IsKeyDown(key);
}

bool Input::KeyPressed(KeyboardKey key)
{
    return IsKeyPressed(key);
}

bool Input::MouseDown(MouseButton button)
{
    return IsMouseButtonDown(button);
}

bool Input::MousePressed(MouseButton button)
{
    return IsMouseButtonPressed(button);
}

bool Input::MouseReleased(MouseButton button)
{
    return IsMouseButtonReleased(button);
}

char Input::GetKeyChar()
{
    return GetCharPressed();
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
