//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_TEXTURE2DDATA_H
#define SIEGE_ENGINE_TEXTURE2DDATA_H

#include <utils/BinarySerialisation.h>

// TODO - Remove this hard-coded value and support various texture channel sizes
#define TEXTURE2D_TEXTURE_CHANNELS 4

namespace Siege
{

struct Texture2DData
{
    int32_t texWidth = 0;
    int32_t texHeight = 0;
    int32_t texChannels = TEXTURE2D_TEXTURE_CHANNELS;
    std::vector<uint8_t> pixels;

    uint64_t GetImageSize() const
    {
        return texWidth * texHeight * texChannels;
    }
};

namespace BinarySerialisation
{

inline void serialise(Buffer& buffer, Texture2DData& value, SerialisationMode mode)
{
    serialise(buffer, value.texWidth, mode);
    serialise(buffer, value.texHeight, mode);
    serialise(buffer, value.texChannels, mode);
    serialise(buffer, value.pixels, mode);
}

} // namespace BinarySerialisation

} // namespace Siege

#endif // SIEGE_ENGINE_TEXTURE2DDATA_H
