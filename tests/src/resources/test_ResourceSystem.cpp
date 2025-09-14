//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <resources/AnimationData.h>
#include <resources/PackFile.h>
#include <resources/PackFileData.h>
#include <resources/ResourceSystem.h>
#include <resources/SceneData.h>
#include <resources/SkeletalMeshData.h>
#include <resources/StaticMeshData.h>
#include <resources/Texture2DData.h>
#include <utest.h>

#include <random>

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
    ASSERT_EQ(8, packFile->GetEntries().size());

    const PackFile::Header& header = packFile->GetHeader();
    ASSERT_STREQ("pck", header.magic.string);
    ASSERT_EQ(49643, header.bodySize);
    ASSERT_EQ(49386, header.tocOffset);

    std::vector<String> packedFilepaths {"assets/scene2.scene",
                                         "assets/scene1.scene",
                                         "assets/garbage.scene",
                                         "assets/cube.sm",
                                         "assets/cube_anim.sk",
                                         "assets/cube_anim.ska",
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
    for (int i = 0; i < 36; i++)
    {
        ASSERT_EQ(expectedIndices[i], actualIndices[i]);
    }

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

    for (int i = 0; i < 24; i++)
    {
        const BaseVertex& expectedVertex = expectedVertices[i];
        const BaseVertex& actualVertex = data->vertices[i];
        ASSERT_NEAR(expectedVertex.position.x, actualVertex.position.x, 0.0001);
        ASSERT_NEAR(expectedVertex.position.y, actualVertex.position.y, 0.0001);
        ASSERT_NEAR(expectedVertex.position.z, actualVertex.position.z, 0.0001);
        ASSERT_NEAR(expectedVertex.color.r, actualVertex.color.r, 0.0001);
        ASSERT_NEAR(expectedVertex.color.g, actualVertex.color.g, 0.0001);
        ASSERT_NEAR(expectedVertex.color.b, actualVertex.color.b, 0.0001);
        ASSERT_NEAR(expectedVertex.color.a, actualVertex.color.a, 0.0001);
        ASSERT_NEAR(expectedVertex.normal.x, actualVertex.normal.x, 0.0001);
        ASSERT_NEAR(expectedVertex.normal.y, actualVertex.normal.y, 0.0001);
        ASSERT_NEAR(expectedVertex.normal.z, actualVertex.normal.z, 0.0001);
        ASSERT_NEAR(expectedVertex.uv.x, actualVertex.uv.x, 0.0001);
        ASSERT_NEAR(expectedVertex.uv.y, actualVertex.uv.y, 0.0001);
    }
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

    uint32_t expectedIndices[60] {0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,  8,  9,  10,
                                  8,  10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19,
                                  20, 21, 22, 20, 22, 23, 24, 25, 21, 24, 21, 20, 26, 27, 17,
                                  26, 17, 16, 28, 29, 9,  28, 9,  8,  30, 31, 1,  30, 1,  0};
    uint32_t* actualIndices = data->indices.data();
    ASSERT_TRUE(actualIndices);
    for (int i = 0; i < 36; i++)
    {
        ASSERT_EQ(expectedIndices[i], actualIndices[i]);
    }

    SkinnedVertex expectedVertices[32] {{{-0.15, -0.15, 0.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {-1.00, 0.00, 0.00},
                                         {1.00, 1.00},
                                         {0.00, 1.00, -1.00, -1.00},
                                         {0.50, 0.50, 0.00, 0.00}},
                                        {{-0.15, 0.15, 0.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {-1.00, 0.00, 0.00},
                                         {1.00, 1.00},
                                         {0.00, 1.00, -1.00, -1.00},
                                         {0.50, 0.50, 0.00, 0.00}},
                                        {{-0.15, 0.15, -1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {-1.00, 0.00, 0.00},
                                         {1.00, 1.00},
                                         {1.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{-0.15, -0.15, -1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {-1.00, 0.00, 0.00},
                                         {1.00, 1.00},
                                         {1.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{-0.15, -0.15, -1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {0.00, 0.00, -1.00},
                                         {1.00, 1.00},
                                         {1.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{-0.15, 0.15, -1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {0.00, 0.00, -1.00},
                                         {1.00, 1.00},
                                         {1.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{0.15, 0.15, -1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {0.00, 0.00, -1.00},
                                         {1.00, 1.00},
                                         {1.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{0.15, -0.15, -1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {0.00, 0.00, -1.00},
                                         {1.00, 1.00},
                                         {1.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{0.15, -0.15, 0.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {1.00, 0.00, 0.00},
                                         {1.00, 1.00},
                                         {0.00, 1.00, -1.00, -1.00},
                                         {0.50, 0.50, 0.00, 0.00}},
                                        {{0.15, 0.15, 0.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {1.00, 0.00, 0.00},
                                         {1.00, 1.00},
                                         {0.00, 1.00, -1.00, -1.00},
                                         {0.50, 0.50, 0.00, 0.00}},
                                        {{0.15, 0.15, 1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {1.00, 0.00, 0.00},
                                         {1.00, 1.00},
                                         {0.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{0.15, -0.15, 1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {1.00, 0.00, 0.00},
                                         {1.00, 1.00},
                                         {0.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{0.15, -0.15, 1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {0.00, 0.00, 1.00},
                                         {1.00, 1.00},
                                         {0.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{0.15, 0.15, 1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {0.00, 0.00, 1.00},
                                         {1.00, 1.00},
                                         {0.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{-0.15, 0.15, 1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {0.00, 0.00, 1.00},
                                         {1.00, 1.00},
                                         {0.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{-0.15, -0.15, 1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {0.00, 0.00, 1.00},
                                         {1.00, 1.00},
                                         {0.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{-0.15, -0.15, 0.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {0.00, -1.00, 0.00},
                                         {1.00, 1.00},
                                         {0.00, 1.00, -1.00, -1.00},
                                         {0.50, 0.50, 0.00, 0.00}},
                                        {{0.15, -0.15, 0.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {0.00, -1.00, 0.00},
                                         {1.00, 1.00},
                                         {0.00, 1.00, -1.00, -1.00},
                                         {0.50, 0.50, 0.00, 0.00}},
                                        {{0.15, -0.15, 1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {0.00, -1.00, 0.00},
                                         {1.00, 1.00},
                                         {0.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{-0.15, -0.15, 1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {0.00, -1.00, 0.00},
                                         {1.00, 1.00},
                                         {0.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{0.15, 0.15, 0.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {0.00, 1.00, 0.00},
                                         {1.00, 1.00},
                                         {0.00, 1.00, -1.00, -1.00},
                                         {0.50, 0.50, 0.00, 0.00}},
                                        {{-0.15, 0.15, 0.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {0.00, 1.00, 0.00},
                                         {1.00, 1.00},
                                         {0.00, 1.00, -1.00, -1.00},
                                         {0.50, 0.50, 0.00, 0.00}},
                                        {{-0.15, 0.15, 1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {0.00, 1.00, 0.00},
                                         {1.00, 1.00},
                                         {0.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{0.15, 0.15, 1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {0.00, 1.00, 0.00},
                                         {1.00, 1.00},
                                         {0.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{0.15, 0.15, -1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {0.00, 1.00, 0.00},
                                         {1.00, 1.00},
                                         {1.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{-0.15, 0.15, -1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {0.00, 1.00, 0.00},
                                         {1.00, 1.00},
                                         {1.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{-0.15, -0.15, -1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {0.00, -1.00, 0.00},
                                         {1.00, 1.00},
                                         {1.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{0.15, -0.15, -1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {0.00, -1.00, 0.00},
                                         {1.00, 1.00},
                                         {1.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{0.15, -0.15, -1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {1.00, 0.00, 0.00},
                                         {1.00, 1.00},
                                         {1.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{0.15, 0.15, -1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {1.00, 0.00, 0.00},
                                         {1.00, 1.00},
                                         {1.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{-0.15, -0.15, 1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {-1.00, 0.00, 0.00},
                                         {1.00, 1.00},
                                         {0.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}},
                                        {{-0.15, 0.15, 1.00},
                                         {1.00, 1.00, 1.00, 1.00},
                                         {-1.00, 0.00, 0.00},
                                         {1.00, 1.00},
                                         {0.00, -1.00, -1.00, -1.00},
                                         {1.00, 0.00, 0.00, 0.00}}};
    ASSERT_FALSE(data->vertices.empty());

    for (int i = 0; i < 32; i++)
    {
        const SkinnedVertex& expectedVertex = expectedVertices[i];
        const SkinnedVertex& actualVertex = data->vertices[i];
        ASSERT_NEAR(expectedVertex.position.x, actualVertex.position.x, 0.01);
        ASSERT_NEAR(expectedVertex.position.y, actualVertex.position.y, 0.01);
        ASSERT_NEAR(expectedVertex.position.z, actualVertex.position.z, 0.01);
        ASSERT_NEAR(expectedVertex.color.r, actualVertex.color.r, 0.0001);
        ASSERT_NEAR(expectedVertex.color.g, actualVertex.color.g, 0.0001);
        ASSERT_NEAR(expectedVertex.color.b, actualVertex.color.b, 0.0001);
        ASSERT_NEAR(expectedVertex.color.a, actualVertex.color.a, 0.0001);
        ASSERT_NEAR(expectedVertex.normal.x, actualVertex.normal.x, 0.0001);
        ASSERT_NEAR(expectedVertex.normal.y, actualVertex.normal.y, 0.0001);
        ASSERT_NEAR(expectedVertex.normal.z, actualVertex.normal.z, 0.0001);
        ASSERT_NEAR(expectedVertex.uv.x, actualVertex.uv.x, 0.0001);
        ASSERT_NEAR(expectedVertex.uv.y, actualVertex.uv.y, 0.0001);
        ASSERT_NEAR(expectedVertex.bones.x, actualVertex.bones.x, 0.0001);
        ASSERT_NEAR(expectedVertex.bones.y, actualVertex.bones.y, 0.0001);
        ASSERT_NEAR(expectedVertex.bones.z, actualVertex.bones.z, 0.0001);
        ASSERT_NEAR(expectedVertex.bones.w, actualVertex.bones.w, 0.0001);
        ASSERT_NEAR(expectedVertex.weights.x, actualVertex.weights.x, 0.0001);
        ASSERT_NEAR(expectedVertex.weights.y, actualVertex.weights.y, 0.0001);
        ASSERT_NEAR(expectedVertex.weights.z, actualVertex.weights.z, 0.0001);
        ASSERT_NEAR(expectedVertex.weights.w, actualVertex.weights.w, 0.0001);
    }

    ASSERT_EQ(2, data->bones.size());
    {
        auto it = data->bones.find("Bone.Root");
        ASSERT_TRUE(it != data->bones.end());
        ASSERT_EQ(0, it->second.id);
        Mat4 expectedMatrix = {{1.00, -0.00, 0.00, -0.00},
                               {-0.00, 0.00, 1.00, 0.00},
                               {0.00, -1.00, 0.00, -0.00},
                               {-0.00, 1.00, -0.00, 1.00}};
        ASSERT_TRUE(expectedMatrix == it->second.bindMatrix);
    }

    {
        auto it = data->bones.find("Bone.Leaf");
        ASSERT_TRUE(it != data->bones.end());
        Mat4 expectedMatrix = {{1.00, -0.00, 0.00, -0.00},
                               {-0.00, 0.00, 1.00, 0.00},
                               {0.00, -1.00, 0.00, -0.00},
                               {-0.00, 0.00, -0.00, 1.00}};
        ASSERT_TRUE(expectedMatrix == it->second.bindMatrix);
    }
}

UTEST_F(test_ResourceSystem, LoadAnimationData)
{
    ResourceSystem& resourceSystem = ResourceSystem::GetInstance();
    PackFile* packFile = resourceSystem.GetPackFile();

    std::shared_ptr<AnimationData> data =
        packFile->FindDataDeserialised<AnimationData>("assets/cube_anim.ska");
    ASSERT_TRUE(data);
    ASSERT_NEAR(833.333, data->length, 0.001);
    ASSERT_NEAR(1000.000, data->speed, 0.001);
    ASSERT_EQ(2, data->channels.size());

    {
        auto it = data->channels.find("Bone.Root");
        ASSERT_TRUE(it != data->channels.end());

        ASSERT_EQ(2, it->second.positionKeys.size());
        ASSERT_EQ(2, it->second.scaleKeys.size());

        ASSERT_EQ(20, it->second.rotationKeys.size());

        AnimKeyRotation expectedRotationKeys[20] {
            {41.66666793823242188, {-0.71, 0.00, 0.00, 0.71}},
            {83.33333587646484375, {-0.70, 0.00, 0.00, 0.71}},
            {125.00000000000000000, {-0.69, 0.00, 0.00, 0.72}},
            {166.66667175292968750, {-0.67, 0.00, 0.00, 0.74}},
            {208.33332824707031250, {-0.64, 0.00, 0.00, 0.77}},
            {250.00000000000000000, {-0.61, 0.00, 0.00, 0.79}},
            {291.66665649414062500, {-0.58, 0.00, 0.00, 0.82}},
            {333.33334350585937500, {-0.54, 0.00, 0.00, 0.84}},
            {375.00000000000000000, {-0.52, 0.00, 0.00, 0.85}},
            {416.66665649414062500, {-0.51, 0.00, 0.00, 0.86}},
            {458.33334350585937500, {-0.52, 0.00, 0.00, 0.85}},
            {500.00000000000000000, {-0.54, 0.00, 0.00, 0.84}},
            {541.66668701171875000, {-0.58, 0.00, 0.00, 0.82}},
            {583.33331298828125000, {-0.61, 0.00, 0.00, 0.79}},
            {625.00000000000000000, {-0.64, 0.00, 0.00, 0.77}},
            {666.66668701171875000, {-0.67, 0.00, 0.00, 0.74}},
            {708.33331298828125000, {-0.69, 0.00, 0.00, 0.73}},
            {750.00000000000000000, {-0.70, 0.00, 0.00, 0.72}},
            {791.66668701171875000, {-0.70, 0.00, 0.00, 0.71}},
            {833.33331298828125000, {-0.71, 0.00, 0.00, 0.71}}};
        for (int i = 0; i < 20; i++)
        {
            const AnimKeyRotation& expectedKey = expectedRotationKeys[i];
            const AnimKeyRotation& actualKey = it->second.rotationKeys[i];
            ASSERT_NEAR(expectedKey.timestamp, actualKey.timestamp, 0.0001);
            ASSERT_NEAR(expectedKey.rotation.x, actualKey.rotation.x, 0.01);
            ASSERT_NEAR(expectedKey.rotation.y, actualKey.rotation.y, 0.01);
            ASSERT_NEAR(expectedKey.rotation.z, actualKey.rotation.z, 0.01);
            ASSERT_NEAR(expectedKey.rotation.w, actualKey.rotation.w, 0.01);
        }
    }

    {
        auto it = data->channels.find("Bone.Leaf");
        ASSERT_TRUE(it != data->channels.end());

        ASSERT_EQ(2, it->second.positionKeys.size());
        ASSERT_EQ(2, it->second.scaleKeys.size());

        AnimKeyRotation expectedRotationKeys[20] {
            {41.66666793823242188, {0.00, 0.00, 0.00, 1.00}},
            {83.33333587646484375, {0.02, 0.00, 0.00, 1.00}},
            {125.00000000000000000, {0.07, 0.00, 0.00, 1.00}},
            {166.66667175292968750, {0.14, 0.00, 0.00, 0.99}},
            {208.33332824707031250, {0.22, 0.00, 0.00, 0.98}},
            {250.00000000000000000, {0.29, 0.00, 0.00, 0.96}},
            {291.66665649414062500, {0.36, 0.00, 0.00, 0.93}},
            {333.33334350585937500, {0.41, 0.00, 0.00, 0.91}},
            {375.00000000000000000, {0.44, 0.00, 0.00, 0.90}},
            {416.66665649414062500, {0.46, 0.00, 0.00, 0.89}},
            {458.33334350585937500, {0.44, 0.00, 0.00, 0.90}},
            {500.00000000000000000, {0.41, 0.00, 0.00, 0.91}},
            {541.66668701171875000, {0.35, 0.00, 0.00, 0.94}},
            {583.33331298828125000, {0.29, 0.00, 0.00, 0.96}},
            {625.00000000000000000, {0.22, 0.00, 0.00, 0.98}},
            {666.66668701171875000, {0.15, 0.00, 0.00, 0.99}},
            {708.33331298828125000, {0.09, 0.00, 0.00, 1.00}},
            {750.00000000000000000, {0.04, 0.00, 0.00, 1.00}},
            {791.66668701171875000, {0.01, 0.00, 0.00, 1.00}},
            {833.33331298828125000, {0.00, 0.00, 0.00, 1.00}}};
        for (int i = 0; i < 20; i++)
        {
            const AnimKeyRotation& expectedKey = expectedRotationKeys[i];
            const AnimKeyRotation& actualKey = it->second.rotationKeys[i];
            ASSERT_NEAR(expectedKey.timestamp, actualKey.timestamp, 0.0001);
            ASSERT_NEAR(expectedKey.rotation.x, actualKey.rotation.x, 0.01);
            ASSERT_NEAR(expectedKey.rotation.y, actualKey.rotation.y, 0.01);
            ASSERT_NEAR(expectedKey.rotation.z, actualKey.rotation.z, 0.01);
            ASSERT_NEAR(expectedKey.rotation.w, actualKey.rotation.w, 0.01);
        }
    }
}
