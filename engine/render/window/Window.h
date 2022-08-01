//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDERER_WINDOW_H
#define SIEGE_ENGINE_RENDERER_WINDOW_H

#include <volk/volk.h>

#include <cstdint>

// Include Vulkan headers through GLFW
#include <GLFW/glfw3.h>
#include <utils/collections/HeapArray.h>

namespace Siege
{
class Window
{
public:

    // 'Structors

    Window(char const* name, int width, int height) : width(width), height(height)
    {
        if (!glfwInitialised)
        {
            glfwInit();
            glfwInitialised = true;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        if (!window) window = glfwCreateWindow(width, height, name, nullptr, nullptr);

        if (glfwRawMouseMotionSupported())
        {
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }

        glfwSetWindowUserPointer(window, this);
        glfwSetWindowSizeCallback(window, ResizeCallback);
        glfwWindows++;
    }

    Window() : Window("Window", 800, 600) {}

    ~Window()
    {
        glfwDestroyWindow(window);
        if (--glfwWindows <= 0) glfwTerminate();
    }

    // Public Getters

    const int& GetHeight() const
    {
        return height;
    }

    const int& GetWidth() const
    {
        return width;
    }

    VkExtent2D GetExtent() const
    {
        return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
    }

    GLFWwindow* GetGlfwWindow()
    {
        return window;
    }

    // Public Functions

    void Update();

    bool WindowShouldClose();

    static bool CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

    bool WasResized() const
    {
        return wasResized;
    }

    void ResetWindowResized()
    {
        wasResized = false;
    }

    void EnableCursor()
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void DisableCursor()
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    static Utils::MHArray<const char*> GetRequiredExtensions();

    void ToggleCursor(bool state)
    {
        state ? DisableCursor() : EnableCursor();
    }

    void WaitEvents()
    {
        glfwWaitEvents();
    }

private:

    static void ResizeCallback(GLFWwindow* windowPtr, int width, int height);

    // Private static variables

    static bool glfwInitialised;
    static size_t glfwWindows;

    // Private variables

    static GLFWwindow* window;

    int width;
    int height;
    bool wasResized = false;
};
} // namespace Siege

#endif