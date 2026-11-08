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
    uint64_t playerId = 0;
    ma_decoder decoder = {};
    ma_sound sound = {};
    bool pendingKill = false;
};

static ma_engine audioEngine;
static ma_decoder_config decoderConfig;

static Voice voices[AUDIO_SYSTEM_MAX_VOICES];
static std::mutex voicesMutex;

bool SoundPlayer::Play()
{
    return AudioSystem::GetInstance().AssignPlayerToVoice(this);
}

bool SoundPlayer::Stop()
{
    return AudioSystem::GetInstance().UnassignPlayerFromVoices(this);
}

bool SoundPlayer::UpdateVoices()
{
    return AudioSystem::GetInstance().UpdateSoundPlayerVoices(this);
}

bool SoundPlayer::IsPlaying() const
{
    return AudioSystem::GetInstance().IsSoundPlayingForPlayer(this);
}

AudioSystem& AudioSystem::GetInstance()
{
    static AudioSystem audioSystem;
    return audioSystem;
}

bool AudioSystem::Initialise()
{
    decoderConfig = ma_decoder_config_init_default();
    ma_engine_config engineConfig = ma_engine_config_init();
    engineConfig.listenerCount = 1;
    ma_result result = ma_engine_init(&engineConfig, &audioEngine);
    return result == MA_SUCCESS;
}

static int32_t FindPolyphonyCountForSound(const SoundPlayer* soundPlayer)
{
    int32_t count = 0;
    for (int32_t i = 0; i < AUDIO_SYSTEM_MAX_VOICES; ++i)
    {
        const Voice& voice = voices[i];
        if (voice.playerId == soundPlayer->playerId)
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
        uint64_t id = voice.playerId;
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
    CC_LOG_INFO("Marked pending kill sound clip with id {} on voice idx {}",
                voice.playerId,
                voiceIdx)
}

bool AudioSystem::AssignPlayerToVoice(SoundPlayer* soundPlayer)
{
    std::scoped_lock lock(voicesMutex);

    void* clipData = soundPlayer->clipData->data;
    uint32_t clipDataLength = soundPlayer->clipData->dataSize;

    if (soundPlayer->playerId != 0 && soundPlayer->maxPolyphony != 0)
    {
        int32_t currentPolyphony = FindPolyphonyCountForSound(soundPlayer);
        if (currentPolyphony >= soundPlayer->maxPolyphony)
        {
            CC_LOG_WARNING("Cannot play requested clip at address above {} max polyphony of {}",
                           reinterpret_cast<uint64_t>(clipData),
                           soundPlayer->maxPolyphony);
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

    ma_result result =
        ma_decoder_init_memory(clipData, clipDataLength, &decoderConfig, &voice.decoder);
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
    if (soundPlayer->playerId == 0)
    {
        soundPlayer->playerId = ++globalId;
    }
    voice.playerId = soundPlayer->playerId;

    result =
        ma_sound_set_end_callback(&voice.sound, OnSoundEnded, reinterpret_cast<void*>(voiceIdx));
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
        --globalId;
        return false;
    }

    if (soundPlayer->looping)
    {
        ma_sound_set_looping(&voice.sound, true);
    }

    ma_sound_set_spatialization_enabled(&voice.sound, !soundPlayer->disableSpatialisation);
    if (!soundPlayer->disableSpatialisation)
    {
        const Vec3& pos = soundPlayer->location;
        ma_sound_set_position(&voice.sound, pos.x, pos.y, pos.z);
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
        --globalId;
        return false;
    }

    CC_LOG_INFO("Started playing sound clip at address {} with id {} on voice idx {}",
                reinterpret_cast<uint64_t>(clipData),
                voice.playerId,
                voiceIdx)
    return true;
}

bool AudioSystem::UnassignPlayerFromVoices(const SoundPlayer* soundPlayer)
{
    std::scoped_lock lock(voicesMutex);
    bool stoppedSounds = false;
    for (int32_t i = 0; i < AUDIO_SYSTEM_MAX_VOICES; ++i)
    {
        Voice& voice = voices[i];
        if (voice.playerId != soundPlayer->playerId)
        {
            continue;
        }

        ma_result result = ma_sound_stop(&voice.sound);
        if (result != MA_SUCCESS)
        {
            CC_LOG_ERROR("Failed to stop sound with id {} on voice idx {}", voice.playerId, i)
        }
        CC_LOG_INFO("Stopped sound clip with id {} on voice idx {}", voice.playerId, i)

        voice.pendingKill = true;
        CC_LOG_INFO("Marked pending kill sound clip with id {} on voice idx {}", voice.playerId, i)
        stoppedSounds = true;
    }
    return stoppedSounds;
}

bool AudioSystem::UpdateSoundPlayerVoices(const SoundPlayer* soundPlayer)
{
    std::scoped_lock lock(voicesMutex);
    bool foundVoices = false;
    for (int32_t i = 0; i < AUDIO_SYSTEM_MAX_VOICES; ++i)
    {
        Voice& voice = voices[i];
        if (voice.playerId != soundPlayer->playerId)
        {
            continue;
        }

        const Vec3& pos = soundPlayer->location;
        ma_sound_set_position(&voice.sound, pos.x, pos.y, pos.z);
        foundVoices = true;
    }
    return foundVoices;
}

bool AudioSystem::IsSoundPlayingForPlayer(const SoundPlayer* soundPlayer) const
{
    std::scoped_lock lock(voicesMutex);
    int32_t currentPolyphony = FindPolyphonyCountForSound(soundPlayer);
    return currentPolyphony > 0;
}

void AudioSystem::SetSoundListenerTransform(const Xform& transform)
{
    uint32_t listenerIdx = 0;
    ma_engine_listener_set_world_up(&audioEngine, listenerIdx, 0, 1, 0);

    const Vec3& pos = transform.GetPosition();
    const Vec3& rot = transform.GetRotation();
    ma_engine_listener_set_position(&audioEngine, listenerIdx, pos.x, pos.y, pos.z);
    ma_engine_listener_set_direction(&audioEngine, listenerIdx, rot.x, rot.y, rot.z);
}

void AudioSystem::Update()
{
    std::scoped_lock lock(voicesMutex);
    for (int32_t i = 0; i < AUDIO_SYSTEM_MAX_VOICES; ++i)
    {
        Voice& voice = voices[i];
        uint64_t id = voice.playerId;
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
            uint64_t id = voice.playerId;
            if (id == 0)
            {
                continue;
            }

            ma_result result = ma_sound_stop(&voice.sound);
            if (result != MA_SUCCESS)
            {
                CC_LOG_ERROR("Failed to stop sound with id {} on voice idx {}", voice.playerId, i)
            }
            CC_LOG_INFO("Stopped sound clip with id {} on voice idx {}", voice.playerId, i)

            voice.pendingKill = true;
            CC_LOG_INFO("Marked pending kill sound clip with id {} on voice idx {}",
                        voice.playerId,
                        i)
        }
    }
    Update();

    ma_engine_uninit(&audioEngine);
}

} // namespace Siege