//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_INPUT_H
#define SIEGE_ENGINE_INPUT_H

#include <map>

#include "Window.h"

#define KEY_W 87
#define KEY_A 65
#define KEY_S 83
#define KEY_D 68
#define KEY_Q 81
#define KEY_E 69
#define KEY_UP 265
#define KEY_DOWN 264
#define KEY_RIGHT 262
#define KEY_LEFT 263
#define KEY_BACKTICK 96
#define KEY_LEFT_SHIFT 340

#define KEY_ESCAPE 256

namespace Siege
{
class Input
{
public:

    struct MouseCoordinates
    {
        double x;
        double y;
    };
    static void SetWindowPointer(Window* window);
    static bool IsKeyDown(int key);
    static bool IsKeyJustPressed(int key);

    static const MouseCoordinates& GetCursorPosition();
    static MouseCoordinates GetNormalisedMousePosition();
};
} // namespace Siege

#endif