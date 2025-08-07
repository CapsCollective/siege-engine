//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_ANIMATIONDATA_H
#define SIEGE_ENGINE_ANIMATIONDATA_H

#include <utils/math/vec/Vec3.h>
#include <utils/math/mat/Mat4.h>

#include "PackFile.h"

namespace Siege
{

struct AnimKeyPosition
{
    double timestamp = 0.f;
    Vec3 position;
};

struct AnimKeyRotation
{
    double timestamp = 0.f;
    Vec4 rotation;
};

struct AnimKeyScale
{
    double timestamp = 0.f;
    Vec3 scale;
};

struct AnimationChannel
{
    std::vector<AnimKeyPosition> positionKeys;
    std::vector<AnimKeyRotation> rotationKeys;
    std::vector<AnimKeyScale> scaleKeys;
};

struct AnimationData
{
    double length;
    double speed;
    std::map<String, AnimationChannel> channels;
};

namespace BinarySerialisation
{

inline void serialise(Buffer& buffer, AnimKeyPosition& value, SerialisationMode mode)
{
    serialise(buffer, value.timestamp, mode);
    serialise(buffer, value.position, mode);
}

inline void serialise(Buffer& buffer, AnimKeyRotation& value, SerialisationMode mode)
{
    serialise(buffer, value.timestamp, mode);
    serialise(buffer, value.rotation, mode);
}

inline void serialise(Buffer& buffer, AnimKeyScale& value, SerialisationMode mode)
{
    serialise(buffer, value.timestamp, mode);
    serialise(buffer, value.scale, mode);
}

inline void serialise(Buffer& buffer, AnimationChannel& value, SerialisationMode mode)
{
    serialise(buffer, value.positionKeys, mode);
    serialise(buffer, value.rotationKeys, mode);
    serialise(buffer, value.scaleKeys, mode);
}

inline void serialise(Buffer& buffer, AnimationData& value, SerialisationMode mode)
{
    serialise(buffer, value.length, mode);
    serialise(buffer, value.speed, mode);
    serialise(buffer, value.channels, mode);
}

} // namespace BinarySerialisation

} // namespace Siege

#endif // SIEGE_ENGINE_ANIMATIONDATA_H
