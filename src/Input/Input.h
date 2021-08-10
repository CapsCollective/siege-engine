#pragma once

#include <GLFW/glfw3.h>

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

class Input 
{
    public:
        static void SetWindowPointer(GLFWwindow* window);
        static bool IsKeyDown(int key);
    private:
        static GLFWwindow* windowPtr;
};
