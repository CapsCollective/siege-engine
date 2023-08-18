//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Window.h"

#include <GLFW/glfw3.h>

#include "window/Window.h"

namespace Siege::Glfw
{

bool Initialize()
{
    if (!glfwInitialised) glfwInitialised = glfwInit();
    rawMouseMotionSupported = glfwRawMouseMotionSupported();
    return glfwInitialised;
}

Window CreateWindow(const char* title,
                    unsigned int width,
                    unsigned int height,
                    void* windowBase,
                    bool isResizable)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, isResizable);

    auto window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    glfwSetWindowUserPointer(window, windowBase);

    return window;
}

void SetWindowMinimisedCallback(Window window, IsMinimisedCallback callback)
{
    glfwSetWindowIconifyCallback(window, callback);
}

void SetWindowResizedCallback(Window window, IsResizedCallback callback)
{
    glfwSetWindowSizeCallback(window, callback);
}

MonitorPixelScale GetWindowDPI()
{
    MonitorPixelScale DPI {};

    glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &DPI.width, &DPI.height);

    return DPI;
}

void* GetUserPtr(Window window)
{
    return glfwGetWindowUserPointer(window);
}

void WaitEvents()
{
    glfwWaitEvents();
}

void GetEvents()
{
    glfwPollEvents();
}

void SetIsMouseMotionRaw(Window window, bool state)
{
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, rawMouseMotionSupported && state);
}

bool WindowShouldClose(Window window)
{
    return glfwWindowShouldClose(window);
}

MHArray<const char*> GetRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    return MHArray<const char*>(glfwExtensions, glfwExtensionCount);
}

void FreeWindow(Window window)
{
    glfwDestroyWindow(window);
}

void FreeGlfw()
{
    glfwTerminate();
}

} // namespace Siege::Glfw