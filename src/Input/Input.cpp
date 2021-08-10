#include "Input.h"
#include <iostream>
    
SnekVk::Window* Input::windowPtr = nullptr;
Input::MouseCoordinates Input::currentMouseCoordinates;
bool Input::movedLastFrame{false};

std::map<int, int> Input::keyMap;

void Input::SetWindowPointer(SnekVk::Window* window)
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
    std::cout << hasKey << std::endl;
    return hasKey;
}

bool Input::IsKeyJustPressed(int key)
{
    bool hasKey = glfwGetKey(windowPtr->GetGlfwWindow(), key) == GLFW_PRESS;
    std::cout << hasKey << std::endl;

    bool keyEntryExists = keyMap.find(key) != keyMap.end();
    if (keyEntryExists && hasKey)
    {
        if (keyMap[key] == 0) 
        {
            keyMap[key] = 1;
            return true;
        }
        else 
        {
            return false;
        }
    } 
    else if (hasKey && !keyEntryExists)
    {
        keyMap[key] = 1;
        return true;
    } 
    else if (!hasKey && keyEntryExists)
    {
        keyMap[key] = 0;
    }
    
    return false;
}

const Input::MouseCoordinates& Input::GetCursorPosition()
{
    return currentMouseCoordinates;
}

Input::MouseCoordinates Input::GetNormalisedMousePosition()
{
    return {
        Utils::Math::Normalise(currentMouseCoordinates.x, 0, windowPtr->GetWidth()),
        Utils::Math::Normalise(currentMouseCoordinates.y, 0, windowPtr->GetHeight())
    };
}

void Input::GetCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    currentMouseCoordinates.x = static_cast<float>(xpos);
    currentMouseCoordinates.y = static_cast<float>(ypos);
}


        