//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "AudioSystem.h"

#include <miniaudio.c>

namespace Siege
{

static ma_engine audioEngine;

AudioSystem& AudioSystem::GetInstance()
{
    static AudioSystem audioSystem;
    return audioSystem;
}

bool AudioSystem::Initialise()
{

    ma_result result = ma_engine_init(nullptr, &audioEngine);
    return result == MA_SUCCESS;
}

bool AudioSystem::PlayClip(const String& path) const
{
    ma_result result = ma_engine_play_sound(&audioEngine, path, nullptr);
    return result == MA_SUCCESS;
}

void AudioSystem::Uninitialise()
{
    ma_engine_uninit(&audioEngine);
}

} // namespace Siege