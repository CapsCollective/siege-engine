//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Ticker.h"

#include <chrono>

namespace Siege
{
long Ticker::GetCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    auto seconds = std::chrono::time_point_cast<std::chrono::microseconds>(now);
    return seconds.time_since_epoch().count();
}

void Ticker::UpdateTime()
{
    static long previousTime = GetCurrentTime();
    long currentTime = GetCurrentTime();
    deltaTime = (float) (currentTime - previousTime) / 1000000.f;
    previousTime = currentTime;
}

float Ticker::GetDeltaTime() const
{
    return deltaTime;
}
} // namespace Siege