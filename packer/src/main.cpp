//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <resources/PackFile.h>
#include <resources/PackFileData.h>
#include <resources/ResourceSystem.h>
#include <resources/SceneData.h>
#include <resources/StaticMeshData.h>
#include <resources/SkeletalMeshData.h>
#include <resources/AnimationData.h>
#include <resources/Texture2DData.h>
#include <utils/Logging.h>
#include <zlib.h>

#include <fstream>

#include "types/AnimationDataPacker.h"
#include "types/GenericFileDataPacker.h"
#include "types/SceneDataPacker.h"
#include "types/SkeletalMeshDataPacker.h"
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

    uint32_t entriesDataSize = 0;
    uint32_t entriesTocSize = 0;

    std::vector<std::pair<PackFile::TocEntry*, void*>> entries;
    for (auto& file : inputFiles)
    {
        if (file.empty()) continue;
        CC_LOG_INFO("Reading asset at path {}", file.c_str())

        Siege::PackFileData* data = nullptr;
        Siege::String fullPath = assetsDir + "/" + Siege::String(file.c_str());
        std::filesystem::path extension = file.extension();
        if (extension == ".sm")
        {
            data = PackStaticMeshFile(fullPath, assetsDir);
        }
        else if (extension == ".sk")
        {
            data = PackSkeletalMeshFile(fullPath, assetsDir);
            bodyDataSize = Siege::SkeletalMeshData::GetDataSize(data);
        }
        else if (extension == ".ska")
        {
            data = PackAnimationFile(fullPath, assetsDir);
            bodyDataSize = Siege::AnimationData::GetDataSize(data);
        }
        else if (extension == ".jpg" || extension == ".jpeg" || extension == ".png")
        {
            data = PackTexture2DFile(fullPath);
        }
        else if (extension == ".spv" || extension == ".ttf")
        {
            data = PackGenericFile(fullPath);
        }
        else if (extension == ".scene")
        {
            data = PackSceneFile(fullPath);
        }

        if (!data)
        {
            CC_LOG_ERROR("Failed to create pack data for file \"{}\"", file.c_str())
            errors = true;
            continue;
        }

        uint32_t dataSize = data->GetDataSize();
        PackFile::TocEntry* tocEntry =
            PackFile::TocEntry::Create(file.c_str(), entriesDataSize, dataSize);

        entries.emplace_back(tocEntry, data);

        entriesDataSize += dataSize;
        entriesTocSize += tocEntry->GetDataSize();
    }

    PackFile::Header header {{PACKER_MAGIC_NUMBER_FILE},
                             PACKER_FILE_VERSION,
                             entriesDataSize + PACKER_MAGIC_NUMBER_SIZE + entriesTocSize,
                             entriesDataSize};

    CC_LOG_INFO(
        "Beginning pack file version {} write process for body size {} and ToC offset of {}...",
        header.version,
        header.bodySize,
        header.tocOffset)
    uint64_t writeTotal = 0;

    std::ofstream outputFileStream;
    outputFileStream.open(outputFile, std::ios::out | std::ios::binary);

    outputFileStream.write(reinterpret_cast<char*>(&header), sizeof(PackFile::Header));
    writeTotal += sizeof(PackFile::Header);
    CC_LOG_INFO("Adding HEADER to pack file with size: {} (write total: {})",
                sizeof(PackFile::Header),
                writeTotal)

    entriesDataSize = 0;
    Bytef* bodyDataBufferCompressed = nullptr;
    for (const std::pair<PackFile::TocEntry*, void*>& entry : entries)
    {
        uLongf bodyDataSizeUncompressed = entry.first->dataSize;
        uLongf bodyDataSizeCompressed = compressBound(bodyDataSizeUncompressed);

        Bytef* tempBuffer =
            static_cast<Bytef*>(realloc(bodyDataBufferCompressed, bodyDataSizeCompressed));
        bodyDataBufferCompressed = tempBuffer;

        int result = compress2(bodyDataBufferCompressed,
                               &bodyDataSizeCompressed,
                               static_cast<Bytef*>(entry.second),
                               bodyDataSizeUncompressed,
                               Z_BEST_COMPRESSION);
        CC_ASSERT(result == Z_OK,
                  "Compression failed for entry: " + Siege::String(entry.first->name));

        outputFileStream.write(reinterpret_cast<char*>(bodyDataBufferCompressed),
                               static_cast<long>(bodyDataSizeCompressed));
        entry.first->dataOffset = entriesDataSize;
        entry.first->dataSizeCompressed = bodyDataSizeCompressed;
        writeTotal += bodyDataSizeCompressed;
        entriesDataSize += bodyDataSizeCompressed;
        CC_LOG_INFO(
            "Adding DATA \"{}\" to pack file with size: {} from {}, compressed to ~{}% (write "
            "total: {})",
            entry.first->name,
            bodyDataSizeCompressed,
            bodyDataSizeUncompressed,
            static_cast<uint8_t>(ceilf(static_cast<float>(bodyDataSizeCompressed) /
                                       static_cast<float>(bodyDataSizeUncompressed) * 100.f)),
            writeTotal)
    }
    if (bodyDataBufferCompressed) free(bodyDataBufferCompressed);

    outputFileStream.write(PACKER_MAGIC_NUMBER_TOC, PACKER_MAGIC_NUMBER_SIZE);
    writeTotal += PACKER_MAGIC_NUMBER_SIZE;
    CC_LOG_INFO("Adding MAGIC NUMBER to pack file with size: {} (write total: {})",
                PACKER_MAGIC_NUMBER_SIZE,
                writeTotal)

    for (const std::pair<PackFile::TocEntry*, void*>& entry : entries)
    {
        outputFileStream.write(reinterpret_cast<char*>(entry.first), entry.first->GetDataSize());
        writeTotal += entry.first->GetDataSize();
        CC_LOG_INFO("Adding TOC \"{}\" (offset: {}) to pack file with size: {} (write total: {})",
                    entry.first->name,
                    entry.first->dataOffset,
                    entry.first->GetDataSize(),
                    writeTotal)
    }

    header.bodySize = entriesDataSize + PACKER_MAGIC_NUMBER_SIZE + entriesTocSize;
    header.tocOffset = entriesDataSize;
    outputFileStream.seekp(0);
    outputFileStream.write(reinterpret_cast<char*>(&header), sizeof(PackFile::Header));

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
        }
    }

    CC_LOG_INFO("ToC contents finished verification process, unmounting written pack file...")
    resourceSystem.UnmountPackFile();

    CC_LOG_INFO("Freeing dynamically allocated memory from packer...")
    for (const std::pair<PackFile::TocEntry*, void*>& entry : entries)
    {
        free(entry.first);
        free(entry.second);
    }

    if (errors)
    {
        CC_LOG_ERROR("Packing process completed with errors!")
        return 1;
    }
    CC_LOG_INFO("Packing process completed successfully!")
    return 0;
}
