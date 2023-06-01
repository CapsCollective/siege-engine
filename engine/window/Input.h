//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_INPUT_H
#define SIEGE_ENGINE_INPUT_H

#include <utils/collections/HeapArray.h>

#include <map>

#include "Key.h"
#include "window/platform/glfw/Types.h"
#include "window/utils/Types.h"

namespace Siege
{
class Input
{
public:

    static void SetInputWindowSource(Glfw::Window window);
    static bool IsKeyDown(int key);
    static bool IsKeyJustPressed(int key);

    static const MousePosition GetCursorPosition();
    static int GetLatestKey();
    static int GetLatestChar();

private:

    static int latestKey;
    static int latestChar;
    static bool keyUpdated;
    static bool charUpdated;
    static Glfw::Window primaryWindow;
    static std::map<int, int> keyMap;
};
} // namespace Siege

#endif