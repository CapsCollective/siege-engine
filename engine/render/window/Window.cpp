//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Window.h"

namespace Siege
{
bool Window::glfwInitialised = false;
size_t Window::glfwWindows = 0;
GLFWwindow* Window::window = nullptr;

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

Array<const char*> Window::GetRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    return Array<const char*>(glfwGetRequiredInstanceExtensions(&glfwExtensionCount),
                              glfwExtensionCount);
}

void Window::ResizeCallback(GLFWwindow* windowPtr, int width, int height)
{
    auto window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(windowPtr));

    window->wasResized = true;

    window->width = width;
    window->height = height;
}
} // namespace Siege