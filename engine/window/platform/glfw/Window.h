//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_GLFW_WINDOW_H
#define SIEGE_ENGINE_GLFW_WINDOW_H

#include <utils/collections/HeapArray.h>

#include "Types.h"
#include "window/utils/Types.h"

namespace Siege::Glfw
{
typedef void (*isMinimisedCallback)(Window, int);
typedef isMinimisedCallback IsMinimisedCallback;
typedef void (*isResizedCallback)(Window, int, int);
typedef isResizedCallback IsResizedCallback;

static bool glfwInitialised {false};
static bool rawMouseMotionSupported {true};

bool Initialize();
Window CreateWindow(const char* title,
                    unsigned int width,
                    unsigned int height,
                    void* windowBase,
                    bool isResizable = true);
void SetWindowMinimisedCallback(Window window, IsMinimisedCallback callback);
void SetWindowResizedCallback(Window window, IsResizedCallback callback);
void* GetUserPtr(Window window);

void WaitEvents();
void GetEvents();

MonitorPixelScale GetWindowDPI();

void SetIsMouseMotionRaw(Window window, bool state);
bool WindowShouldClose(Window window);

MHArray<const char*> GetRequiredExtensions();

void FreeWindow(Window window);
void FreeGlfw();

} // namespace Siege::Glfw

#endif // SIEGE_ENGINE_GLFW_WINDOW_H
