//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <core/entity/Entity.h>
#include <core/scene/SceneFile.h>
#include <utest.h>

#include <utility>

using namespace Siege;

class TestEntity1 : public Entity
{
public:

    static const String ENTITY_NAME;

    TestEntity1() : Entity(ENTITY_NAME) {};

    TestEntity1(Xform transform, int zIndex) : Entity(ENTITY_NAME, transform, zIndex) {};
};
const String TestEntity1::ENTITY_NAME("TestEntity1");

class TestEntity2 : public Entity
{
public:

    static const String ENTITY_NAME;

    TestEntity2() : Entity(ENTITY_NAME) {};

    explicit TestEntity2(String customData) :
        Entity(ENTITY_NAME),
        customData(std::move(customData)) {};

    String customData;
};
const String TestEntity2::ENTITY_NAME("TestEntity2");

class TestEntity3 : public Entity
{
public:

    static const String ENTITY_NAME;

    TestEntity3() : Entity(ENTITY_NAME) {};
};
const String TestEntity3::ENTITY_NAME("TestEntity3");

// Define test fixture
struct test_SceneFile
{
    test_SceneFile()
    {
        // Register all serialisables for the fixture
        SceneFile::RegisterSerialisable(
            TestEntity1::ENTITY_NAME,
            [](Entity* entity) -> String {
                return DefineField("CUSTOM_DATA", "this is some custom data");
            },
            [](const EntityData& data, const std::vector<String>& args) -> Entity* {
                return new TestEntity1(Xform(data.position, data.rotation), data.zIndex);
            });
        SceneFile::RegisterSerialisable(
            TestEntity2::ENTITY_NAME,
            [](Entity* entity) -> String {
                return DefineField("CUSTOM_DATA", "this is some other custom data");
            },
            [](const EntityData& data, const std::vector<String>& args) -> Entity* {
                return new TestEntity2(args[CUSTOM_FIELD_1]);
            });
        SceneFile::RegisterSerialisable(TestEntity3::ENTITY_NAME, nullptr, nullptr);
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
    ASSERT_STREQ("TestEntity1\n"
                 "POSITION:0.00,0.00,0.00\n"
                 "ROTATION:0.000000\n"
                 "Z-INDEX:0\n"
                 "CUSTOM_DATA:this is some custom data\n",
                 sceneData.Str());

    // Modifying its fields should result in the same transforms being applied
    sceneData.Clear();
    e1.SetPosition({1, 2, 3});
    e1.SetRotation({0.f, 25.f, 0.f});
    e1.SetZIndex(-3);
    ASSERT_TRUE(SceneFile::SerialiseToString(&e1, sceneData));
    ASSERT_STREQ("TestEntity1\n"
                 "POSITION:1.00,2.00,3.00\n"
                 "ROTATION:25.000000\n"
                 "Z-INDEX:-3\n"
                 "CUSTOM_DATA:this is some custom data\n",
                 sceneData.Str());
}

UTEST_F(test_SceneFile, SerialiseEntityToStringWithoutSerialiser)
{
    // Entities that do not define a serialiser should have only basic serialisation
    TestEntity3 e3;
    String sceneData;
    ASSERT_TRUE(SceneFile::SerialiseToString(&e3, sceneData));
    ASSERT_STREQ("TestEntity3\n"
                 "POSITION:0.00,0.00,0.00\n"
                 "ROTATION:0.000000\n"
                 "Z-INDEX:0\n",
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
    String fileData = "TesPOSITIOTA\n"
                      "this is some custom data\n"
                      "T some custom data\n";
    Entity* e1 = SceneFile::DeserialiseFromString(fileData);
    ASSERT_FALSE(e1);
}

UTEST_F(test_SceneFile, DeserialiseEntityFromString)
{
    // When deserialising a scene with a single entity it should deserialise correctly
    String fileData = "TestEntity1\n"
                      "POSITION:1.00,2.00,3.00\n"
                      "ROTATION:25.000000\n"
                      "Z-INDEX:-3\n"
                      "CUSTOM_DATA:this is some custom data\n";
    Entity* e1 = SceneFile::DeserialiseFromString(fileData);
    ASSERT_TRUE(e1);
    ASSERT_TRUE(dynamic_cast<TestEntity1*>(e1));

    // It should retain its standard field values
    ASSERT_STREQ("TestEntity1", e1->GetName().Str());
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
    fileData = "TestEntity2\n"
               "POSITION:0.00,0.00,0.00\n"
               "ROTATION:0.000000\n"
               "Z-INDEX:0\n"
               "CUSTOM_DATA:this is some other custom data\n";
    Entity* e2 = SceneFile::DeserialiseFromString(fileData);
    ASSERT_TRUE(e2);
    auto* e2Cast = dynamic_cast<TestEntity2*>(e2);
    ASSERT_TRUE(e2Cast);
    ASSERT_STREQ("TestEntity2", e2Cast->GetName().Str());
    ASSERT_STREQ("this is some other custom data", e2Cast->customData.Str());
    delete e2;
}

UTEST_F(test_SceneFile, DeserialiseEntityFromStringWithoutDeserialiser)
{
    // When deserialising entities that do not define a deserialiser, it should do nothing
    String fileData = "TestEntity3\n"
                      "POSITION:0.00,0.00,0.00\n"
                      "ROTATION:0.000000\n"
                      "Z-INDEX:0\n"
                      "CUSTOM_DATA:this is some other custom data\n";
    Entity* e1 = SceneFile::DeserialiseFromString(fileData);
    ASSERT_FALSE(e1);
}

UTEST_F(test_SceneFile, DeserialiseEntityFromStringWithoutInterface)
{
    // Entities that do not define a serialisation interface should not deserialise
    String fileData = "Entity\n"
                      "POSITION:0.00,0.00,0.00\n"
                      "ROTATION:0.000000\n"
                      "Z-INDEX:0\n"
                      "CUSTOM_DATA:this is some other custom data\n";
    Entity* e1 = SceneFile::DeserialiseFromString(fileData);
    ASSERT_FALSE(e1);
}