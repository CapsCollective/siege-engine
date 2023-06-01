//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Window.h"

#include <GLFW/glfw3.h>

static GLFWwindow* window = nullptr;

static size_t windowCount = 0;

void WindowResizeCallback(GLFWwindow* windowPtr, int width, int height)
{
    auto windowContext = reinterpret_cast<Siege::Window*>(glfwGetWindowUserPointer(windowPtr));
    windowContext->ResizeWindow({static_cast<uint32_t>(width), static_cast<uint32_t>(height)});
}

namespace Siege
{

Window::Window(const char* name, WindowExtents extents) : extents(extents)
{
    static bool glfwInitialised = false;
    if (!glfwInitialised)
    {
        glfwInit();
        glfwInitialised = true;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    if (!window)
    {
        window = glfwCreateWindow(extents.width, extents.height, name, nullptr, nullptr);
    }

    if (glfwRawMouseMotionSupported())
    {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    glfwSetWindowIconifyCallback(window, [](GLFWwindow* window, int iconified) {
        auto windowContext = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        windowContext->isVisible = !iconified;
    });

    glfwSetWindowUserPointer(window, this);
    glfwSetWindowSizeCallback(window, WindowResizeCallback);
    windowCount++;

    glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &DPI.width, &DPI.height);
}

Window::~Window()
{
    glfwDestroyWindow(window);
    if (--windowCount <= 0) glfwTerminate();
}

int Window::GetHeight() const
{
    return extents.height;
}

int Window::GetWidth() const
{
    return extents.width;
}

bool Window::IsVisible() const
{
    return isVisible;
}

uint32_t Window::GetDPI() const
{
    // NOTE(Aryeh): Need to find a better solution when the DPI in different axes are not the
    // same
    return DPI.width;
}

uint32_t Window::GetScaledWidth() const
{
    return extents.width * DPI.width;
}

uint32_t Window::GetScaledHeight() const
{
    return extents.height * DPI.height;
}

WindowExtents Window::GetExtents() const
{
    return extents;
}

void* Window::GetWindow()
{
    return window;
}

void Window::Update()
{
    glfwPollEvents();
}

bool Window::WindowShouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::ResizeWindow(WindowExtents newExtents)
{
    wasResized = true;
    extents = newExtents;
}

bool Window::WasResized() const
{
    return wasResized;
}

void Window::ResetWindowResized()
{
    wasResized = false;
}

void Window::EnableCursor()
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::DisableCursor()
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

MHArray<const char*> Window::GetRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    return MHArray<const char*>(glfwExtensions, glfwExtensionCount);
}

void Window::ToggleCursor(bool state)
{
    state ? DisableCursor() : EnableCursor();
}

void Window::WaitEvents()
{
    glfwWaitEvents();
}

} // namespace Siege