//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <resources/PackFileData.h>
#include <resources/PackFile.h>
#include <resources/ResourceSystem.h>
#include <resources/SceneData.h>
#include <resources/StaticMeshData.h>
#include <resources/Texture2DData.h>
#include <utest.h>

#include "utils/Logging.h"

using namespace Siege;

// Define test fixture
struct test_ResourceSystem
{};

UTEST_F_SETUP(test_ResourceSystem)
{
    ResourceSystem& resourceSystem = ResourceSystem::GetInstance();
    bool result = resourceSystem.MountPackFile();
    ASSERT_TRUE(result);

    PackFile* packFile = resourceSystem.GetPackFile();
    ASSERT_TRUE(packFile);
}

UTEST_F_TEARDOWN(test_ResourceSystem)
{
    ResourceSystem& resourceSystem = ResourceSystem::GetInstance();
    resourceSystem.UnmountPackFile();
}

UTEST(test_ResourceSystem, LoadNonExistentPackFile)
{
    ResourceSystem& resourceSystem = ResourceSystem::GetInstance();
    bool result = resourceSystem.MountPackFile("assets/nopackfile");
    ASSERT_FALSE(result);

    PackFile* packFile = resourceSystem.GetPackFile();
    ASSERT_FALSE(packFile);
}

UTEST_F(test_ResourceSystem, ReadAllPackedEntries)
{
    ResourceSystem& resourceSystem = ResourceSystem::GetInstance();
    PackFile* packFile = resourceSystem.GetPackFile();
    ASSERT_EQ(6, packFile->GetEntries().size());

    const PackFile::Header& header = packFile->GetHeader();
    ASSERT_STREQ("pck", header.magic.string);
    ASSERT_EQ(48681, header.bodySize);
    ASSERT_EQ(48489, header.tocOffset);

    std::vector<String> packedFilepaths {"assets/scene2.scene",
                                         "assets/scene1.scene",
                                         "assets/garbage.scene",
                                         "assets/cube.sm",
                                         "assets/cappy.png",
                                         "assets/PublicPixel.ttf"};

    for (const String& filepath : packedFilepaths)
    {
        auto it = packFile->GetEntries().find(filepath.Str());
        ASSERT_TRUE(it != packFile->GetEntries().end());
    }
}

UTEST_F(test_ResourceSystem, LoadNonExistentData)
{
    ResourceSystem& resourceSystem = ResourceSystem::GetInstance();
    PackFile* packFile = resourceSystem.GetPackFile();

    std::shared_ptr<PackFileData> data = packFile->FindData<PackFileData>("assets/nonexistent.filetype");
    ASSERT_FALSE(data);
}

UTEST_F(test_ResourceSystem, LoadPackFileData)
{
    ResourceSystem& resourceSystem = ResourceSystem::GetInstance();
    PackFile* packFile = resourceSystem.GetPackFile();

    std::shared_ptr<PackFileData> data = packFile->FindData<PackFileData>("assets/PublicPixel.ttf");
    ASSERT_TRUE(data);
    ASSERT_EQ(97456, data->dataSize);
}

UTEST_F(test_ResourceSystem, LoadStaticMeshData)
{
    ResourceSystem& resourceSystem = ResourceSystem::GetInstance();
    PackFile* packFile = resourceSystem.GetPackFile();

    std::shared_ptr<StaticMeshData> data = packFile->FindData<StaticMeshData>("assets/cube.sm");
    ASSERT_TRUE(data);
    ASSERT_EQ(36, data->indicesCount);
    ASSERT_EQ(24, data->verticesCount);
    ASSERT_EQ(1312, StaticMeshData::GetDataSize(data.get()));

    uint32_t expectedIndices[36] {0, 1,  2, 3, 4,  5, 6, 7,  8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                                  0, 18, 1, 3, 19, 4, 6, 20, 7, 9, 21, 10, 12, 22, 13, 15, 23, 16};
    uint32_t* actualIndices = data->GetIndices();
    ASSERT_TRUE(actualIndices);

    bool indicesCorrect = true;
    for (int i = 0; i < 36; i++)
    {
        if (expectedIndices[i] != actualIndices[i])
        {
            indicesCorrect = false;
        }
    }
    ASSERT_TRUE(indicesCorrect);

    BaseVertex expectedVertices[24] {{{-1, -1, 1}, {1, 1, 1, 1}, {0, -1, 0}, {0.875, 0.5}},
                                     {{1, -1, -1}, {1, 1, 1, 1}, {0, -1, 0}, {0.625, 0.75}},
                                     {{1, -1, 1}, {1, 1, 1, 1}, {0, -1, 0}, {0.625, 0.5}},
                                     {{1, -1, -1}, {1, 1, 1, 1}, {0, 0, -1}, {0.625, 0.75}},
                                     {{-1, 1, -1}, {1, 1, 1, 1}, {0, 0, -1}, {0.375, 1}},
                                     {{1, 1, -1}, {1, 1, 1, 1}, {0, 0, -1}, {0.375, 0.75}},
                                     {{-1, -1, -1}, {1, 1, 1, 1}, {-1, 0, 0}, {0.625, 0}},
                                     {{-1, 1, 1}, {1, 1, 1, 1}, {-1, 0, 0}, {0.375, 0.25}},
                                     {{-1, 1, -1}, {1, 1, 1, 1}, {-1, 0, 0}, {0.375, 0}},
                                     {{1, 1, 1}, {1, 1, 1, 1}, {0, 1, 0}, {0.375, 0.5}},
                                     {{-1, 1, -1}, {1, 1, 1, 1}, {0, 1, 0}, {0.125, 0.75}},
                                     {{-1, 1, 1}, {1, 1, 1, 1}, {0, 1, 0}, {0.125, 0.5}},
                                     {{1, -1, 1}, {1, 1, 1, 1}, {1, 0, 0}, {0.625, 0.5}},
                                     {{1, 1, -1}, {1, 1, 1, 1}, {1, 0, 0}, {0.375, 0.75}},
                                     {{1, 1, 1}, {1, 1, 1, 1}, {1, 0, 0}, {0.375, 0.5}},
                                     {{-1, -1, 1}, {1, 1, 1, 1}, {0, 0, 1}, {0.625, 0.25}},
                                     {{1, 1, 1}, {1, 1, 1, 1}, {0, 0, 1}, {0.375, 0.5}},
                                     {{-1, 1, 1}, {1, 1, 1, 1}, {0, 0, 1}, {0.375, 0.25}},
                                     {{-1, -1, -1}, {1, 1, 1, 1}, {0, -1, 0}, {0.875, 0.75}},
                                     {{-1, -1, -1}, {1, 1, 1, 1}, {0, 0, -1}, {0.625, 1}},
                                     {{-1, -1, 1}, {1, 1, 1, 1}, {-1, 0, 0}, {0.625, 0.25}},
                                     {{1, 1, -1}, {1, 1, 1, 1}, {0, 1, 0}, {0.375, 0.75}},
                                     {{1, -1, -1}, {1, 1, 1, 1}, {1, 0, 0}, {0.625, 0.75}},
                                     {{1, -1, 1}, {1, 1, 1, 1}, {0, 0, 1}, {0.625, 0.5}}};
    BaseVertex* actualVertices = data->GetVertices();
    ASSERT_TRUE(actualVertices);

    bool verticesCorrect = true;
    for (int i = 0; i < 24; i++)
    {
        const BaseVertex& expectedVertex = expectedVertices[i];
        const BaseVertex& actualVertex = actualVertices[i];
        if (expectedVertex != actualVertex)
        {
            verticesCorrect = false;
        }
    }
    ASSERT_TRUE(verticesCorrect);
}

UTEST_F(test_ResourceSystem, LoadTextureData)
{
    ResourceSystem& resourceSystem = ResourceSystem::GetInstance();
    PackFile* packFile = resourceSystem.GetPackFile();

    std::shared_ptr<Texture2DData> data = packFile->FindData<Texture2DData>("assets/cappy.png");
    ASSERT_TRUE(data);
    ASSERT_EQ(160000, data->GetImageSize());

    ASSERT_EQ(160000, data->pixels.size());
}

UTEST_F(test_ResourceSystem, LoadSceneData)
{
    ResourceSystem& resourceSystem = ResourceSystem::GetInstance();
    PackFile* packFile = resourceSystem.GetPackFile();

    const char* expectedSceneData = "TYPE:TestEntity;"
                                    "POSITION:0.000000,0.000000,0.000000;"
                                    "ROTATION:0.000000;"
                                    "Z_INDEX:0;"
                                    "|"
                                    "TYPE:TestEntity;"
                                    "POSITION:0.000000,0.000000,0.000000;"
                                    "ROTATION:0.000000;"
                                    "Z_INDEX:0;"
                                    "|"
                                    "TYPE:TestEntity;"
                                    "POSITION:0.000000,0.000000,0.000000;"
                                    "ROTATION:0.000000;"
                                    "Z_INDEX:0;"
                                    "|";

    std::shared_ptr<PackFileData> data = packFile->FindData<PackFileData>("assets/scene1.scene");
    ASSERT_TRUE(data);
    ASSERT_STREQ(expectedSceneData, "Fixme");
}