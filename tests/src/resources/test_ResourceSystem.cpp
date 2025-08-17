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
struct TestStruct
{
    uint64_t inta {0};
    uint64_t intb {0};
};

struct FreeBlockNode
{
    FreeBlockNode* prev {nullptr};
    FreeBlockNode* next {nullptr};
};

struct SmallBlockHeader
{
    uint16_t sizeAndFlags {0};
};

struct SmallBlockFooter
{
    uint16_t sizeAndFlags {0};
};

struct BlockHeader
{
    // bitmask where the first three bits are flags:
    // 0 = is the block free
    // 1 = is the previous block free
    // 2 = padding
    uint32_t sizeAndFlags {0};
};

struct BlockFooter
{
    uint32_t totalBlockSize {0};
};

struct LargeBlockHeader
{
    uint64_t sizeAndFlags {0};
};

struct LargeBlockFooter
{
    uint64_t sizeAndFlags {0};
};

#define FL_MIN 4
#define FL_INDEX(capacity) 63 - __builtin_clzll(capacity)
#define MAX_SL_INDEX 16
#define MIN_ALLOC_SIZE 16

#define TO_BYTES(val) ((unsigned char*)val)
#define TO_HBLOCK(val) ((BlockHeader*)val)
#define TO_FBLOCK(val) ((BlockFooter*)val)
#define TO_FREEBLOCK(val) ((FreeBlockNode*)val)

#define SL_BITS 4 // we get the log(2) the max sl index
#define SL_MASK ((1 << SL_BITS) - 1)

#define FLAG_BITS 3
#define IS_FREE_FLAG 1
#define IS_PREV_FREE_FLAG 2

#define INVALID_INDEX SIZE_T_MAX

class Allocator
{
public:
    Allocator() {};
    Allocator(const size_t capacity)
        : capacity {capacity}, bytesRemaining {capacity}
    {
        data = TO_BYTES(calloc(1, sizeof(BlockHeader) + sizeof(BlockFooter) + capacity));

        size_t fl = 0, sl = 0, index;

        CalculateIndices(capacity, fl, sl ,index);

        uint64_t maxIndices = fl + 1;

        freeList = (FreeBlockNode**)calloc(maxIndices * MAX_SL_INDEX, sizeof(FreeBlockNode*));
        slBitmasks = (uint16_t*)calloc(maxIndices, sizeof(uint16_t));

        BlockHeader* firstHeader = (BlockHeader*)data;
        firstHeader->sizeAndFlags = (capacity << FLAG_BITS) | IS_FREE_FLAG;

        FreeBlockNode* firstBlock = TO_FREEBLOCK((data+sizeof(BlockHeader)));

        BlockFooter* firstFooter = TO_FBLOCK(TO_BYTES(firstBlock)+capacity);
        firstFooter->totalBlockSize = capacity;
        freeList[index] = firstBlock;

        flBitmask = SetBitOn64(flBitmask, fl);
        slBitmasks[fl] = SetBitOn(slBitmasks[fl], sl);
    }

    ~Allocator()
    {
        if (data) free(data);
        if (freeList) free(freeList);
        if (slBitmasks) free(slBitmasks);

        capacity = 0;
        data = nullptr;
        freeList = nullptr;
        slBitmasks = nullptr;
    }

    void* Allocate(size_t size)
    {
        size_t requiredSize = sizeof(BlockHeader) + size + sizeof(BlockFooter);
        if (!data || capacity == 0 ||size == 0
            || requiredSize < size || requiredSize > bytesRemaining) return nullptr;

        if (requiredSize < MIN_ALLOC_SIZE) return nullptr;

        size_t freeListIdx = 0, fl = 0, sl = 0;

        FreeBlockNode* block = FindFreeBlock(requiredSize, fl, sl, freeListIdx);

        if (freeListIdx == INVALID_INDEX) return nullptr;

        BlockHeader* header = GetHeader(block);

        TrySplitBlock(header, block, requiredSize, fl, sl, freeListIdx);

        BlockHeader* newHeader = TO_HBLOCK(TO_BYTES(header));

        size_t flags = 0;
        TrySetPreviousHeaderFlag(newHeader, flags);

        newHeader->sizeAndFlags = (requiredSize << FLAG_BITS) | flags;

        unsigned char* ptr = TO_BYTES(TO_BYTES(header)+sizeof(BlockHeader));

        BlockFooter* footer = TO_FBLOCK((ptr+size));
        footer->totalBlockSize = requiredSize;

        bytesRemaining -= requiredSize;

        return ptr;
    }

    template<typename T>
    void Deallocate(T*& ptr)
    {
        if (!ptr) return;
        if ((TO_BYTES(ptr) < data || TO_BYTES(ptr) >= (data + capacity))) return;

        BlockHeader* header = TO_HBLOCK((TO_BYTES(ptr) - sizeof(BlockHeader)));
        size_t blockSize = GetHeaderSize(header);

        size_t totalBlockSize = blockSize;
        BlockHeader* targetHeader = TryCoalesce(header, totalBlockSize);
        BlockFooter* footer = TO_FBLOCK((TO_BYTES(targetHeader) + totalBlockSize - sizeof(BlockFooter)));

        targetHeader->sizeAndFlags = (totalBlockSize << FLAG_BITS) | (targetHeader->sizeAndFlags & IS_PREV_FREE_FLAG) | IS_FREE_FLAG;
        footer->totalBlockSize = totalBlockSize;

        BlockHeader* nextHeader = GetNextHeader(targetHeader, totalBlockSize);
        if (nextHeader) nextHeader->sizeAndFlags |= IS_PREV_FREE_FLAG;

        CreateNewBlock(TO_BYTES(targetHeader), totalBlockSize);

        bytesRemaining += blockSize;

        ptr = nullptr;
    }

    size_t GetNextFreeSlotIndex(OUT size_t& fl,OUT size_t& sl)
    {
        sl = FindLargerSlots(slBitmasks[fl], sl);
        if (sl) return fl * MAX_SL_INDEX + __builtin_ctz(sl);

        fl = FindLargerSlots64(flBitmask, fl);

        if (!fl) return INVALID_INDEX;

        fl = __builtin_ctzll(fl);
        CC_ASSERT(slBitmasks[fl] > 0,
                  "SlBitmasks is returning 0. This should not be happening and indicates an implementation error.")

        sl = __builtin_ctz(slBitmasks[fl]);

        return fl * MAX_SL_INDEX + sl;
    }

    bool IsFree(size_t fl, size_t sl) { return Mask(slBitmasks[fl], BitMask(sl)); }
    const size_t FindLargerSlots(size_t mask, size_t idx) { return Mask(mask, FlipBits(BitMask(idx+1)-1)); }
    const size_t FindLargerSlots64(size_t mask, size_t idx) { return Mask(mask, FlipBits(BitMask64(idx+1)-1)); }
    const size_t BitMask(size_t shiftAmount) { return 1 << shiftAmount; }
    const size_t BitMask64(size_t shiftAmount) { return 1ULL << shiftAmount; }
    const size_t SetBitOn64(size_t val, size_t bit) { return val | BitMask64(bit);}
    const size_t SetBitOn(size_t val, size_t bit) { return val | BitMask(bit);}
    const size_t FlipBits(size_t mask) { return ~(mask); }
    const size_t Mask(size_t value, size_t mask) { return value & mask; }

    const size_t GetHeaderSize(BlockHeader* header) { return header->sizeAndFlags >> FLAG_BITS; }
    const size_t PrevBlockIsFree(BlockHeader* header) { return Mask(header->sizeAndFlags, IS_PREV_FREE_FLAG); }
    const size_t BlockIsFree(BlockHeader* header) { return Mask(header->sizeAndFlags, IS_FREE_FLAG); }
    BlockHeader* GetHeader(FreeBlockNode* node) { return TO_HBLOCK((TO_BYTES(node) - sizeof(BlockHeader))); }

    BlockHeader* TryCoalesce(BlockHeader* header, OUT size_t& size)
    {
        BlockHeader* start = header;
        BlockHeader* prev = GetPreviousHeader(start);

        if (prev && BlockIsFree(prev))
        {
            start = prev;
            size += GetHeaderSize(prev);
            RemoveFromFreeList(prev);
        }

        BlockHeader* next = GetNextHeader(start, size);

        if (next && BlockIsFree(next))
        {
            size += GetHeaderSize(next);
            RemoveFromFreeList(next);
        }

        return start;
    }

    BlockHeader* GetPreviousHeader(BlockHeader* currentHeader)
    {
        if (IsHead(currentHeader)) return nullptr;
        BlockFooter* prev = TO_FBLOCK((TO_BYTES(currentHeader) - sizeof(BlockFooter)));
        BlockHeader* prevHeader = TO_HBLOCK((TO_BYTES(currentHeader) - prev->totalBlockSize));
        return prevHeader;
    }

    BlockHeader* GetNextHeader(void* currentHeader, size_t offset)
    {
        unsigned char* next = TO_BYTES(currentHeader) + offset;
        if (next >= (data+capacity)) return nullptr;
        if (IsTail(next)) return nullptr;
        return TO_HBLOCK(next);
    }

    void TrySetPreviousHeaderFlag(BlockHeader* currentHeader, size_t& flags)
    {
        BlockHeader* prevHeader = GetPreviousHeader(currentHeader);
        if (prevHeader &&
            Mask(prevHeader->sizeAndFlags,IS_FREE_FLAG)) flags |= IS_PREV_FREE_FLAG;
    }

    void TrySplitBlock(BlockHeader* header, FreeBlockNode* block, size_t size, size_t fl, size_t sl, size_t index)
    {
        size_t blockSize = GetHeaderSize(header);
        RemoveFromFreeList(block, fl, sl, index);
        if (blockSize <= size) return;

        CreateNewBlock(TO_BYTES(header) + size, blockSize - size);
    }

    void RemoveFromFreeList(FreeBlockNode* block, size_t fl, size_t sl, size_t index)
    {
        if (!block) return;
        if (block->prev) block->prev->next = block->next;
        if (block->next) block->next->prev = block->prev;

        freeList[index] = block->next;

        slBitmasks[fl] = Mask(slBitmasks[fl],FlipBits(BitMask(sl)));
        if (!slBitmasks[fl]) flBitmask = Mask(flBitmask,FlipBits(BitMask(fl)));
    }

    void RemoveFromFreeList(BlockHeader* block)
    {
        size_t size = GetHeaderSize(block);
        size_t fl, sl, index;

        CalculateIndices(size, fl, sl, index);

        FreeBlockNode* freeBlock = TO_FREEBLOCK((TO_BYTES(block) + sizeof(BlockHeader)));
        RemoveFromFreeList(freeBlock, fl, sl, index);
    }

    void CalculateIndices(size_t size, OUT size_t& fl, OUT size_t& sl, OUT size_t& index)
    {
        GetIndices(size, fl, sl);
        index = fl * MAX_SL_INDEX + sl;
    }

    void CreateNewBlock(unsigned char* ptr, size_t size)
    {
            size_t fl = 0,sl = 0, index;
            CalculateIndices(size, fl, sl, index);

            size_t sizeFlag = (size << FLAG_BITS);

            BlockHeader* header = TO_HBLOCK(ptr);
            header->sizeAndFlags = (sizeFlag | IS_FREE_FLAG);

            FreeBlockNode* block = TO_FREEBLOCK((TO_BYTES(header) + sizeof(BlockHeader)));

            BlockFooter* firstFooter = TO_FBLOCK((TO_BYTES(header)+(size-sizeof(BlockFooter))));
            firstFooter->totalBlockSize = size;

            block->next = freeList[index];
            block->prev = nullptr;

            if (block->next) block->next->prev = block;

            freeList[index] = block;

            flBitmask = SetBitOn64(flBitmask, fl);
            slBitmasks[fl] = SetBitOn(slBitmasks[fl], sl);
    }

    FreeBlockNode* FindFreeBlock(size_t size, OUT size_t& fl, OUT size_t& sl,  OUT size_t& index)
    {
        CalculateIndices(size, fl, sl, index);

        if (!IsFree(fl, sl)) index = GetNextFreeSlotIndex(fl, sl);
        if (index == INVALID_INDEX) return nullptr;

        return freeList[index];
    }

    void GetIndices(const size_t size, OUT size_t& fl, OUT size_t& sl)
    {
        size_t rawFl = FL_INDEX(size);
        sl = SlIndex(size, rawFl);
        fl = rawFl - FL_MIN;
    }

    bool IsHead(void* ptr) { return TO_BYTES(ptr) == data ;}
    bool IsTail(void* ptr) { return TO_BYTES(ptr) + sizeof(BlockHeader) >= (data + capacity); }
    size_t& Capacity() { return capacity;}
    unsigned char* Data() { return data; };
    size_t SlIndex(size_t size, size_t fl) { return (size >> (fl - SL_BITS)) & SL_MASK; }
    const uint64_t& FlBitmask() { return flBitmask; }
    uint16_t*& SlBitmask() { return slBitmasks; }
    FreeBlockNode** FreeList() { return freeList; }
    const size_t BytesRemaining() { return bytesRemaining; }

private:
    size_t capacity {0};
    size_t bytesRemaining {0};
    unsigned char* data {nullptr};
    FreeBlockNode** freeList {nullptr};

    // bitmasks
    uint64_t flBitmask {0};
    uint16_t* slBitmasks {nullptr};
};

UTEST(test_ResourceSystem, TestEmptyAllocatorCreation)
{
    Allocator a;
    ASSERT_EQ(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 0);
}

UTEST(test_ResourceSystem, TestAllocatorCreationWithSize)
{
    Allocator a(64);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 64);
    ASSERT_EQ(64, a.BytesRemaining());

    // 0001 0000
    ASSERT_EQ(a.FlBitmask(), 4);
    ASSERT_EQ(a.SlBitmask()[2], 1);

    FreeBlockNode* block = a.FreeList()[2 * 16];
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);

    BlockHeader* header = TO_HBLOCK((TO_BYTES(block) - sizeof(BlockHeader)));
    ASSERT_TRUE(header);
    ASSERT_EQ(64, a.GetHeaderSize(header));
    ASSERT_TRUE(a.BlockIsFree(header));
    ASSERT_FALSE(a.PrevBlockIsFree(header));

    BlockFooter* footer = TO_FBLOCK(TO_BYTES(block) + a.GetHeaderSize(header));
    ASSERT_TRUE(header);
    ASSERT_EQ(64, footer->totalBlockSize);
}

UTEST(test_ResourceSystem, TestAllocateFunction)
{
    Allocator a(64);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 64);
    ASSERT_EQ(64, a.BytesRemaining());

    TestStruct* p = (TestStruct*) a.Allocate(sizeof(TestStruct));

    p->inta = 10;
    p->intb = 20;

    ASSERT_EQ(10, p->inta);
    ASSERT_EQ(20, p->intb);

    ASSERT_EQ(a.FlBitmask(), 2);
    ASSERT_EQ(16, a.SlBitmask()[1]);

    FreeBlockNode* block = a.FreeList()[(1 * 16) + 4];
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);

    auto header = (BlockHeader*)a.Data();
    auto data = (TestStruct*) (((unsigned char*)header) + sizeof(BlockHeader));
    auto footer = (BlockFooter*) (((unsigned char*)data) + sizeof(TestStruct));

    ASSERT_EQ(192, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(BlockHeader) + sizeof(TestStruct) + sizeof(BlockFooter), footer->totalBlockSize);
    ASSERT_EQ(40, a.BytesRemaining());

    Siege::String* str = (Siege::String*) a.Allocate(sizeof(Siege::String));

    *str = "Hello There!";
    ASSERT_STREQ(str->Str(),"Hello There!");

    ASSERT_EQ(1, a.FlBitmask());
    ASSERT_EQ(0, a.SlBitmask()[1]);

    FreeBlockNode* NewBlock = a.FreeList()[0];
    ASSERT_EQ(NewBlock->next, nullptr);
    ASSERT_EQ(NewBlock->prev, nullptr);

    auto newHeader = (BlockHeader*)(TO_BYTES(footer) + sizeof(BlockFooter));
    auto newData = (Siege::String*) (((unsigned char*)newHeader) + sizeof(BlockHeader));
    auto newFooter = (BlockFooter*) (((unsigned char*)newData) + sizeof(TestStruct));

    ASSERT_EQ(192, newHeader->sizeAndFlags);
    ASSERT_EQ(newData, str);
    ASSERT_EQ(sizeof(BlockHeader) + sizeof(Siege::String) + sizeof(BlockFooter), newFooter->totalBlockSize);
    ASSERT_EQ(16, a.BytesRemaining());

    // Edge cases

    // Empty allocator
    Allocator emptyA;

    TestStruct* ptr = (TestStruct*) emptyA.Allocate(sizeof(TestStruct));
    ASSERT_FALSE(ptr);

    // Allocate 0 bytes
    void* emptyAllocPtr = a.Allocate(0);
    ASSERT_FALSE(emptyAllocPtr);

    // Allocate an amount that causes a value overflow
    Allocator overflowAlloc(SIZE_T_MAX);
    void* overflowPtr = overflowAlloc.Allocate(SIZE_T_MAX);
    ASSERT_FALSE(overflowPtr);

    void* tooLargeCase = a.Allocate(100);
    ASSERT_FALSE(tooLargeCase);

    Allocator tooSmallAlloc(64);
    void* tooLargeAlloc = a.Allocate(100);
    ASSERT_FALSE(tooLargeCase);
}

UTEST(test_ResourceSystem, TestDeallocateFunction)
{
    Allocator a(64);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 64);
    ASSERT_EQ(64, a.BytesRemaining());

    TestStruct* p = (TestStruct*) a.Allocate(sizeof(TestStruct));

    p->inta = 10;
    p->intb = 20;

    ASSERT_EQ(10, p->inta);
    ASSERT_EQ(20, p->intb);

    ASSERT_EQ(a.FlBitmask(), 2);
    ASSERT_EQ(16, a.SlBitmask()[1]);

    FreeBlockNode* block = a.FreeList()[(1 * 16) + 4];
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);
    ASSERT_EQ(40, a.BytesRemaining());

    auto header = (BlockHeader*)a.Data();
    auto data = (TestStruct*) (((unsigned char*)header) + sizeof(BlockHeader));
    auto footer = (BlockFooter*) (((unsigned char*)data) + sizeof(TestStruct));

    ASSERT_EQ(192, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(BlockHeader) + sizeof(TestStruct) + sizeof(BlockFooter), footer->totalBlockSize);

    a.Deallocate(p);

    // Should be empty
    ASSERT_EQ(513, header->sizeAndFlags);
    ASSERT_TRUE(header->sizeAndFlags & IS_FREE_FLAG);
    ASSERT_FALSE(p);
    ASSERT_EQ(a.FlBitmask(), 4);
    ASSERT_EQ(a.SlBitmask()[2], 1);
    ASSERT_EQ(64, a.BytesRemaining());

    p = (TestStruct*) a.Allocate(sizeof(TestStruct));
    p->inta = 10;
    p->intb = 20;

    ASSERT_EQ(10, p->inta);
    ASSERT_EQ(20, p->intb);

    ASSERT_EQ(a.FlBitmask(), 2);
    ASSERT_EQ(16, a.SlBitmask()[1]);

    block = a.FreeList()[(1 * 16) + 4];
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);
    ASSERT_EQ(40, a.BytesRemaining());

    header = (BlockHeader*)a.Data();
    data = (TestStruct*) (((unsigned char*)header) + sizeof(BlockHeader));
    footer = (BlockFooter*) (((unsigned char*)data) + sizeof(TestStruct));

    ASSERT_EQ(192, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(BlockHeader) + sizeof(TestStruct) + sizeof(BlockFooter), footer->totalBlockSize);

    Siege::String* str = (Siege::String*) a.Allocate(sizeof(Siege::String));

    *str = "Hello There!";
    ASSERT_STREQ(str->Str(),"Hello There!");

    ASSERT_EQ(1, a.FlBitmask());
    ASSERT_EQ(0, a.SlBitmask()[1]);

    FreeBlockNode* NewBlock = a.FreeList()[0];
    ASSERT_EQ(NewBlock->next, nullptr);
    ASSERT_EQ(NewBlock->prev, nullptr);
    ASSERT_EQ(16, a.BytesRemaining());

    auto newHeader = (BlockHeader*)(TO_BYTES(footer) + sizeof(BlockFooter));
    auto newData = (Siege::String*) (((unsigned char*)newHeader) + sizeof(BlockHeader));
    auto newFooter = (BlockFooter*) (((unsigned char*)newData) + sizeof(TestStruct));

    ASSERT_EQ(192, newHeader->sizeAndFlags);
    ASSERT_EQ(newData, str);
    ASSERT_EQ(sizeof(BlockHeader) + sizeof(Siege::String) + sizeof(BlockFooter), newFooter->totalBlockSize);

    a.Deallocate(p);
    ASSERT_FALSE(p);
    ASSERT_TRUE(newHeader->sizeAndFlags & IS_PREV_FREE_FLAG);
    ASSERT_EQ(194, newHeader->sizeAndFlags);
    ASSERT_TRUE(header->sizeAndFlags & IS_FREE_FLAG);
    FreeBlockNode* newFreeBlock = TO_FREEBLOCK((TO_BYTES(header)+sizeof(BlockHeader)));
    ASSERT_EQ(newFreeBlock->next, nullptr);
    ASSERT_EQ(newFreeBlock->prev, nullptr);
    ASSERT_EQ(40, a.BytesRemaining());

    a.Deallocate(str);
    ASSERT_FALSE(str);
    ASSERT_EQ(513, header->sizeAndFlags);
    ASSERT_EQ(4, a.FlBitmask());
    ASSERT_EQ(1, a.SlBitmask()[2]);
    ASSERT_EQ(64, a.BytesRemaining());
}

UTEST(test_ResourceSystem, TestBlockCoalescing)
{
    Allocator a(128);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 128);
    ASSERT_EQ(128, a.BytesRemaining());

    TestStruct* p = (TestStruct*) a.Allocate(sizeof(TestStruct));

    p->inta = 10;
    p->intb = 20;

    ASSERT_EQ(10, p->inta);
    ASSERT_EQ(20, p->intb);

    ASSERT_EQ(4, a.FlBitmask());
    ASSERT_EQ(0, a.SlBitmask()[1]);

    FreeBlockNode* block = a.FreeList()[42];
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);
    ASSERT_EQ(104, a.BytesRemaining());

    auto header = (BlockHeader*)a.Data();
    auto data = (TestStruct*) (((unsigned char*)header) + sizeof(BlockHeader));
    auto footer = (BlockFooter*) (((unsigned char*)data) + sizeof(TestStruct));

    ASSERT_EQ(192, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(BlockHeader) + sizeof(TestStruct) + sizeof(BlockFooter), footer->totalBlockSize);

    Siege::String* str = (Siege::String*) a.Allocate(sizeof(Siege::String));

    *str = "Hello There!";
    ASSERT_STREQ(str->Str(),"Hello There!");

    ASSERT_EQ(4, a.FlBitmask());
    ASSERT_EQ(0, a.SlBitmask()[1]);

    FreeBlockNode* NewBlock = a.FreeList()[36];
    ASSERT_EQ(NewBlock->next, nullptr);
    ASSERT_EQ(NewBlock->prev, nullptr);
    ASSERT_EQ(80, a.BytesRemaining());

    auto newHeader = (BlockHeader*)(TO_BYTES(footer) + sizeof(BlockFooter));
    auto newData = (Siege::String*) (((unsigned char*)newHeader) + sizeof(BlockHeader));
    auto newFooter = (BlockFooter*) (((unsigned char*)newData) + sizeof(TestStruct));

    ASSERT_EQ(192, newHeader->sizeAndFlags);
    ASSERT_EQ(newData, str);
    ASSERT_EQ(sizeof(BlockHeader) + sizeof(Siege::String) + sizeof(BlockFooter), newFooter->totalBlockSize);

    TestStruct* p2 = (TestStruct*) a.Allocate(sizeof(TestStruct));

    p2->inta = 10;
    p2->intb = 20;

    ASSERT_EQ(10, p2->inta);
    ASSERT_EQ(20, p2->intb);

    ASSERT_EQ(a.FlBitmask(), 2);
    ASSERT_EQ(4096, a.SlBitmask()[1]);

    FreeBlockNode* newFreeBlock = a.FreeList()[28];
    ASSERT_EQ(newFreeBlock->next, nullptr);
    ASSERT_EQ(newFreeBlock->prev, nullptr);
    ASSERT_EQ(56, a.BytesRemaining());

    auto newHeader2 = (BlockHeader*)(TO_BYTES(newFooter) + sizeof(BlockFooter));
    auto newData2 = (TestStruct*) (((unsigned char*)newHeader2) + sizeof(BlockHeader));
    auto newFooter2 = (BlockFooter*) (((unsigned char*)newData2) + sizeof(TestStruct));

    ASSERT_EQ(192, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(BlockHeader) + sizeof(TestStruct) + sizeof(BlockFooter), footer->totalBlockSize);

    a.Deallocate(p);

    FreeBlockNode* firstFree = a.FreeList()[8];
    ASSERT_EQ(firstFree->next, nullptr);
    ASSERT_EQ(firstFree->prev, nullptr);
    ASSERT_EQ(80, a.BytesRemaining());

    BlockHeader* firstFreeHeader = TO_HBLOCK((TO_BYTES(firstFree) - sizeof(BlockHeader)));
    ASSERT_TRUE(firstFreeHeader->sizeAndFlags & IS_FREE_FLAG);
    ASSERT_FALSE(firstFreeHeader->sizeAndFlags & IS_PREV_FREE_FLAG);
    ASSERT_FALSE(p);
    ASSERT_EQ(a.FlBitmask(), 3);
    ASSERT_EQ(256, a.SlBitmask()[0]);

    a.Deallocate(p2);
    ASSERT_FALSE(p2);
    ASSERT_EQ(104, a.BytesRemaining());
    ASSERT_EQ(a.FlBitmask(), 5);
    ASSERT_EQ(256, a.SlBitmask()[0]);
    ASSERT_EQ(16, a.SlBitmask()[2]);

    a.Deallocate(str);
    ASSERT_FALSE(str);
    ASSERT_EQ(128, a.BytesRemaining());
    ASSERT_EQ(a.FlBitmask(), 8);
    ASSERT_EQ(a.SlBitmask()[3], 1);

    // Edge Cases

    void* badPointer = nullptr;
    a.Deallocate(badPointer);
    ASSERT_FALSE(badPointer);

    // try to deallocate pointer not in allocator;

    uint64_t* val = new uint64_t;
    a.Deallocate(val); // Should do nothing and be ignored.
    free(val);
}

UTEST(test_ResourceSystem, TestAllocationWhenNoAppropriateFragmentExists)
{
    Allocator a(128);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 128);
    ASSERT_EQ(128, a.BytesRemaining());

    void* p0 = a.Allocate(16);
    void* p1 = a.Allocate(32);
    void* p2 = a.Allocate(16);
    void* p3 = a.Allocate(32);
    ASSERT_EQ(0, a.BytesRemaining());

    a.Deallocate(p0);
    a.Deallocate(p2);

    ASSERT_EQ(48, a.BytesRemaining());
    void* tooLargeVal = a.Allocate(24);
    ASSERT_FALSE(tooLargeVal);
}
