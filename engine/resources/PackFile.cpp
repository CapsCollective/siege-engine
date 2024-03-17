//
// Copyright (c) 2020-present Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "PackFile.h"

#include <fstream>
#include <iostream>
#include <map>

#include <utils/FileSystem.h>
#include <utils/Logging.h>

namespace Siege
{

PackFile::PackFile(const String& filepath)
{
    LoadFromPath(filepath);
}

bool PackFile::LoadFromPath(const String& filepath)
{
    std::ifstream inputFileStream;
    inputFileStream.open(filepath, std::ios::in | std::ios::binary);
    inputFileStream.read(reinterpret_cast<char*>(&header), sizeof(PackFile::Header));

    body = reinterpret_cast<char*>(malloc(header.bodySize));
    inputFileStream.read(body, (uint32_t) header.bodySize);
    inputFileStream.close();

    char* tocStart = body + header.tocOffset;
    CC_ASSERT(memcmp(tocStart, &PACKER_MAGIC_NUMBER_TOC, 4) == 0, "Failed to find magic number!")

    char* tocCurr = tocStart + PACKER_MAGIC_NUMBER_SIZE;
    char* tocEnd = body + header.bodySize;
    while (tocCurr < tocEnd)
    {
        PackFile::TocEntry* toc = reinterpret_cast<PackFile::TocEntry*>(tocCurr);
        if (!toc)
        {
            break;
        }
        tocCurr += toc->GetDataSize();
        entries.emplace(toc->name, toc);
    }
    return true;
}

const std::map<String, PackFile::TocEntry*>& PackFile::GetEntries()
{
    return entries;
}

PackFile::TocEntry* PackFile::TocEntry::Create(const String& name, uint32_t dataOffset, uint32_t dataSize)
{
    uint32_t nameDataSize = name.Size() + 1;
    void* mem = malloc(sizeof(TocEntry) + nameDataSize);

    PackFile::TocEntry* tocEntry = new (mem) TocEntry();
    tocEntry->nameSize = nameDataSize;
    tocEntry->dataOffset = dataOffset;
    tocEntry->dataSize = dataSize;
    strcpy(&tocEntry->name[0], name.Str());

    return tocEntry;
}

} // namespace Siege

namespace std
{

std::ostream& operator<<(std::ostream& os, const Siege::PackFile::MagicNumber& magicNumber)
{
    os << magicNumber.string;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Siege::PackFile::Header& header)
{
    os << "PackFile::Header: {";
    os << "magic: " << header.magic << ", ";
    os << "version: " << header.version << ", ";
    os << "bodySize: " << header.bodySize << ", ";
    os << "tocOffset: " << header.tocOffset << "}";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Siege::PackFile::TocEntry& tocEntry)
{
    os << "PackFile::TocEntry: {";
    os << "magic: " << tocEntry.nameSize << ", ";
    os << "version: " << tocEntry.dataOffset << ", ";
    os << "bodySize: " << tocEntry.dataSize << ", ";
    os << "tocOffset: " << tocEntry.name << "}";
    return os;
}

} // namespace std
