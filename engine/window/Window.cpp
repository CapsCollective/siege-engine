//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Window.h"

#include "window/platform/glfw/Window.h"

static Siege::Glfw::Window AsGlfwWindow(void* window)
{
    return reinterpret_cast<Siege::Glfw::Window>(window);
}

namespace Siege
{
Window::Window(const char* name, WindowExtents extents) : extents(extents)
{
    Glfw::Initialize();

    if (!window) window = Glfw::CreateWindow(name, extents.width, extents.height, this);

    Glfw::SetIsMouseMotionRaw(AsGlfwWindow(window), true);

    Glfw::SetWindowMinimisedCallback(
        AsGlfwWindow(window),
        [](Glfw::Window pWindow, int isMinimised) {
            reinterpret_cast<Window*>(Glfw::GetUserPtr(pWindow))->isVisible = !isMinimised;
        });

    Glfw::SetWindowResizedCallback(
        AsGlfwWindow(window),
        [](Glfw::Window pWindow, int width, int height) {
            auto windowContext = reinterpret_cast<Window*>(Glfw::GetUserPtr(pWindow));
            windowContext->wasResized = true;
            windowContext->extents = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
        });

    DPI = Glfw::GetWindowDPI();
}

Window::~Window()
{
    Glfw::FreeWindow(AsGlfwWindow(window));
    Glfw::FreeGlfw();
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

unsigned int Window::GetDPI() const
{
    // NOTE(Aryeh): Need to find a better solution when the DPI in different axes are not the
    // same
    return DPI.width;
}

WindowExtents Window::GetExtents() const
{
    return extents;
}

void* Window::GetRawWindow()
{
    return window;
}

void Window::Update()
{
    Glfw::GetEvents();
}

bool Window::WindowShouldClose()
{
    return Glfw::WindowShouldClose(AsGlfwWindow(window));
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
    Glfw::SetIsCursorVisible(AsGlfwWindow(window), true);
}

void Window::DisableCursor()
{
    Glfw::SetIsCursorVisible(AsGlfwWindow(window), false);
}

MHArray<const char*> Window::GetRequiredExtensions()
{
    return Glfw::GetRequiredExtensions();
}

void* Window::GetWindowSurface(Window& window, void* instance)
{
    return Glfw::GetWindowSurface((Glfw::Window) window.GetRawWindow(), instance);
}

void Window::ToggleCursor(bool state)
{
    state ? DisableCursor() : EnableCursor();
}

void Window::WaitEvents()
{
    Glfw::WaitEvents();
}
} // namespace Siege