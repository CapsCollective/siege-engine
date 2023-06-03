//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_GLFW_INPUT_H
#define SIEGE_ENGINE_GLFW_INPUT_H

#include "Types.h"
#include "window/utils/Types.h"

namespace Siege::Glfw
{
typedef void (*setKeyPressedCallback)(Window, int key, int scancode, int action, int mods);
typedef setKeyPressedCallback SetKeyPressedCallback;
typedef void (*setTextKeyPressedCallback)(Window, unsigned int key);
typedef setTextKeyPressedCallback SetTextKeyPressedCallback;

bool IsKeyDown(Window window, int keyCode);
MousePosition GetMousePosition(Window window);
void SetOnKeyPressedCallback(Window window, SetKeyPressedCallback callback);
void SetOnTextKeyPressedCallback(Window window, SetTextKeyPressedCallback callback);
} // namespace Siege::Glfw

#endif // SIEGE_ENGINE_GLFW_INPUT_H
