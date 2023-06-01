//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDERER_WINDOW_H
#define SIEGE_ENGINE_RENDERER_WINDOW_H

#include <utils/collections/HeapArray.h>

#include <cstdint>

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

    Window(char const* name, WindowExtents extents);

    Window() : Window("Window", {800, 600}) {}

    ~Window();

    // Public Getters

    int GetHeight() const;

    int GetWidth() const;

    bool IsVisible() const;

    uint32_t GetDPI() const;

    uint32_t GetScaledWidth() const;

    uint32_t GetScaledHeight() const;

    WindowExtents GetExtents() const;

    void* GetWindow();

    // Public Functions

    void Update();

    bool WindowShouldClose();

    void ResizeWindow(WindowExtents newExtents);

    bool WasResized() const;

    void ResetWindowResized();

    void EnableCursor();

    void DisableCursor();

    static MHArray<const char*> GetRequiredExtensions();

    void ToggleCursor(bool state);

    void WaitEvents();

private:

    struct MonitorPixelScale
    {
        float width {0};
        float height {0};
    };

    // Private variables

    WindowExtents extents;

    MonitorPixelScale DPI {};

    bool wasResized {false};

    bool isVisible {true};
};
} // namespace Siege

#endif
