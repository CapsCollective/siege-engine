#ifndef A_DARK_DISCOMFORT_WINDOW_H
#define A_DARK_DISCOMFORT_WINDOW_H

#include <raylib/Window.hpp>
#include <raylib/Color.hpp>

class Window
{
public:

    // 'Structors

    Window(const std::string& title, int width, int height) :
        backgroundColour(RAYWHITE),
        window(width, height, title)
    {
        window.SetTargetFPS(60);
        SetExitKey(-1); // Deactivate the exit key
    }

    // Public methods

    bool ShouldClose()
    {
        return window.ShouldClose();
    }

    void BeginDraw()
    {
        window.BeginDrawing();
        backgroundColour.ClearBackground();
    }

    void EndDraw()
    {
        window.EndDrawing();
    }

private:

    // Private fields

    raylib::Color backgroundColour;

    raylib::Window window;

};


#endif //A_DARK_DISCOMFORT_WINDOW_H
