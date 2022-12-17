//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_INPUT_H
#define SIEGE_ENGINE_INPUT_H

#include <map>

#include "../utils/Math.h"
#include "../window/Window.h"

#define KEY_W GLFW_KEY_W
#define KEY_A GLFW_KEY_A
#define KEY_S GLFW_KEY_S
#define KEY_D GLFW_KEY_D
#define KEY_Q GLFW_KEY_Q
#define KEY_E GLFW_KEY_E
#define KEY_UP GLFW_KEY_UP
#define KEY_DOWN GLFW_KEY_DOWN
#define KEY_RIGHT GLFW_KEY_RIGHT
#define KEY_LEFT GLFW_KEY_LEFT

#define KEY_ESCAPE GLFW_KEY_ESCAPE

namespace Siege
{
class Input
{
public:

    struct MouseCoordinates
    {
        double x;
        double y;
    };
    static void SetWindowPointer(Window* window);
    static bool IsKeyDown(int key);
    static bool IsKeyJustPressed(int key);

    static const MouseCoordinates& GetCursorPosition();
    static MouseCoordinates GetNormalisedMousePosition();

private:

    static Window* windowPtr;
    static bool movedLastFrame;
    static MouseCoordinates currentMouseCoordinates;
    static std::map<int, int> keyMap;

    static void GetCursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
};
} // namespace Siege

#endif