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
    Token contextToken;
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
    decoderConfig = ma_decoder_config_init_default();
    ma_result result = ma_engine_init(nullptr, &audioEngine);
    return result == MA_SUCCESS;
}

static int32_t FindPolyphonyCountForSound(const SoundSource& soundSource)
{
    int32_t count = 0;
    for (int32_t i = 0; i < AUDIO_SYSTEM_MAX_VOICES; ++i)
    {
        const Voice& voice = voices[i];
        if (voice.contextToken == soundSource.contextToken)
        {
            ++count;
        }
    }
    return count;
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
    std::scoped_lock lock(voicesMutex);
    uint64_t voiceIdx = reinterpret_cast<uint64_t>(userData);
    Voice& voice = voices[voiceIdx];
    voice.pendingKill = true;
    CC_LOG_INFO("Marked pending kill sound clip with id {} on voice idx {}", voice.id, voiceIdx)
}

bool AudioSystem::PlaySoundSource(const SoundSource& soundSource)
{
    std::scoped_lock lock(voicesMutex);

    void* clipData = soundSource.clipData->data;
    uint32_t clipDataLength = soundSource.clipData->dataSize;

    if (soundSource.maxPolyphony != 0)
    {
        int32_t currentPolyphony = FindPolyphonyCountForSound(soundSource);
        if (currentPolyphony >= soundSource.maxPolyphony)
        {
            CC_LOG_WARNING("Cannot play requested clip at address above {} max polyphony of {}",
                reinterpret_cast<uint64_t>(clipData), soundSource.maxPolyphony);
            return false;
        }
    }

    int32_t voiceIdx = FindAvailableVoice();
    if (voiceIdx == -1)
    {
        CC_LOG_WARNING("Failed to find available voice for requested clip at address {}",
            reinterpret_cast<uint64_t>(clipData));
        return false;
    }

    Voice& voice = voices[voiceIdx];

    ma_result result = ma_decoder_init_memory(clipData, clipDataLength, &decoderConfig, &voice.decoder);
    if (result != MA_SUCCESS)
    {
        CC_LOG_ERROR("Failed to initialise decoder for requested clip at address {}",
            reinterpret_cast<uint64_t>(clipData))
        return false;
    }

    result = ma_sound_init_from_data_source(&audioEngine, &voice.decoder, 0, nullptr, &voice.sound);
    if (result != MA_SUCCESS)
    {
        CC_LOG_ERROR("Failed to initialise sound for requested clip at address {}",
            reinterpret_cast<uint64_t>(clipData))
        result = ma_decoder_uninit(&voice.decoder);
        if (result != MA_SUCCESS)
        {
            CC_LOG_ERROR("Failed to uninitialise decoder after bad sound initialisation")
        }
        return false;
    }

    static uint64_t globalId = 0;
    voice.id = ++globalId;

    voice.contextToken = soundSource.contextToken;

    result = ma_sound_set_end_callback(&voice.sound, OnSoundEnded, reinterpret_cast<void*>(voiceIdx));
    if (result != MA_SUCCESS)
    {
        CC_LOG_ERROR("Failed to set sound ended callback for requested clip at address {}",
            reinterpret_cast<uint64_t>(clipData))
        ma_sound_uninit(&voice.sound);
        result = ma_decoder_uninit(&voice.decoder);
        if (result != MA_SUCCESS)
        {
            CC_LOG_ERROR("Failed to uninitialise decoder after failure to set callback")
        }
        return false;
    }

    if (soundSource.looping)
    {
        ma_sound_set_looping(&voice.sound, true);
    }

    result = ma_sound_start(&voice.sound);
    if (result != MA_SUCCESS)
    {
        CC_LOG_ERROR("Failed to start sound for requested clip at address {}",
            reinterpret_cast<uint64_t>(clipData))
        ma_sound_uninit(&voice.sound);
        result = ma_decoder_uninit(&voice.decoder);
        if (result != MA_SUCCESS)
        {
            CC_LOG_ERROR("Failed to uninitialise decoder after sound failed to start")
        }
        return false;
    }

    CC_LOG_INFO("Started playing sound clip at address {} with id {} on voice idx {}",
        reinterpret_cast<uint64_t>(clipData), voice.id, voiceIdx)
    return true;
}

bool AudioSystem::StopSoundSource(const SoundSource& soundSource)
{
    std::scoped_lock lock(voicesMutex);
    bool stoppedSounds = false;
    for (int32_t i = 0; i < AUDIO_SYSTEM_MAX_VOICES; ++i)
    {
        Voice& voice = voices[i];
        if (voice.contextToken != soundSource.contextToken)
        {
            continue;
        }

        ma_result result = ma_sound_stop(&voice.sound);
        if (result != MA_SUCCESS)
        {
            CC_LOG_ERROR("Failed to stop sound with id {} on voice idx {}", voice.id, i)
        }
        CC_LOG_INFO("Stopped sound clip with id {} on voice idx {}", voice.id, i)

        voice.pendingKill = true;
        CC_LOG_INFO("Marked pending kill sound clip with id {} on voice idx {}", voice.id, i)
        stoppedSounds = true;
    }
    return stoppedSounds;
}

void AudioSystem::Update()
{
    std::scoped_lock lock(voicesMutex);
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
}

void AudioSystem::Uninitialise()
{
    {
        std::scoped_lock lock(voicesMutex);
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
            CC_LOG_INFO("Stopped sound clip with id {} on voice idx {}", voice.id, i)

            voice.pendingKill = true;
            CC_LOG_INFO("Marked pending kill sound clip with id {} on voice idx {}", voice.id, i)
        }
    }
    Update();

    ma_engine_uninit(&audioEngine);
}

} // namespace Siege