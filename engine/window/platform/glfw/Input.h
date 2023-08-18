//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_GLFW_INPUT_H
#define SIEGE_ENGINE_GLFW_INPUT_H

#include <utils/math/vec/Vec2.h>

#include "Types.h"
#include "window/utils/Types.h"

namespace Siege::Glfw
{
typedef void (*setKeyPressedCallback)(Window, int key, int scancode, int action, int mods);
typedef setKeyPressedCallback SetKeyPressedCallback;
typedef void (*setTextKeyPressedCallback)(Window, unsigned int key);
typedef setTextKeyPressedCallback SetTextKeyPressedCallback;
typedef void (*setMousePressedCallback)(Window, int button, int action, int mods);
typedef setMousePressedCallback SetMouseButtonPressedCallback;
typedef void (*setMousePositionCallback)(Window, double x, double y);
typedef setMousePositionCallback SetMousePositionCallback;

bool IsKeyDown(Window window, int keyCode);
bool IsKeyReleased(Window window, int keyCode);
bool IsMouseButtonDown(Window window, int buttonCode);
bool IsMouseButtonReleased(Window window, int buttonCode);
void SetOnKeyPressedCallback(Window window, SetKeyPressedCallback callback);
void SetOnTextKeyPressedCallback(Window window, SetTextKeyPressedCallback callback);
void SetOnMouseKeyPressedCallback(Window window, SetMouseButtonPressedCallback callback);
void SetOnMouseMovedCallback(Window window, SetMousePositionCallback callback);
void SetIsCursorVisible(Window window, bool isCursorVisible);
} // namespace Siege::Glfw

#endif // SIEGE_ENGINE_GLFW_INPUT_H
