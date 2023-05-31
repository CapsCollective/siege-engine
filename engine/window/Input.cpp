//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Input.h"

#include <utils/math/Float.h>

namespace Siege
{
Window* Input::windowPtr = nullptr;
Input::MouseCoordinates Input::currentMouseCoordinates;

std::map<int, int> Input::keyMap;

void Input::SetWindowPointer(Window* window)
{
    // TODO: Fail if inputted pointer is nullptr
    windowPtr = window;
    glfwSetCursorPosCallback(window->GetGlfwWindow(), GetCursorPositionCallback);
}
bool Input::IsKeyDown(int key)
{
    bool hasKey = (glfwGetKey(windowPtr->GetGlfwWindow(), key) == GLFW_PRESS);

    if (keyMap.find(key) != keyMap.end())
    {
        keyMap[key] = hasKey ? 1 : 0;
    }
    else
    {
        if (hasKey) keyMap[key] = 1;
    }
    return hasKey;
}

bool Input::IsKeyJustPressed(int key)
{
    bool hasKey = glfwGetKey(windowPtr->GetGlfwWindow(), key) == GLFW_PRESS;

    bool keyEntryExists = keyMap.find(key) != keyMap.end();
    if (keyEntryExists && hasKey)
    {
        if (keyMap[key] == 0)
        {
            keyMap[key] = 1;
            return true;
        }
        else return false;
    }
    else if (hasKey && !keyEntryExists)
    {
        keyMap[key] = 1;
        return true;
    }
    else if (!hasKey && keyEntryExists) keyMap[key] = 0;

    return false;
}

const Input::MouseCoordinates& Input::GetCursorPosition()
{
    return currentMouseCoordinates;
}

Input::MouseCoordinates Input::GetNormalisedMousePosition()
{
    using namespace Siege::Float;
    return {Float::Clamp(Float::Normalise(currentMouseCoordinates.x, 0, windowPtr->GetWidth()),
                         -1.f,
                         1.f),
            Float::Clamp(Float::Normalise(currentMouseCoordinates.y, 0, windowPtr->GetHeight()),
                         -1.f,
                         1.f)};
}

void Input::GetCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    currentMouseCoordinates.x = static_cast<float>(xpos);
    currentMouseCoordinates.y = static_cast<float>(ypos);
}
} // namespace Siege