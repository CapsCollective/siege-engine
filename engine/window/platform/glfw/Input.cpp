#include "Input.h"

#include <GLFW/glfw3.h>

namespace Siege::Glfw
{
bool IsKeyDown(Window window, int keyCode)
{
    return glfwGetKey(window, keyCode) == GLFW_PRESS;
}

MousePosition GetMousePosition(Window window)
{
    MousePosition position = {};
    glfwGetCursorPos(window, &position.x, &position.y);
    return position;
}

void SetOnKeyPressedCallback(Window window, SetKeyPressedCallback callback)
{
    glfwSetKeyCallback(window, callback);
}

void SetOnTextKeyPressedCallback(Window window, SetTextKeyPressedCallback callback)
{
    glfwSetCharCallback(window, callback);
}

} // namespace Siege::Glfw