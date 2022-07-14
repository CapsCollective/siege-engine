//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_WINDOW_H
#define SIEGE_ENGINE_WINDOW_H

#include <utils/Colour.h>
#include <utils/String.h>

namespace Siege
{
class Window
{
public:

    // 'Structors

    Window(const String& title, int width, int height);

    // Public methods

    bool ShouldClose();

    void BeginDraw();

    void EndDraw();

    int GetWidth();

    int GetHeight();

    static int GetTextWidth(const String& text, int textSize);

    static long GetCurrentTime();

    static void UpdateTime();

    static float GetDeltaTime();

private:

    // Private fields

    Colour backgroundColour;

    static float deltaTime;
};
}

#endif // SIEGE_ENGINE_WINDOW_H
