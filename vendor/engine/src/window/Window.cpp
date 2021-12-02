#include "Window.h"
#include <raylib/Window.hpp>
#include "../utils/TransitionAdapter.h"

static std::string windowTitle;
static int windowWidth;
static int windowHeight;

double Window::deltaTime;

static raylib::Window& GetWindow()
{
    static raylib::Window window(windowWidth, windowHeight, windowTitle);
    return window;
}

Window::Window(const std::string& title, int width, int height) :
        backgroundColour(245, 245, 245)
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

long Window::GetCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    return ms.time_since_epoch().count();
}

void Window::UpdateTime()
{
    static auto previousTime = (double) GetCurrentTime();
    auto currentTime = (double) GetCurrentTime();
    deltaTime = currentTime - previousTime;
    previousTime = currentTime;
}

float Window::GetDeltaTime()
{
    return (float) deltaTime;
}
