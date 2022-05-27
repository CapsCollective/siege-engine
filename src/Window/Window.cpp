#include "Window.h"

namespace SnekVk 
{
    bool Window::glfwInitialised = false;
    size_t Window::glfwWindows = 0;

    void Window::Update() 
    {
        glfwPollEvents();
    }

    bool Window::WindowShouldClose()
    {
        return glfwWindowShouldClose(window);
    }

    bool Window::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
    {
        return glfwCreateWindowSurface(instance, window, nullptr, surface) == VK_SUCCESS;
    }

    void Window::ResizeCallback(GLFWwindow* windowPtr, int width, int height)
    {
        auto window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(windowPtr));

        window->wasResized = true;
        
        window->width = width;
        window->height = height;
    }
}