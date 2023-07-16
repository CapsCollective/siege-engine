//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Input.h"

#include "window/platform/glfw/Input.h"

namespace Siege
{
Glfw::Window Input::primaryWindow = nullptr;
int Input::latestKey {-1};
int Input::latestChar {-1};
bool Input::keyUpdated {false};
bool Input::charUpdated {false};
std::map<int, int> Input::keyMap;
Vec2 Input::lastMousePos {Vec2::Zero()};
Vec2 Input::mouseMoveDelta {Vec2::Zero()};
bool Input::isFirstMouseCall {true};

void Input::SetInputWindowSource(Glfw::Window window)
{
    primaryWindow = window;

    using namespace Glfw;

    SetOnTextKeyPressedCallback(window, [](Window, unsigned int key) {
        latestChar = key;
        charUpdated = true;
    });

    SetOnKeyPressedCallback(window, [](Window, int key, int scancode, int action, int mod) {
        latestKey = ((action == ACTION_PRESSED || action == ACTION_REPEAT) * key) +
                    (action == ACTION_RELEASED * -1);
        keyUpdated = true;
    });

    SetOnMouseKeyPressedCallback(window, [](Window, int button, int action, int mods) {
        // TODO: Set logic here
    });

    SetOnMouseMovedCallback(window, [](Window, double x, double y) {
        if (isFirstMouseCall)
        {
            lastMousePos = {(float) x, (float) y};
            mouseMoveDelta = {0.f, 0.f};
            isFirstMouseCall = false;
            return;
        }

        mouseMoveDelta = {(lastMousePos.x - (float) x), ((float) y - lastMousePos.y)};
        lastMousePos = {(float) x, (float) y};
    });
}

bool Input::IsKeyDown(int key)
{
    bool hasKey = Glfw::IsKeyDown(primaryWindow, key);

    if (keyMap.find(key) != keyMap.end())
    {
        keyMap[key] = hasKey ? 1 : 0;
    }
    else
    {
        if (hasKey) keyMap[key] = 1;
    }
    return hasKey;
}

bool Input::IsKeyJustPressed(int key)
{
    bool hasKey = Glfw::IsKeyDown(primaryWindow, key);

    bool keyEntryExists = keyMap.find(key) != keyMap.end();
    if (keyEntryExists && hasKey)
    {
        if (keyMap[key] == 0)
        {
            keyMap[key] = 1;
            return true;
        }
        else return false;
    }
    else if (hasKey && !keyEntryExists)
    {
        keyMap[key] = 1;
        return true;
    }
    else if (!hasKey && keyEntryExists) keyMap[key] = 0;

    return false;
}

bool Input::IsMouseButtonDown(int button)
{
    bool hasButton = Glfw::IsMouseButtonDown(primaryWindow, button);

    if (keyMap.find(button) != keyMap.end())
    {
        keyMap[button] = hasButton ? 1 : 0;
    }
    else
    {
        if (hasButton) keyMap[button] = 1;
    }
    return hasButton;
}

bool Input::IsMouseButtonJustPressed(int button)
{
    bool hasButton = Glfw::IsMouseButtonDown(primaryWindow, button);

    bool keyEntryExists = keyMap.find(button) != keyMap.end();
    if (keyEntryExists && hasButton)
    {
        if (keyMap[button] == 0)
        {
            keyMap[button] = 1;
            return true;
        }
        else return false;
    }
    else if (hasButton && !keyEntryExists)
    {
        keyMap[button] = 1;
        return true;
    }
    else if (!hasButton && keyEntryExists) keyMap[button] = 0;

    return false;
}

const Vec2 Input::GetCursorPosition()
{
    return lastMousePos;
}

const Vec2 Input::GetMouseDirection()
{
    auto oldMouseDir = mouseMoveDelta;
    mouseMoveDelta = Vec2::Zero();
    return oldMouseDir;
};

int Input::GetLatestKey()
{
    auto key = (keyUpdated * latestKey) + (!keyUpdated * -1);
    keyUpdated = false;
    return key;
}

int Input::GetLatestChar()
{
    auto key = (charUpdated * latestChar) + (!charUpdated * -1);
    charUpdated = false;
    return key;
}
} // namespace Siege
