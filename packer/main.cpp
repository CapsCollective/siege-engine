//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <resources/GenericFileData.h>
#include <resources/PackFile.h>
#include <resources/ResourceSystem.h>
#include <resources/SceneData.h>
#include <resources/StaticMeshData.h>
#include <resources/Texture2DData.h>
#include <utils/Logging.h>

#include <fstream>

#include "types/GenericFileDataPacker.h"
#include "types/SceneDataPacker.h"
#include "types/StaticMeshDataPacker.h"
#include "types/Texture2DDataPacker.h"

using Siege::PackFile;

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        CC_LOG_ERROR("Requires at least three arguments, expected form <outputFile> <assetsDir> "
                     "[<inputFiles>]")
        return 1;
    }

    bool errors = false;
    Siege::String outputFile = argv[1];
    Siege::String assetsDir = argv[2];

    std::vector<std::filesystem::path> inputFiles;
    for (int currentArg = 3; currentArg < argc; ++currentArg)
    {
        inputFiles.emplace_back(argv[currentArg]);
    }

    uint32_t dataSize = 0;
    uint32_t entriesSize = 0;

    struct BodyDataEntry
    {
        void* ptr;
        uint32_t size;
    };

    std::vector<void*> dynamicAllocations;
    std::vector<PackFile::TocEntry*> tocEntries;
    std::vector<BodyDataEntry> bodyDataEntries;
    for (auto& file : inputFiles)
    {
        if (file.empty()) continue;

        void* data = nullptr;
        uint32_t bodyDataSize = 0;
        Siege::String fullPath = assetsDir + "/" + Siege::String(file.c_str());
        std::filesystem::path extension = file.extension();
        if (extension == ".sm")
        {
            data = PackStaticMeshFile(fullPath);
            bodyDataSize = Siege::StaticMeshData::GetDataSize(data);
        }
        else if (extension == ".jpg" || extension == ".jpeg" || extension == ".png")
        {
            data = PackTexture2DFile(fullPath);
            bodyDataSize = Siege::Texture2DData::GetDataSize(data);
        }
        else if (extension == ".spv" || extension == ".ttf")
        {
            data = PackGenericFile(fullPath);
            bodyDataSize = Siege::GenericFileData::GetDataSize(data);
        }
        else if (extension == ".scene")
        {
            data = PackSceneFile(fullPath);
            bodyDataSize = Siege::SceneData::GetDataSize(data);
        }

        if (!data)
        {
            CC_LOG_ERROR("Failed to create pack data for file \"{}\"", file.c_str())
            errors = true;
            continue;
        }

        PackFile::TocEntry* tocEntry =
            PackFile::TocEntry::Create(file.c_str(), dataSize, bodyDataSize);

        bodyDataEntries.push_back({data, bodyDataSize});
        tocEntries.push_back(tocEntry);

        dataSize += bodyDataSize;
        entriesSize += tocEntry->GetDataSize();

        dynamicAllocations.push_back(data);
        dynamicAllocations.push_back(tocEntry);
    }

    PackFile::Header header {{PACKER_MAGIC_NUMBER_FILE},
                             PACKER_FILE_VERSION,
                             dataSize + PACKER_MAGIC_NUMBER_SIZE + entriesSize,
                             dataSize};

    CC_LOG_INFO(
        "Beginning pack file version {} write process for body size {} and ToC offset of {}...",
        header.version,
        header.bodySize,
        header.tocOffset)
    uint64_t writeTotal = 0;
    uint64_t dataOffset = 0;

    std::ofstream outputFileStream;
    outputFileStream.open(outputFile, std::ios::out | std::ios::binary);

    outputFileStream.write(reinterpret_cast<char*>(&header), sizeof(PackFile::Header));
    writeTotal += sizeof(PackFile::Header);
    CC_LOG_INFO("Adding HEADER to pack file with size: {} (write total: {})",
                sizeof(PackFile::Header),
                writeTotal)

    for (auto entry : bodyDataEntries)
    {
        uint32_t bodyDataSize = entry.size;
        outputFileStream.write(reinterpret_cast<char*>(entry.ptr), bodyDataSize);
        writeTotal += bodyDataSize;
        CC_LOG_INFO("Adding DATA (offset: {}) to pack file with size: {} (write total: {})",
                    dataOffset,
                    bodyDataSize,
                    writeTotal)
        dataOffset += bodyDataSize;
    }

    outputFileStream.write(PACKER_MAGIC_NUMBER_TOC, PACKER_MAGIC_NUMBER_SIZE);
    writeTotal += PACKER_MAGIC_NUMBER_SIZE;
    CC_LOG_INFO("Adding MAGIC NUMBER to pack file with size: {} (write total: {})",
                PACKER_MAGIC_NUMBER_SIZE,
                writeTotal)

    for (Siege::PackFile::TocEntry* toc : tocEntries)
    {
        outputFileStream.write(reinterpret_cast<char*>(toc), toc->GetDataSize());
        writeTotal += toc->GetDataSize();
        CC_LOG_INFO(
            "Adding TOC ENTRY \"{}\" (offset: {}) to pack file with size: {} (write total: {})",
            toc->name,
            toc->dataOffset,
            toc->GetDataSize(),
            writeTotal)
    }

    outputFileStream.close();
    CC_LOG_INFO("Ended pack file write process (write total: {})", writeTotal)

    uint32_t expectedSize = sizeof(PackFile::Header) + header.bodySize;
    CC_ASSERT(expectedSize == writeTotal, "Write total does not match expected file size!")

    CC_LOG_INFO("Mounting and verifying ToC contents of written pack file...")
    Siege::ResourceSystem& resourceSystem = Siege::ResourceSystem::GetInstance();
    std::filesystem::path buildDir(outputFile.Str());
    resourceSystem.MountPackFile(buildDir.remove_filename().c_str());
    PackFile* packFile = resourceSystem.GetPackFile();

    for (const std::filesystem::path& file : inputFiles)
    {
        auto it = packFile->GetEntries().find(file.c_str());
        if (it == packFile->GetEntries().end())
        {
            CC_LOG_WARNING("Missing ToC entry for input file \"{}\"", file.c_str())
            errors = true;
            continue;
        }
    }

    CC_LOG_INFO("ToC contents finished verification process, unmounting written pack file...")
    resourceSystem.UnmountPackFile();

    CC_LOG_INFO("Freeing dynamically allocated memory from packer...")
    for (void* allocation : dynamicAllocations) free(allocation);

    if (errors)
    {
        CC_LOG_ERROR("Packing process completed with errors!")
        return 1;
    }
    CC_LOG_INFO("Packing process completed successfully!")
    return 0;
}
