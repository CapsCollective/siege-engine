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
#include <resources/Texture2DData.h>
#include <utest.h>

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
    ASSERT_EQ(7, packFile->GetEntries().size());

    const PackFile::Header& header = packFile->GetHeader();
    ASSERT_STREQ("pck", header.magic.string);
    ASSERT_EQ(48951, header.bodySize);
    ASSERT_EQ(48727, header.tocOffset);

    std::vector<String> packedFilepaths {"assets/scene2.scene",
                                         "assets/scene1.scene",
                                         "assets/garbage.scene",
                                         "assets/cube.sm",
                                         "assets/cube_anim.sk",
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

    std::shared_ptr<PackFileData> data = packFile->FindData("assets/nonexistent.filetype");
    ASSERT_FALSE(data);
}

UTEST_F(test_ResourceSystem, LoadPackFileData)
{
    ResourceSystem& resourceSystem = ResourceSystem::GetInstance();
    PackFile* packFile = resourceSystem.GetPackFile();

    std::shared_ptr<PackFileData> data = packFile->FindData("assets/PublicPixel.ttf");
    ASSERT_TRUE(data);
    ASSERT_EQ(97456, data->dataSize);
}

UTEST_F(test_ResourceSystem, LoadStaticMeshData)
{
    ResourceSystem& resourceSystem = ResourceSystem::GetInstance();
    PackFile* packFile = resourceSystem.GetPackFile();

    std::shared_ptr<StaticMeshData> data =
        packFile->FindDataDeserialised<StaticMeshData>("assets/cube.sm");
    ASSERT_TRUE(data);
    ASSERT_EQ(36, data->indices.size());
    ASSERT_EQ(24, data->vertices.size());

    uint32_t expectedIndices[36] {0, 1,  2, 3, 4,  5, 6, 7,  8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                                  0, 18, 1, 3, 19, 4, 6, 20, 7, 9, 21, 10, 12, 22, 13, 15, 23, 16};
    uint32_t* actualIndices = data->indices.data();
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
    ASSERT_FALSE(data->vertices.empty());

    bool verticesCorrect = true;
    for (int i = 0; i < 24; i++)
    {
        const BaseVertex& expectedVertex = expectedVertices[i];
        const BaseVertex& actualVertex = data->vertices[i];
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

    std::shared_ptr<Texture2DData> data =
        packFile->FindDataDeserialised<Texture2DData>("assets/cappy.png");
    ASSERT_TRUE(data);
    ASSERT_EQ(160000, data->GetImageSize());
    ASSERT_EQ(160000, data->pixels.size());
}

UTEST_F(test_ResourceSystem, LoadSceneData)
{
    ResourceSystem& resourceSystem = ResourceSystem::GetInstance();
    PackFile* packFile = resourceSystem.GetPackFile();

    std::vector<String> expectedSceneData = {"TYPE:TestEntity;"
                                             "POSITION:0.000000,0.000000,0.000000;"
                                             "ROTATION:0.000000;"
                                             "Z_INDEX:0;",
                                             "TYPE:TestEntity;"
                                             "POSITION:0.000000,0.000000,0.000000;"
                                             "ROTATION:0.000000;"
                                             "Z_INDEX:0;",
                                             "TYPE:TestEntity;"
                                             "POSITION:0.000000,0.000000,0.000000;"
                                             "ROTATION:0.000000;"
                                             "Z_INDEX:0;"};

    std::shared_ptr<SceneData> data =
        packFile->FindDataDeserialised<SceneData>("assets/scene1.scene");
    ASSERT_TRUE(data);
    ASSERT_EQ(expectedSceneData.size(), data->entities.size());
    for (int i = 0; i < expectedSceneData.size(); ++i)
    {
        const String& expectedEntityData = expectedSceneData[i];
        const String& actualEntityData = data->entities[i];
        ASSERT_STREQ(expectedEntityData.Str(), actualEntityData.Str());
    }
}

UTEST_F(test_ResourceSystem, LoadSkeletalMeshData)
{
    ResourceSystem& resourceSystem = ResourceSystem::GetInstance();
    PackFile* packFile = resourceSystem.GetPackFile();

    std::shared_ptr<SkeletalMeshData> data =
        packFile->FindDataDeserialised<SkeletalMeshData>("assets/cube_anim.sk");
    ASSERT_TRUE(data);
    ASSERT_EQ(60, data->indices.size());
    ASSERT_EQ(32, data->vertices.size());

    uint32_t expectedIndices[60] {0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13,
                                  14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23,
                                  24, 25, 21, 24, 21, 20, 26, 27, 17, 26, 17, 16, 28, 29, 9, 28, 9,
                                  8, 30, 31, 1, 30, 1, 0};
    uint32_t* actualIndices = data->indices.data();
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
    SkinnedVertex expectedVertices[32] {
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {-1.00,0.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {-1.00,0.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {-1.00,0.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {-1.00,0.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {0.00,0.00,-1.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {0.00,0.00,-1.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {0.00,0.00,-1.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {0.00,0.00,-1.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {1.00,0.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {1.00,0.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {1.00,0.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {1.00,0.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {0.00,0.00,1.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {0.00,0.00,1.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {0.00,0.00,1.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {0.00,0.00,1.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {0.00,-1.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {0.00,-1.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {0.00,-1.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {0.00,-1.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {0.00,1.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {0.00,1.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {0.00,1.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {0.00,1.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {0.00,1.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {0.00,1.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {0.00,-1.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {0.00,-1.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {1.00,0.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {1.00,0.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {-1.00,0.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}},
        {{{0.00,0.00,0.00}, {1.00,1.00,1.00,1.00}, {-1.00,0.00,0.00}, {1.00,1.00}},{-1.00,-1.00,-1.00,-1.00},{0.00,0.00,0.00,0.00}}
    };
    ASSERT_FALSE(data->vertices.empty());

    bool verticesCorrect = true;
    for (int i = 0; i < 32; i++)
    {
        const SkinnedVertex& expectedVertex = expectedVertices[i];
        const SkinnedVertex& actualVertex = data->vertices[i];
        if (expectedVertex != actualVertex)
        {
            verticesCorrect = false;
        }
    }
    ASSERT_TRUE(verticesCorrect);
}
