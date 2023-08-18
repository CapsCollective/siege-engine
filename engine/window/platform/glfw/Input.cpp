//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Input.h"

#include <GLFW/glfw3.h>

namespace Siege::Glfw
{
bool IsKeyDown(Window window, int keyCode)
{
    return glfwGetKey(window, keyCode) == GLFW_PRESS;
}

bool IsKeyReleased(Window window, int keyCode)
{
    return glfwGetKey(window, keyCode) == GLFW_RELEASE;
}

bool IsMouseButtonDown(Window window, int buttonCode)
{
    return glfwGetMouseButton(window, buttonCode) == GLFW_PRESS;
}

bool IsMouseButtonReleased(Window window, int buttonCode)
{
    return glfwGetMouseButton(window, buttonCode) == GLFW_RELEASE;
}

void SetOnKeyPressedCallback(Window window, SetKeyPressedCallback callback)
{
    glfwSetKeyCallback(window, callback);
}

void SetOnTextKeyPressedCallback(Window window, SetTextKeyPressedCallback callback)
{
    glfwSetCharCallback(window, callback);
}

void SetOnMouseKeyPressedCallback(Window window, SetMouseButtonPressedCallback callback)
{
    glfwSetMouseButtonCallback(window, callback);
}

void SetOnMouseMovedCallback(Window window, SetMousePositionCallback callback)
{
    glfwSetCursorPosCallback(window, callback);
}

void SetIsCursorVisible(Window window, bool isCursorVisible)
{
    glfwSetInputMode(
        window,
        GLFW_CURSOR,
        (isCursorVisible * GLFW_CURSOR_NORMAL) + (!isCursorVisible * GLFW_CURSOR_DISABLED));
}

} // namespace Siege::Glfw