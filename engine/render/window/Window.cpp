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
GLFWwindow* Window::window = nullptr;
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

MHArray<const char*> Window::GetRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    return MHArray<const char*>(glfwExtensions, glfwExtensionCount);
}

void Window::ResizeCallback(GLFWwindow* windowPtr, int width, int height)
{
    auto windowContext = reinterpret_cast<Window*>(glfwGetWindowUserPointer(windowPtr));

    windowContext->wasResized = true;

    windowContext->width = width;
    windowContext->height = height;
}
} // namespace Siege