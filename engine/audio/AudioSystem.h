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

#include <utils/math/Xform.h>
#include <resources/PackFileData.h>
#include <utils/Token.h>

#include <memory>

namespace Siege
{

struct SoundSource
{
    std::shared_ptr<PackFileData> clipData;
    uint8_t maxPolyphony = 0;
    bool looping = false;
    Token contextToken;
    bool disableSpatialisation = false;
    Vec3 location;
};

class AudioSystem
{
public:

    // Public methods

    static AudioSystem& GetInstance();

    bool Initialise();

    bool PlaySoundSource(const SoundSource& soundSource);

    bool StopSoundSource(const SoundSource& soundSource);

    void SetSoundListenerTransform(const Xform& transform);

    void UpdateSoundSourcePosition(const SoundSource& soundSource);

    void Update();

    void Uninitialise();

};

} // namespace Siege

#endif // SIEGE_ENGINE_AUDIOSYSTEM_H
