#include "Window.h"

namespace SnekVk 
{
    bool Window::glfwInitialised = false;
    size_t Window::glfwWindows = 0;

    void Window::Update() 
    {
        glfwPollEvents();
    }

    void Window::DestroyWindow()
    {
        glfwDestroyWindow(window);
        if (--glfwWindows <= 0) glfwTerminate();
    }

    bool Window::WindowShouldClose()
    {
        return glfwWindowShouldClose(window);
    }

    void Window::InitWindow()
    {
        if (!glfwInitialised)
        {
            glfwInit();
            glfwInitialised = true;    
        }
        
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(width, height, name, nullptr, nullptr);
        glfwWindows++;
    }
}