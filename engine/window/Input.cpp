//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Input.h"

#include <GLFW/glfw3.h>
#include <utils/math/Float.h>

static Siege::Window* windowPtr = nullptr;

static Siege::Input::MouseCoordinates currentMouseCoordinates;

static std::map<int, int> keyMap;

static void GetCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    currentMouseCoordinates.x = static_cast<float>(xpos);
    currentMouseCoordinates.y = static_cast<float>(ypos);
}

static GLFWwindow* AsGlfwWindow(void* window)
{
    return reinterpret_cast<GLFWwindow*>(window);
}

namespace Siege
{

void Input::SetWindowPointer(Window* window)
{
    // TODO: Fail if inputted pointer is nullptr
    windowPtr = window;
    glfwSetCursorPosCallback(AsGlfwWindow(window->GetWindow()), GetCursorPositionCallback);
}
bool Input::IsKeyDown(int key)
{
    bool hasKey = (glfwGetKey(AsGlfwWindow(windowPtr->GetWindow()), key) == GLFW_PRESS);

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
    bool hasKey =
        glfwGetKey(reinterpret_cast<GLFWwindow*>(windowPtr->GetWindow()), key) == GLFW_PRESS;

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

} // namespace Siege
