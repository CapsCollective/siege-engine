//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_PACKFILE_H
#define SIEGE_ENGINE_PACKFILE_H

#include <utils/BinarySerialisation.h>
#include <utils/Logging.h>
#include <utils/String.h>
#include <zlib.h>

#include <filesystem>
#include <map>

#include "AnimationData.h"
#include "PackFileData.h"
#include "SceneData.h"
#include "SkeletalMeshData.h"
#include "StaticMeshData.h"
#include "Texture2DData.h"

#define PACKER_MAGIC_NUMBER_FILE "pck"
#define PACKER_MAGIC_NUMBER_TOC "toc!"
#define PACKER_MAGIC_NUMBER_SIZE sizeof(uint32_t)
#define PACKER_FILE_VERSION 1

namespace Siege
{

class PackFile
{
public:

#pragma pack(push, 1)
    union MagicNumber
    {
        char string[4];
        uint32_t number;
    };

    struct Header
    {
        MagicNumber magic;
        uint32_t version;
        uint64_t bodySize;
        uint64_t tocOffset;
    };

    struct TocEntry
    {
        uint32_t dataOffset;
        uint32_t dataSize;
        uint32_t dataSizeCompressed;
        char name[];

        uint32_t GetDataSize() const
        {
            return sizeof(TocEntry) + strlen(name) + 1;
        }

        static TocEntry* Create(const String& name, uint32_t dataOffset, uint32_t dataSize);
    };
#pragma pack(pop)

    // 'Structors

    explicit PackFile(const String& filepath);

    // Public methods

    bool LoadFromPath(const String& filepath);

    template<typename T>
    T* FindData(const String& filepath)
    {
        PackFileData* packFileData = FindData<PackFileData>(filepath);

        BinarySerialisation::Buffer dataBuffer;
        dataBuffer.Fill(reinterpret_cast<uint8_t*>(packFileData->data), packFileData->dataSize);
        delete packFileData;

        T* typedData = new T();
        BinarySerialisation::serialise(dataBuffer, *typedData, BinarySerialisation::DESERIALISE);

        return typedData;
    }

    template<>
    PackFileData* FindData<PackFileData>(const String& filepath)
    {
        const TocEntry* toc = entries[filepath];
        if (!toc)
        {
            return nullptr;
        }

        uLongf bodyDataSizeUncompressed = toc->dataSize;
        uLongf bodyDataSizeCompressed = toc->dataSizeCompressed;

        PackFileData* packFileData = new (malloc(bodyDataSizeUncompressed)) PackFileData();
        int result = uncompress(reinterpret_cast<Bytef*>(packFileData),
                                &bodyDataSizeUncompressed,
                                reinterpret_cast<Bytef*>(body + toc->dataOffset),
                                bodyDataSizeCompressed);
        CC_ASSERT(result == Z_OK, "Decompression failed for filepath: " + filepath);

        return packFileData;
    }

    const std::map<String, TocEntry*>& GetEntries();

    const Header& GetHeader();

private:

    // Private fields

    Header header {};

    char* body = nullptr;

    std::map<String, TocEntry*> entries;
};

} // namespace Siege

namespace std
{

std::ostream& operator<<(std::ostream& os, const Siege::PackFile::MagicNumber& magicNumber);

std::ostream& operator<<(std::ostream& os, const Siege::PackFile::Header& header);

std::ostream& operator<<(std::ostream& os, const Siege::PackFile::TocEntry& tocEntry);

} // namespace std

#endif // SIEGE_ENGINE_PACKFILE_H
