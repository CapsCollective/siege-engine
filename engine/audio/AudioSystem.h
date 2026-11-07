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

#include <utils/String.h>
#include <resources/PackFileData.h>

namespace Siege
{

class AudioSystem
{
public:

    // Public methods

    static AudioSystem& GetInstance();

    bool Initialise();

    bool PlayClip(const String& path) const;

    bool PlayClip(const PackFileData& data) const;

    void Update();

    void Uninitialise();

};

} // namespace Siege

#endif // SIEGE_ENGINE_AUDIOSYSTEM_H
