#ifndef A_DARK_DISCOMFORT_WINDOW_H
#define A_DARK_DISCOMFORT_WINDOW_H

#include "../utils/Maths.h"

class Window
{
public:

    // 'Structors

    Window(const std::string& title, int width, int height);

    // Public methods

    bool ShouldClose();

    void BeginDraw();

    void EndDraw();

    int GetWidth();

    int GetHeight();


    static long GetCurrentTime();

    static void UpdateTime();

    static float GetDeltaTime();

private:

    // Private fields

    Colour backgroundColour;

    static double deltaTime;

};


#endif //A_DARK_DISCOMFORT_WINDOW_H
