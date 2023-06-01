#ifndef SIEGE_ENGINE_WINDOW_TYPES_H
#define SIEGE_ENGINE_WINDOW_TYPES_H

#include <cstdint>

namespace Siege
{
struct WindowExtents
{
    uint32_t width {0};
    uint32_t height {0};
};

struct MonitorPixelScale
{
    float width {0};
    float height {0};
};

struct MousePosition
{
    double x {0.f};
    double y {0.f};
};
} // namespace Siege

#endif // SIEGE_ENGINE_WINDOW_TYPES_H
