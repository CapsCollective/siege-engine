//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_AUDIOSYSTEM_H
#define SIEGE_ENGINE_AUDIOSYSTEM_H

#include <resources/PackFileData.h>
#include <utils/Token.h>
#include <utils/math/Xform.h>

#include <memory>

namespace Siege
{

class SoundPlayer
{
public:

    // Public methods

    /**
     * Plays the sound clip with the current configuration of the object
     * @return true if the sound successfully began play, otherwise false
     */
    bool Play();

    /**
     * Stops the sound across all active voices related to the player
     * @return true if the sound successfully stopped play, otherwise false
     */
    bool Stop();

    /**
     * Updates all active voices related to the player with the current
     * configuration of the object
     * @return true if the player updated any voices, otherwise false
     */
    bool UpdateVoices();

    /**
     * Checks whether there are any playing or pending kill voices
     * related to the player
     * @return true if playing, false otherwise
     */
    bool IsPlaying() const;

    // Public fields

    // An automatically assigned ID for the player
    // (DO NOT modify this value)
    uint64_t playerId = 0;

    // Pointer to raw data of audio clip
    std::shared_ptr<PackFileData> clipData;

    // Spatial location of the sound
    Vec3 location;

    // Forces spatialisation off for this sound
    bool disableSpatialisation = false;

    // Loops the clip upon reaching its end
    bool looping = false;

    // The limit of concurrent voices allowed for
    // this player (leave as 0 for no limiting)
    uint8_t maxPolyphony = 0;
};

class AudioSystem
{
public:

    // Public methods

    /**
     * Retrieves the audio system singleton object
     * @return a reference to the audio system
     */
    static AudioSystem& GetInstance();

    /**
     * Initialises the audio engine and any other system configuration
     * @return true if the initialisation was successful, otherwise false
     */
    bool Initialise();

    /**
     * Finds and assigns a free voice to a given sound player
     * @param soundPlayer - the sound player to assign
     * @return true if the assignment was successful, otherwise false
     * @note this method will also assign an ID to the player from if
     *       it does not already have one
     */
    bool AssignPlayerToVoice(SoundPlayer* soundPlayer);

    /**
     * Finds and marks all voices related to a particular sound player
     * for pending kill on the next audio system update
     * @param soundPlayer - the sound player to unassign
     * @return true if the unassignment was successful, otherwise false
     */
    bool UnassignPlayerFromVoices(const SoundPlayer* soundPlayer);

    /**
     * Updates all voices related to a particular sound player,
     * including audio spatialisation settings
     * @param soundPlayer - the sound player to update
     * @return true if any relevant voices were found, otherwise false
     */
    bool UpdateSoundPlayerVoices(const SoundPlayer* soundPlayer);

    /**
     * Checks whether there are any playing or pending kill voices
     * @param soundPlayer - the sound player to check
     * @return true if the sound is playing, otherwise false
     */
    bool IsSoundPlayingForPlayer(const SoundPlayer* soundPlayer) const;

    /**
     * Sets the transform of the audio system sound listener
     * @param transform - the transform to apply
     */
    void SetSoundListenerTransform(const Xform& transform);

    /**
     * Updates the audio system for the current frame to free up any
     * voices pending kill for future use
     */
    void Update();

    /**
     * Stops all sounds, marks all active voices for pending kill,
     * runs a final update frame to clear out playing sounds, and
     * uninitialises the audio engine
     */
    void Uninitialise();
};

} // namespace Siege

#endif // SIEGE_ENGINE_AUDIOSYSTEM_H
