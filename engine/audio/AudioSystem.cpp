//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "AudioSystem.h"

#include <utils/Logging.h>
#include <miniaudio.c>

namespace Siege
{

struct SoundInstance
{
    ma_decoder_config decoderConfig;
    ma_decoder decoder;
    ma_sound sound;
    uint64_t id = -1;
    float killTicks  = -1;
};

static ma_engine audioEngine;

uint64_t globalIdx = 0;
std::mutex mutex;
static std::vector<SoundInstance> playingSounds;

AudioSystem& AudioSystem::GetInstance()
{
    static AudioSystem audioSystem;
    return audioSystem;
}

bool AudioSystem::Initialise()
{
    playingSounds.reserve(100);
    ma_result result = ma_engine_init(nullptr, &audioEngine);
    return result == MA_SUCCESS;
}

bool AudioSystem::PlayClip(const String& path) const
{
    ma_result result = ma_engine_play_sound(&audioEngine, path, nullptr);
    return result == MA_SUCCESS;
}

bool AudioSystem::PlayClip(const PackFileData& data) const
{
    ma_result result;
    mutex.lock();
    SoundInstance& instance = playingSounds.emplace_back();

    instance.decoderConfig = ma_decoder_config_init(ma_format_f32, 2, 48000);
    instance.decoderConfig.encodingFormat = ma_encoding_format_wav;
    result = ma_decoder_init_memory(data.data, data.dataSize, &instance.decoderConfig, &instance.decoder);
    if (result != MA_SUCCESS)
    {
        CC_LOG_ERROR("Decoder init error!")
    }

    result = ma_sound_init_from_data_source(&audioEngine, &instance.decoder, 0, nullptr, &instance.sound);
    if (result != MA_SUCCESS)
    {
        CC_LOG_ERROR("Sound init error!")
    }

    instance.id = globalIdx++;
    instance.killTicks = 120;

    result = ma_sound_start(&instance.sound);
    if (result != MA_SUCCESS)
    {
        CC_LOG_ERROR("Sound start error!")
    }
    mutex.unlock();

    CC_LOG_ERROR("Played {}!", instance.id)
    return result == MA_SUCCESS;
}

void AudioSystem::Update()
{
    mutex.lock();
    for (auto it = playingSounds.begin(); it != playingSounds.end();)
    {
        --it->killTicks;
        if (it->killTicks > 0)
        {
            ++it;
            continue;
        }

        uint64_t id = it->id;

        ma_sound_uninit(&it->sound);
        ma_result result = ma_decoder_uninit(&it->decoder);
        if (result != MA_SUCCESS)
        {
            CC_LOG_ERROR("Decoder uninit error!")
        }

        std::memset(&*it, 0, sizeof(SoundInstance));
        it = playingSounds.erase(it);
        CC_LOG_WARNING("Erased {}!", id)
    }
    mutex.unlock();
}

void AudioSystem::Uninitialise()
{
    mutex.lock();
    for (auto it = playingSounds.begin(); it != playingSounds.end();)
    {
        it->killTicks = 0;
    }
    mutex.unlock();
    Update();

    ma_engine_uninit(&audioEngine);
}

} // namespace Siege