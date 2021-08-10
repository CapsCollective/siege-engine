#include "Input.h"
    GLFWwindow* Input::windowPtr = nullptr;

    void Input::SetWindowPointer(GLFWwindow* window)
    {
        // TODO: Fail if inputted pointer is nullptr
        windowPtr = window;
    }
    bool Input::IsKeyDown(int key)
    {
        return glfwGetKey(windowPtr, key) == GLFW_PRESS;
    }

        