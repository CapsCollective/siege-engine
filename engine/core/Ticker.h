//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_TICKER_H
#define SIEGE_ENGINE_TICKER_H

namespace Siege
{
class Ticker
{
public:

    // 'Structors

    explicit Ticker(float initialDeltaTime = 0.f) : deltaTime(initialDeltaTime) {}

    // Public methods

    static long GetCurrentTime();

    void UpdateTime();

    float GetDeltaTime() const;

private:

    // Private fields

    float deltaTime;
};
} // namespace Siege

#endif // SIEGE_ENGINE_TICKER_H
