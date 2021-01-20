#include "Profiler.h"

void Profiler::OnDraw()
{
    if (!isActive) return;

    DrawRectangle(GetScreenWidth() - 155, 0, 155, 45,
                  ColorAlpha(BLACK, 0.5f));
    DrawText(("FPS: " + std::to_string(GetFPS()) + "\n" +
                   "Frame Time: " + std::to_string(GetFrameTime())).c_str(),
             GetScreenWidth() - 150, 5, 15, WHITE);
}

void Profiler::ToggleActive()
{
    isActive = !isActive;
}
