//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDERER_WINDOW_H
#define SIEGE_ENGINE_RENDERER_WINDOW_H

#include "window/platform/glfw/Window.h"

namespace Siege
{

class Window
{
public:

    // 'Structors

    Window(char const* name, WindowExtents extents) : extents(extents)
    {
        Glfw::Initialize();

        if (!window) window = Glfw::CreateWindow(name, extents.width, extents.height, this);

        Glfw::SetIsMouseMotionRaw(window, true);

        Glfw::SetWindowMinimisedCallback(window, [](Glfw::Window pWindow, int isMinimised) {
            reinterpret_cast<Window*>(Glfw::GetUserPtr(pWindow))->isVisible = !isMinimised;
        });

        Glfw::SetWindowResizedCallback(window, [](Glfw::Window pWindow, int width, int height) {
            auto windowContext = reinterpret_cast<Window*>(Glfw::GetUserPtr(pWindow));
            windowContext->wasResized = true;
            windowContext->extents = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
        });

        DPI = Glfw::GetWindowDPI();
    }

    Window() : Window("Window", {800, 600}) {}

    ~Window()
    {
        Glfw::FreeWindow(window);
        Glfw::FreeGlfw();
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

    const unsigned int GetDPI() const
    {
        // NOTE(Aryeh): Need to find a better solution when the DPI in different axes are not the
        // same
        return DPI.width;
    }

    WindowExtents GetExtents() const
    {
        return extents;
    }

    Glfw::Window GetRawWindow()
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
        Glfw::SetIsCursorVisible(window, true);
    }

    void DisableCursor()
    {
        Glfw::SetIsCursorVisible(window, false);
    }

    static MHArray<const char*> GetRequiredExtensions();

    void ToggleCursor(bool state)
    {
        state ? DisableCursor() : EnableCursor();
    }

    void WaitEvents()
    {
        Glfw::WaitEvents();
    }

private:

    // Private variables

    Glfw::Window window {nullptr};

    WindowExtents extents;
    MonitorPixelScale DPI {};
    bool wasResized {false};
    bool isVisible {true};
};
} // namespace Siege

#endif
