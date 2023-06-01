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
void Window::Update()
{
    Glfw::GetEvents();
}

bool Window::WindowShouldClose()
{
    return Glfw::WindowShouldClose(window);
}

MHArray<const char*> Window::GetRequiredExtensions()
{
    return Glfw::GetRequiredExtensions();
}
} // namespace Siege