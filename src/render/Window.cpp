#include "Window.h"

#include <chrono>
#include <raylib/Window.hpp>

#include "../utils/TransitionAdapter.h"

static std::string windowTitle;
static int windowWidth;
static int windowHeight;

float Window::deltaTime;

static raylib::Window& GetWindow()
{
    static raylib::Window window(windowWidth, windowHeight, windowTitle);
    return window;
}

Window::Window(const std::string& title, int width, int height) : backgroundColour(245, 245, 245)
{
    // TODO fix this ugly workaround
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

int Window::GetTextWidth(const std::string& text, int textSize)
{
    return MeasureText(text.c_str(), textSize);
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
