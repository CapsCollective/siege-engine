//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDERER_WINDOW_H
#define SIEGE_ENGINE_RENDERER_WINDOW_H

#include <cstdint>

#include <GLFW/glfw3.h>
#include <utils/collections/HeapArray.h>

namespace Siege
{


struct WindowExtents
{
    uint32_t width {0};
    uint32_t height {0};
};

class Window
{
public:

    // 'Structors

    Window(char const* name, WindowExtents extents) : extents(extents)
    {
        if (!glfwInitialised)
        {
            glfwInit();
            glfwInitialised = true;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        if (!window) window = glfwCreateWindow(extents.width, extents.height, name, nullptr, nullptr);

        if (glfwRawMouseMotionSupported())
        {
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }

        glfwSetWindowIconifyCallback(window, [](GLFWwindow* window, int iconified) {
            auto windowContext = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
            windowContext->isVisible = !iconified;
        });

        glfwSetWindowUserPointer(window, this);
        glfwSetWindowSizeCallback(window, ResizeCallback);
        glfwWindows++;

        glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &DPI.width, &DPI.height);
    }

    Window() : Window("Window", {800, 600}) {}

    ~Window()
    {
        glfwDestroyWindow(window);
        if (--glfwWindows <= 0) glfwTerminate();
    }

    // Public Getters

    const int GetHeight() const
    {
        return extents.height;
    }

    const int GetWidth() const
    {
        return extents.width;
    }

    const bool IsVisible() const
    {
        return isVisible;
    }

    const uint32_t GetDPI() const
    {
        // NOTE(Aryeh): Need to find a better solution when the DPI in different axes are not the
        // same
        return DPI.width;
    }

    const uint32_t GetScaledWidth() const
    {
        return extents.width * DPI.width;
    }

    const uint32_t GetScaledHeight() const
    {
        return extents.height * DPI.height;
    }

    WindowExtents GetExtents() const
    {
        return extents;
    }

    GLFWwindow* GetGlfwWindow()
    {
        return window;
    }

    // Public Functions

    void Update();

    bool WindowShouldClose();

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

    static MHArray<const char*> GetRequiredExtensions();

    void ToggleCursor(bool state)
    {
        state ? DisableCursor() : EnableCursor();
    }

    void WaitEvents()
    {
        glfwWaitEvents();
    }

private:

    struct MonitorPixelScale
    {
        float width {0};
        float height {0};
    };

    static void ResizeCallback(GLFWwindow* windowPtr, int width, int height);

    // Private static variables

    static bool glfwInitialised;
    static size_t glfwWindows;

    // Private variables

    static GLFWwindow* window;

    WindowExtents extents;

    MonitorPixelScale DPI {};
    bool wasResized {false};
    bool isVisible {true};
};
} // namespace Siege

#endif
