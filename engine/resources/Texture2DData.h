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

#include "PackFile.h"
#include <utils/BinarySerialisation.h>

// TODO - Remove this hard-coded value and support various texture channel sizes
#define TEXTURE2D_TEXTURE_CHANNELS 4

namespace Siege
{

struct Texture2DData : BinarySerialisation::BinarySerialisable
{
    int32_t texWidth = 0;
    int32_t texHeight = 0;
    int32_t texChannels = TEXTURE2D_TEXTURE_CHANNELS;
    std::vector<uint8_t> pixels;

    uint64_t GetImageSize() const
    {
        return texWidth * texHeight * TEXTURE2D_TEXTURE_CHANNELS;
    }

    void serialise(BinarySerialisation::Buffer& buffer, BinarySerialisation::SerialisationMode mode) override
    {
        BinarySerialisation::serialise(buffer, texWidth, mode);
        BinarySerialisation::serialise(buffer, texHeight, mode);
        BinarySerialisation::serialise(buffer, texChannels, mode);
        BinarySerialisation::serialise(buffer, pixels, mode);
    }
};

} // namespace Siege

#endif // SIEGE_ENGINE_TEXTURE2DDATA_H
