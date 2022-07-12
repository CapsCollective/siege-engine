//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <core/entity/Entity.h>
#include <core/scene/SceneFile.h>
#include <utest.h>

#include <utility>

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

UTEST_F(test_SceneFile, SerialiseSingleEntity)
{
    // When serialising an entity, it should serialise correctly
    TestEntity1 e1;
    String sceneData = SceneFile::SerialiseToString({&e1});
    ASSERT_STREQ("TestEntity1|"
                 "POSITION:0.00,0.00,0.00|"
                 "ROTATION:0.000000|"
                 "Z-INDEX:0|"
                 "CUSTOM_DATA:this is some custom data|\n",
                 sceneData.Str());

    // Modifying its fields should result in the same transforms being applied
    e1.SetPosition({1, 2, 3});
    e1.SetRotation(25.f);
    e1.SetZIndex(-3);
    sceneData = SceneFile::SerialiseToString({&e1});
    ASSERT_STREQ("TestEntity1|"
                 "POSITION:1.00,2.00,3.00|"
                 "ROTATION:25.000000|"
                 "Z-INDEX:-3|"
                 "CUSTOM_DATA:this is some custom data|\n",
                 sceneData.Str());
}

UTEST_F(test_SceneFile, SerialiseMultipleEntities)
{
    // When serialising multiple entities, it should serialise in order of position
    TestEntity1 e1;
    TestEntity2 e2;
    String sceneData = SceneFile::SerialiseToString({&e2, &e1});
    ASSERT_STREQ("TestEntity2|"
                 "POSITION:0.00,0.00,0.00|"
                 "ROTATION:0.000000|"
                 "Z-INDEX:0|"
                 "CUSTOM_DATA:this is some other custom data|\n"
                 "TestEntity1|"
                 "POSITION:0.00,0.00,0.00|"
                 "ROTATION:0.000000|"
                 "Z-INDEX:0|"
                 "CUSTOM_DATA:this is some custom data|\n",
                 sceneData.Str());
    sceneData = SceneFile::SerialiseToString({&e1, &e2});
    ASSERT_STREQ("TestEntity1|"
                 "POSITION:0.00,0.00,0.00|"
                 "ROTATION:0.000000|"
                 "Z-INDEX:0|"
                 "CUSTOM_DATA:this is some custom data|\n"
                 "TestEntity2|"
                 "POSITION:0.00,0.00,0.00|"
                 "ROTATION:0.000000|"
                 "Z-INDEX:0|"
                 "CUSTOM_DATA:this is some other custom data|\n",
                 sceneData.Str());
}

UTEST_F(test_SceneFile, SerialiseEntityWithoutSerialiser)
{
    // Entities that do not define a serialiser should have only basic serialisation
    TestEntity1 e1;
    TestEntity2 e2;
    TestEntity3 e3;
    String sceneData = SceneFile::SerialiseToString({&e2, &e3, &e1});
    ASSERT_STREQ("TestEntity2|"
                 "POSITION:0.00,0.00,0.00|"
                 "ROTATION:0.000000|"
                 "Z-INDEX:0|"
                 "CUSTOM_DATA:this is some other custom data|\n"
                 "TestEntity3|"
                 "POSITION:0.00,0.00,0.00|"
                 "ROTATION:0.000000|"
                 "Z-INDEX:0|\n"
                 "TestEntity1|"
                 "POSITION:0.00,0.00,0.00|"
                 "ROTATION:0.000000|"
                 "Z-INDEX:0|"
                 "CUSTOM_DATA:this is some custom data|\n",
                 sceneData.Str());
}

UTEST_F(test_SceneFile, SerialiseEntityWithoutInterface)
{
    // Entities that do not define a serialiser should have only basic serialisation
    TestEntity1 e1;
    TestEntity2 e2;
    Entity e4;
    String sceneData = SceneFile::SerialiseToString({&e2, &e4, &e1});
    ASSERT_STREQ("TestEntity2|"
                 "POSITION:0.00,0.00,0.00|"
                 "ROTATION:0.000000|"
                 "Z-INDEX:0|"
                 "CUSTOM_DATA:this is some other custom data|\n"
                 "TestEntity1|"
                 "POSITION:0.00,0.00,0.00|"
                 "ROTATION:0.000000|"
                 "Z-INDEX:0|"
                 "CUSTOM_DATA:this is some custom data|\n",
                 sceneData.Str());
}

UTEST_F(test_SceneFile, DeserialiseNoEntitites)
{
    // When deserialising a scene with no entities it should not instantiate any entities
    std::vector<Entity*>& entities = test_SceneFile::Entities();
    std::vector<String> sceneLines;

    SceneFile::DeserialiseLines(sceneLines, entities);
    ASSERT_TRUE(entities.empty());
}

UTEST_F(test_SceneFile, DeserialiseGarbageData)
{
    // When deserialising a scene with bad data it should not instantiate any entities
    std::vector<Entity*>& entities = test_SceneFile::Entities();

    std::vector<String> sceneLines = {"TesPOSITIOTA:this is some custom data|\n"
                                      "T some custom data|\n"};
    SceneFile::DeserialiseLines(sceneLines, entities);
    ASSERT_TRUE(entities.empty());
}

UTEST_F(test_SceneFile, DeserialiseSingleEntity)
{
    // When deserialising a scene with a single entity it should deserialise correctly
    std::vector<Entity*>& entities = test_SceneFile::Entities();
    std::vector<String> sceneLines = {
        "TestEntity1|"
        "POSITION:1.00,2.00,3.00|"
        "ROTATION:25.000000|"
        "Z-INDEX:-3|"
        "CUSTOM_DATA:this is some custom data|\n",
        "TestEntity2|"
        "POSITION:0.00,0.00,0.00|"
        "ROTATION:0.000000|"
        "Z-INDEX:0|"
        "CUSTOM_DATA:this is some other custom data|\n",
    };
    SceneFile::DeserialiseLines(sceneLines, entities);
    ASSERT_EQ(2, entities.size());
    ASSERT_TRUE(dynamic_cast<TestEntity1*>(entities[0]));

    // It should retain its standard field values
    ASSERT_STREQ("TestEntity1", entities[0]->GetName().Str());
    Siege::Vec3 pos(entities[0]->GetPosition());
    ASSERT_EQ(1.f, pos.x);
    ASSERT_EQ(2.f, pos.y);
    ASSERT_EQ(3.f, pos.z);
    ASSERT_EQ(25.f, entities[0]->GetRotation());
    ASSERT_EQ(-3, entities[0]->GetZIndex());

    // It should retain its custom data
    ASSERT_EQ(2, entities.size());
    auto e2 = dynamic_cast<TestEntity2*>(entities[1]);
    ASSERT_TRUE(e2);
    ASSERT_STREQ("TestEntity2", e2->GetName().Str());
    ASSERT_STREQ("this is some other custom data", e2->customData.Str());
}

UTEST_F(test_SceneFile, DeserialiseMultipleEntities)
{
    // When deserialising a scene with multiple entities, it should be in order of position
    std::vector<Entity*>& entities = test_SceneFile::Entities();
    std::vector<String> sceneLines;

    sceneLines = {"TestEntity2|"
                  "POSITION:0.00,0.00,0.00|"
                  "ROTATION:0.000000|"
                  "Z-INDEX:0|"
                  "CUSTOM_DATA:this is some other custom data|\n",
                  "TestEntity1|"
                  "POSITION:0.00,0.00,0.00|"
                  "ROTATION:0.000000|"
                  "Z-INDEX:0|"
                  "CUSTOM_DATA:this is some custom data|\n"};
    SceneFile::DeserialiseLines(sceneLines, entities);
    ASSERT_EQ(2, entities.size());
    ASSERT_TRUE(dynamic_cast<TestEntity2*>(entities[0]));
    ASSERT_TRUE(dynamic_cast<TestEntity1*>(entities[1]));

    sceneLines = {"TestEntity1|"
                  "POSITION:0.00,0.00,0.00|"
                  "ROTATION:0.000000|"
                  "Z-INDEX:0|"
                  "CUSTOM_DATA:this is some custom data|\n",
                  "TestEntity2|"
                  "POSITION:0.00,0.00,0.00|"
                  "ROTATION:0.000000|"
                  "Z-INDEX:0|"
                  "CUSTOM_DATA:this is some other custom data|\n"};
    SceneFile::DeserialiseLines(sceneLines, entities);
    ASSERT_EQ(4, entities.size());
    ASSERT_TRUE(dynamic_cast<TestEntity1*>(entities[2]));
    ASSERT_TRUE(dynamic_cast<TestEntity2*>(entities[3]));
}

UTEST_F(test_SceneFile, DeserialiseEntityWithoutDeserialiser)
{
    // When deserialising entities that do not define a deserialiser, it should do nothing
    std::vector<Entity*>& entities = test_SceneFile::Entities();
    std::vector<String> sceneLines = {"TestEntity3|"
                                      "POSITION:0.00,0.00,0.00|"
                                      "ROTATION:0.000000|"
                                      "Z-INDEX:0|"
                                      "CUSTOM_DATA:this is some other custom data|\n",
                                      "TestEntity1|"
                                      "POSITION:0.00,0.00,0.00|"
                                      "ROTATION:0.000000|"
                                      "Z-INDEX:0|"
                                      "CUSTOM_DATA:this is some custom data|\n"};
    SceneFile::DeserialiseLines(sceneLines, entities);
    ASSERT_EQ(1, entities.size());
    ASSERT_TRUE(dynamic_cast<TestEntity1*>(entities[0]));
}

UTEST_F(test_SceneFile, DeserialiseEntityWithoutInterface)
{
    // Entities that do not define a serialisation interface should not deserialise
    std::vector<Entity*>& entities = test_SceneFile::Entities();
    std::vector<String> sceneLines = {"TestEntity1|"
                                      "POSITION:0.00,0.00,0.00|"
                                      "ROTATION:0.000000|"
                                      "Z-INDEX:0|"
                                      "CUSTOM_DATA:this is some custom data|\n",
                                      "Entity|"
                                      "POSITION:0.00,0.00,0.00|"
                                      "ROTATION:0.000000|"
                                      "Z-INDEX:0|"
                                      "CUSTOM_DATA:this is some other custom data|\n",
                                      "TestEntity2|"
                                      "POSITION:0.00,0.00,0.00|"
                                      "ROTATION:0.000000|"
                                      "Z-INDEX:0|"
                                      "CUSTOM_DATA:this is some custom data|\n"};
    SceneFile::DeserialiseLines(sceneLines, entities);
    ASSERT_EQ(2, entities.size());
    ASSERT_TRUE(dynamic_cast<TestEntity1*>(entities[0]));
    ASSERT_TRUE(dynamic_cast<TestEntity2*>(entities[1]));
}