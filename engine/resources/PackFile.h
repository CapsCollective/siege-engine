//
// Copyright (c) 2020-present Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_PACKFILE_H
#define SIEGE_ENGINE_PACKFILE_H

#include <utils/String.h>

#include <filesystem>
#include <map>

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
        PackFile::TocEntry* toc = entries[filepath];
        if (!toc)
        {
            return nullptr;
        }
        return reinterpret_cast<T*>(body + toc->dataOffset);
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
