//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Window.h"

#include <chrono>
#include <raylib/Window.hpp>

#include "../TransitionAdapter.h"

namespace Siege
{
static String windowTitle;
static int windowWidth;
static int windowHeight;

float Window::deltaTime;

static raylib::Window& GetWindow()
{
    static raylib::Window window(windowWidth, windowHeight, windowTitle.Str());
    return window;
}

Window::Window(const String& title, int width, int height) : backgroundColour(245, 245, 245)
{
    windowTitle = title;
    windowWidth = width;
    windowHeight = height;
    GetWindow().SetTargetFPS(60);
    SetExitKey(-1); // Deactivate the exit key
}

bool Window::ShouldClose()
{
    return GetWindow().ShouldClose();
}

void Window::BeginDraw()
{
    GetWindow().BeginDrawing();
    ClearBackground(FromColour(backgroundColour));
}

void Window::EndDraw()
{
    GetWindow().EndDrawing();
}

int Window::GetWidth()
{
    return windowWidth;
}

int Window::GetHeight()
{
    return windowHeight;
}

int Window::GetTextWidth(const String& text, int textSize)
{
    return MeasureText(text, textSize);
}

long Window::GetCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    auto seconds = std::chrono::time_point_cast<std::chrono::microseconds>(now);
    return seconds.time_since_epoch().count();
}

void Window::UpdateTime()
{
    static long previousTime = GetCurrentTime();
    long currentTime = GetCurrentTime();
    deltaTime = (float) (currentTime - previousTime) / 1000000.f;
    previousTime = currentTime;
}

float Window::GetDeltaTime()
{
    return deltaTime;
}
}