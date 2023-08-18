//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_INPUT_H
#define SIEGE_ENGINE_INPUT_H

#include <utils/collections/HeapArray.h>
#include <utils/math/vec/Vec2.h>

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
    static bool IsKeyReleased(int key);
    static bool IsKeyJustPressed(int key);
    static bool IsMouseButtonDown(int button);
    static bool IsMouseButtonReleased(int button);
    static bool IsMouseButtonJustPressed(int button);

    static const Vec2 GetCursorPosition();
    static const Vec2 GetMouseDirection();
    static int GetLatestKey();
    static int GetLatestChar();
    static void DisableCursor();
    static void EnableCursor();
    static void ToggleCursor(bool state);

private:

    static int latestKey;
    static int latestChar;
    static bool keyUpdated;
    static bool charUpdated;
    static Glfw::Window primaryWindow;
    static std::map<int, int> keyMap;
    static Vec2 lastMousePos;
    static Vec2 mouseMoveDelta;
    static bool isFirstMouseCall;
};
} // namespace Siege

#endif