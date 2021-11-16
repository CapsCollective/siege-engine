#include "Window.h"
#include <raylib/Window.hpp>
#include "../utils/TransitionAdapter.h"

static std::string windowTitle;
static int windowWidth;
static int windowHeight;

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
