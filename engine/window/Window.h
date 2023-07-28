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

#include "window/utils/Types.h"

namespace Siege
{

class Window
{
public:

    static MHArray<const char*> GetRequiredExtensions();

    // 'Structors

    Window(char const* name, WindowExtents extents);

    Window() : Window("Window", {800, 600}) {}

    ~Window();

    // Public Getters

    int GetHeight() const;

    int GetWidth() const;

    bool IsVisible() const;

    unsigned int GetDPI() const;

    WindowExtents GetExtents() const;

    void* GetRawWindow();

    // Public Functions

    void Update();

    bool WindowShouldClose();

    bool WasResized() const;

    void ResetWindowResized();

    void WaitEvents();

    inline float AspectRatio()
    {
        return static_cast<float>(extents.width) / static_cast<float>(extents.height);
    }

private:

    // Private variables

    void* window {nullptr};

    WindowExtents extents;
    MonitorPixelScale DPI {};
    bool wasResized {false};
    bool isVisible {true};
};
} // namespace Siege

#endif
