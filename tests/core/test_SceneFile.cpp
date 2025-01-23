//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <core/entity/Entity.h>
#include <core/scene/SceneFile.h>
#include <utest.h>
#include <utils/Token.h>

#include <utility>

using namespace Siege;

REGISTER_TOKEN(TestEntity1);
class TestEntity1 : public Entity
{
public:

    TestEntity1() : Entity(TOKEN_TestEntity1) {};

    TestEntity1(Xform transform, int zIndex) : Entity(TOKEN_TestEntity1, transform, zIndex) {};
};

REGISTER_TOKEN(TestEntity2);
class TestEntity2 : public Entity
{
public:

    TestEntity2() : Entity(TOKEN_TestEntity2) {};

    explicit TestEntity2(String customData) :
        Entity(TOKEN_TestEntity2),
        customData(std::move(customData)) {};

    String customData;
};

REGISTER_TOKEN(TestEntity3);
class TestEntity3 : public Entity
{
public:

    TestEntity3() : Entity(TOKEN_TestEntity3) {};
};

// Define test fixture
struct test_SceneFile
{
    test_SceneFile()
    {
        // Register all serialisables for the fixture
        SceneFile::RegisterSerialisable(
            TOKEN_TestEntity1,
            [](Entity* entity) -> String {
                return DefineField("CUSTOM_DATA", "this is some custom data");
            },
            [](const std::map<String, String>& attributes) -> Entity* {
                Siege::EntityData data = Siege::SceneFile::GetBaseEntityData(attributes);
                return new TestEntity1(Xform(data.position, data.rotation), data.zIndex);
            });
        SceneFile::RegisterSerialisable(
            TOKEN_TestEntity2,
            [](Entity* entity) -> String {
                return DefineField("CUSTOM_DATA", "this is some other custom data");
            },
            [](const std::map<String, String>& attributes) -> Entity* {
                auto it = attributes.find("CUSTOM_DATA");
                return new TestEntity2(it != attributes.end() ? it->second : "");
            });
        SceneFile::RegisterSerialisable(TOKEN_TestEntity3, nullptr, nullptr);
    }

    // Entity storage with cleanup
    static std::vector<Entity*>& Entities()
    {
        static std::vector<Entity*> entities;
        return entities;
    }
};

UTEST_F_SETUP(test_SceneFile)
{
    ASSERT_TRUE(test_SceneFile::Entities().empty());
}

UTEST_F_TEARDOWN(test_SceneFile)
{
    std::vector<Entity*>& entities = test_SceneFile::Entities();
    for (auto& entity : entities) delete entity;
    entities.clear();
}

UTEST_F(test_SceneFile, SerialiseEntityToString)
{
    // When serialising an entity, it should serialise correctly
    TestEntity1 e1;
    String sceneData;
    ASSERT_TRUE(SceneFile::SerialiseToString(&e1, sceneData));
    ASSERT_STREQ("TYPE:TestEntity1;"
                 "POSITION:0.00,0.00,0.00;"
                 "ROTATION:0.000000;"
                 "Z-INDEX:0;"
                 "CUSTOM_DATA:this is some custom data;",
                 sceneData.Str());

    // Modifying its fields should result in the same transforms being applied
    sceneData.Clear();
    e1.SetPosition({1, 2, 3});
    e1.SetRotation({0.f, 25.f, 0.f});
    e1.SetZIndex(-3);
    ASSERT_TRUE(SceneFile::SerialiseToString(&e1, sceneData));
    ASSERT_STREQ("TYPE:TestEntity1;"
                 "POSITION:1.00,2.00,3.00;"
                 "ROTATION:25.000000;"
                 "Z-INDEX:-3;"
                 "CUSTOM_DATA:this is some custom data;",
                 sceneData.Str());
}

UTEST_F(test_SceneFile, SerialiseEntityToStringWithoutSerialiser)
{
    // Entities that do not define a serialiser should have only basic serialisation
    TestEntity3 e3;
    String sceneData;
    ASSERT_TRUE(SceneFile::SerialiseToString(&e3, sceneData));
    ASSERT_STREQ("TYPE:TestEntity3;"
                 "POSITION:0.00,0.00,0.00;"
                 "ROTATION:0.000000;"
                 "Z-INDEX:0;",
                 sceneData.Str());
}

UTEST_F(test_SceneFile, SerialiseEntityToStringWithoutInterface)
{
    // Entities that do not define a serialiser should not serialise
    Entity e4;
    String sceneData;
    ASSERT_FALSE(SceneFile::SerialiseToString(&e4, sceneData));
    ASSERT_STREQ("", sceneData.Str());
}

UTEST_F(test_SceneFile, DeserialiseEntityFromStringNoData)
{
    // When deserialising a scene with no entities it should not instantiate any entities
    String fileData;
    Entity* e1 = SceneFile::DeserialiseFromString(fileData);
    ASSERT_FALSE(e1);
}

UTEST_F(test_SceneFile, DeserialiseEntityFromStringGarbageData)
{
    // When deserialising a scene with bad data it should not instantiate any entities
    String fileData = "TesPOSITIOTA;"
                      "this is some custom data;"
                      "T some custom data;";
    Entity* e1 = SceneFile::DeserialiseFromString(fileData);
    ASSERT_FALSE(e1);
}

UTEST_F(test_SceneFile, DeserialiseEntityFromString)
{
    // When deserialising a scene with a single entity it should deserialise correctly
    String fileData = "TYPE:TestEntity1;"
                      "POSITION:1.00,2.00,3.00;"
                      "ROTATION:25.000000;"
                      "Z-INDEX:-3;"
                      "CUSTOM_DATA:this is some custom data;";
    Entity* e1 = SceneFile::DeserialiseFromString(fileData);
    ASSERT_TRUE(e1);
    ASSERT_TRUE(dynamic_cast<TestEntity1*>(e1));

    // It should retain its standard field values
    ASSERT_STREQ("TestEntity1", e1->GetType().GetId());
    Vec3 pos(e1->GetPosition());
    Vec3 rot(e1->GetRotation());
    ASSERT_EQ(1.f, pos.x);
    ASSERT_EQ(2.f, pos.y);
    ASSERT_EQ(3.f, pos.z);
    ASSERT_EQ(0.f, rot.x);
    ASSERT_EQ(25.f, rot.y);
    ASSERT_EQ(0.f, rot.z);
    ASSERT_EQ(-3, e1->GetZIndex());
    delete e1;

    // It should retain its custom data
    fileData = "TYPE:TestEntity2;"
               "POSITION:0.00,0.00,0.00;"
               "ROTATION:0.000000;"
               "Z-INDEX:0;"
               "CUSTOM_DATA:this is some other custom data;";
    Entity* e2 = SceneFile::DeserialiseFromString(fileData);
    ASSERT_TRUE(e2);
    auto* e2Cast = dynamic_cast<TestEntity2*>(e2);
    ASSERT_TRUE(e2Cast);
    ASSERT_STREQ("TestEntity2", e2Cast->GetType().GetId());
    ASSERT_STREQ("this is some other custom data", e2Cast->customData.Str());
    delete e2;
}

UTEST_F(test_SceneFile, DeserialiseEntityFromStringWithoutDeserialiser)
{
    // When deserialising entities that do not define a deserialiser, it should do nothing
    String fileData = "TYPE:TestEntity3;"
                      "POSITION:0.00,0.00,0.00;"
                      "ROTATION:0.000000;"
                      "Z-INDEX:0;"
                      "CUSTOM_DATA:this is some other custom data;";
    Entity* e1 = SceneFile::DeserialiseFromString(fileData);
    ASSERT_FALSE(e1);
}

UTEST_F(test_SceneFile, DeserialiseEntityFromStringWithoutInterface)
{
    // Entities that do not define a serialisation interface should not deserialise
    String fileData = "TYPE:Entity;"
                      "POSITION:0.00,0.00,0.00;"
                      "ROTATION:0.000000;"
                      "Z-INDEX:0;"
                      "CUSTOM_DATA:this is some other custom data;";
    Entity* e1 = SceneFile::DeserialiseFromString(fileData);
    ASSERT_FALSE(e1);
}