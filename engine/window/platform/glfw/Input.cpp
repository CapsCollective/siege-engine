//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
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

bool IsMouseButtonDown(Window window, int buttonCode)
{
    return glfwGetMouseButton(window, buttonCode);
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

} // namespace Siege::Glfw