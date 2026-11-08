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
#include <mutex>

#define MA_NO_RESOURCE_MANAGER

#define AUDIO_SYSTEM_MAX_VOICES 16

namespace Siege
{

struct Voice
{
    uint64_t id = 0;
    bool pendingKill = false;
    ma_decoder decoder = {};
    ma_sound sound = {};
};

static ma_engine audioEngine;
static ma_decoder_config decoderConfig;

static Voice voices[AUDIO_SYSTEM_MAX_VOICES];
static std::mutex voicesMutex;

AudioSystem& AudioSystem::GetInstance()
{
    static AudioSystem audioSystem;
    return audioSystem;
}

bool AudioSystem::Initialise()
{
    decoderConfig = ma_decoder_config_init(ma_format_f32, 2, 48000);
    decoderConfig.encodingFormat = ma_encoding_format_wav;

    ma_result result = ma_engine_init(nullptr, &audioEngine);
    return result == MA_SUCCESS;
}

static int32_t FindAvailableVoice()
{
    for (int32_t i = 0; i < AUDIO_SYSTEM_MAX_VOICES; ++i)
    {
        const Voice& voice = voices[i];
        uint64_t id = voice.id;
        if (id == 0)
        {
            return i;
        }
    }
    return -1;
}

static void OnSoundEnded(void* userData, ma_sound* sound)
{
    voicesMutex.lock();
    uint64_t voiceIdx = reinterpret_cast<uint64_t>(userData);
    Voice& voice = voices[voiceIdx];
    voice.pendingKill = true;
    CC_LOG_INFO("Marked pending kill sound clip with id {} on voice idx {}", voice.id, voiceIdx)
    voicesMutex.unlock();
}

bool AudioSystem::PlayClip(const PackFileData& data) const
{
    voicesMutex.lock();
    int32_t voiceIdx = FindAvailableVoice();
    if (voiceIdx == -1)
    {
        CC_LOG_ERROR("Failed to find available voice for requested clip at address {}", reinterpret_cast<uint64_t>(&data));
        return false;
    }

    Voice& voice = voices[voiceIdx];

    ma_result result = ma_decoder_init_memory(data.data, data.dataSize, &decoderConfig, &voice.decoder);
    if (result != MA_SUCCESS)
    {
        CC_LOG_ERROR("Failed to initialise decoder for requested clip at address {}", reinterpret_cast<uint64_t>(&data))
        return false;
    }

    result = ma_sound_init_from_data_source(&audioEngine, &voice.decoder, 0, nullptr, &voice.sound);
    if (result != MA_SUCCESS)
    {
        CC_LOG_ERROR("Failed to initialise sound for requested clip at address {}", reinterpret_cast<uint64_t>(&data))
        result = ma_decoder_uninit(&voice.decoder);
        if (result != MA_SUCCESS)
        {
            CC_LOG_ERROR("Failed to uninitialise decoder after bad sound initialisation")
        }
        return false;
    }

    static uint64_t globalId = 0;
    voice.id = ++globalId;

    result = ma_sound_set_end_callback(&voice.sound, OnSoundEnded, reinterpret_cast<void*>(voiceIdx));
    if (result != MA_SUCCESS)
    {
        CC_LOG_ERROR("Failed to set sound ended callback for requested clip at address {}", reinterpret_cast<uint64_t>(&data))
        ma_sound_uninit(&voice.sound);
        result = ma_decoder_uninit(&voice.decoder);
        if (result != MA_SUCCESS)
        {
            CC_LOG_ERROR("Failed to uninitialise decoder after failure to set callback")
        }
        return false;
    }

    result = ma_sound_start(&voice.sound);
    if (result != MA_SUCCESS)
    {
        CC_LOG_ERROR("Failed to start sound for requested clip at address {}", reinterpret_cast<uint64_t>(&data))
        ma_sound_uninit(&voice.sound);
        result = ma_decoder_uninit(&voice.decoder);
        if (result != MA_SUCCESS)
        {
            CC_LOG_ERROR("Failed to uninitialise decoder after sound failed to start")
        }
        return false;
    }
    voicesMutex.unlock();

    CC_LOG_INFO("Started playing sound clip at address {} with id {} on voice idx {}", reinterpret_cast<uint64_t>(&data), voice.id, voiceIdx)
    return true;
}

void AudioSystem::Update()
{
    voicesMutex.lock();
    for (int32_t i = 0; i < AUDIO_SYSTEM_MAX_VOICES; ++i)
    {
        Voice& voice = voices[i];
        uint64_t id = voice.id;
        if (id == 0)
        {
            continue;
        }

        if (!voice.pendingKill)
        {
            continue;
        }

        ma_sound_uninit(&voice.sound);
        ma_result result = ma_decoder_uninit(&voice.decoder);
        if (result != MA_SUCCESS)
        {
            CC_LOG_ERROR("Failed to uninitialise decoder")
        }

        std::memset(&voice, 0, sizeof(Voice));
        CC_LOG_INFO("Flushed voice playing sound clip with id {} on voice idx {}", id, i)
    }
    voicesMutex.unlock();
}

void AudioSystem::Uninitialise()
{
    voicesMutex.lock();
    for (int32_t i = 0; i < AUDIO_SYSTEM_MAX_VOICES; ++i)
    {
        Voice& voice = voices[i];
        uint64_t id = voice.id;
        if (id == 0)
        {
            continue;
        }

        ma_result result = ma_sound_stop(&voice.sound);
        if (result != MA_SUCCESS)
        {
            CC_LOG_ERROR("Failed to stop sound with id {} on voice idx {}", voice.id, i)
        }
        voice.pendingKill = true;
    }
    voicesMutex.unlock();
    Update();

    ma_engine_uninit(&audioEngine);
}

} // namespace Siege