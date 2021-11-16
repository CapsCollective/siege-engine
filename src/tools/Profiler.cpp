#include "Profiler.h"
#include <utils/Maths.h>

void Profiler::OnDraw2D()
{
    if (!isActive) return;

    DrawRectangle(GetScreenWidth() - 155, 0, 155, 45, Colour::Black);
    DrawText(("FPS: " + std::to_string(GetFPS()) + "\n" +
                   "Frame Time: " + std::to_string(GetFrameTime())).c_str(),
             GetScreenWidth() - 150, 5, 15, Colour::White);
}

void Profiler::ToggleActive()
{
    isActive = !isActive;
}
