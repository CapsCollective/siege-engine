//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_SCENEDATA_H
#define SIEGE_ENGINE_SCENEDATA_H

#include <utils/BinarySerialisation.h>

namespace Siege
{

struct SceneData
{
    std::vector<String> entities;
};

namespace BinarySerialisation
{

inline void serialise(Buffer& buffer, SceneData& value, SerialisationMode mode)
{
    serialise(buffer, value.entities, mode);
}

} // namespace BinarySerialisation

} // namespace Siege

#endif // SIEGE_ENGINE_SCENEDATA_H
