#include "catch.hpp"
#include "../src/engine/scene/SceneSerialiser.h"
#include "../src/engine/entity/Entity.h"

class TestEntity1 : public Entity
{
public:

    static const std::string ENTITY_NAME;

    TestEntity1() :
            Entity(ENTITY_NAME)
    {};

    TestEntity1(raylib::Vector3 position, float rotation, int zIndex) :
            Entity(ENTITY_NAME, position, rotation, zIndex)
    {};
};
const std::string TestEntity1::ENTITY_NAME("TestEntity1");


class TestEntity2 : public Entity
{
public:

    static const std::string ENTITY_NAME;

    TestEntity2() :
            Entity(ENTITY_NAME)
    {};

    explicit TestEntity2(const std::string& customData) :
            Entity(ENTITY_NAME),
            customData(customData)
    {};

    std::string customData;
};
const std::string TestEntity2::ENTITY_NAME("TestEntity2");


class TestEntity3 : public Entity
{
public:

    static const std::string ENTITY_NAME;

    TestEntity3() :
            Entity(ENTITY_NAME)
    {};
};
const std::string TestEntity3::ENTITY_NAME("TestEntity3");


TEST_CASE("serialisation and deserialisation can be performed", "[SceneSerialiser]")
{
    auto serialise1 = [](Entity* entity)->std::string {
        return DefineField("CUSTOM_DATA", "this is some custom data");
    };
    auto deserialise1 = [](const EntityData& data, const std::vector<std::string>& args)->Entity* {
        return new TestEntity1(data.position, data.rotation, data.zIndex);
    };

    auto serialise2 = [](Entity* entity)->std::string {
        return DefineField("CUSTOM_DATA", "this is some other custom data");
    };
    auto deserialise2 = [](const EntityData& data, const std::vector<std::string>& args)->Entity* {
        return new TestEntity2(args[CUSTOM_FIELD_1]);
    };

    SceneSerialiser::RegisterSerialisable(TestEntity1::ENTITY_NAME, serialise1, deserialise1);
    SceneSerialiser::RegisterSerialisable(TestEntity2::ENTITY_NAME, serialise2, deserialise2);
    SceneSerialiser::RegisterSerialisable(TestEntity3::ENTITY_NAME, nullptr, nullptr);

    SECTION("when serialising")
    {
        auto e1 = new TestEntity1();
        auto e2 = new TestEntity2();
        auto e3 = new TestEntity3();

        SECTION("with a single entity it should serialise correctly")
        {
            std::string sceneData = SceneSerialiser::Serialise({e1});
            REQUIRE(sceneData == "TestEntity1|"
                                 "POSITION:0.00,0.00,0.00|"
                                 "ROTATION:0.000000|"
                                 "Z-INDEX:0|"
                                 "CUSTOM_DATA:this is some custom data|\n");

            SECTION("and modifying its fields should result in the same transforms being applied to the scene data")
            {
                e1->SetPosition({1, 2, 3});
                e1->SetRotation(25.f);
                e1->SetZIndex(-3);

                sceneData = SceneSerialiser::Serialise({e1});
                REQUIRE(sceneData == "TestEntity1|"
                                     "POSITION:1.00,2.00,3.00|"
                                     "ROTATION:25.000000|"
                                     "Z-INDEX:-3|"
                                     "CUSTOM_DATA:this is some custom data|\n");
            }
        }

        SECTION("with multiple entities, it should serialise in order of position")
        {
            std::string sceneData = SceneSerialiser::Serialise({e2, e1});
            REQUIRE(sceneData == "TestEntity2|"
                                 "POSITION:0.00,0.00,0.00|"
                                 "ROTATION:0.000000|"
                                 "Z-INDEX:0|"
                                 "CUSTOM_DATA:this is some other custom data|\n"
                                 "TestEntity1|"
                                 "POSITION:0.00,0.00,0.00|"
                                 "ROTATION:0.000000|"
                                 "Z-INDEX:0|"
                                 "CUSTOM_DATA:this is some custom data|\n");

            sceneData = SceneSerialiser::Serialise({e1, e2});
            REQUIRE(sceneData == "TestEntity1|"
                                 "POSITION:0.00,0.00,0.00|"
                                 "ROTATION:0.000000|"
                                 "Z-INDEX:0|"
                                 "CUSTOM_DATA:this is some custom data|\n"
                                 "TestEntity2|"
                                 "POSITION:0.00,0.00,0.00|"
                                 "ROTATION:0.000000|"
                                 "Z-INDEX:0|"
                                 "CUSTOM_DATA:this is some other custom data|\n");
        }

        SECTION("with entities that do not define a serialiser, it should have only basic serialisation")
        {
            std::string sceneData = SceneSerialiser::Serialise({e2, e3, e1});
            REQUIRE(sceneData == "TestEntity2|"
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
                                 "CUSTOM_DATA:this is some custom data|\n");
        }

        SECTION("with entities that do not define a serialisation interface, it should not serialise")
        {
            auto e4 = new Entity();
            std::string sceneData = SceneSerialiser::Serialise({e2, e4, e1});
            REQUIRE(sceneData == "TestEntity2|"
                                 "POSITION:0.00,0.00,0.00|"
                                 "ROTATION:0.000000|"
                                 "Z-INDEX:0|"
                                 "CUSTOM_DATA:this is some other custom data|\n"
                                 "TestEntity1|"
                                 "POSITION:0.00,0.00,0.00|"
                                 "ROTATION:0.000000|"
                                 "Z-INDEX:0|"
                                 "CUSTOM_DATA:this is some custom data|\n");
        }

        delete e1;
        delete e2;
        delete e3;
    }

    SECTION("when deserialising")
    {
        std::vector<Entity*> entities;
        std::vector<std::string> sceneLines;

        SECTION("with no entities it should deserialise correctly")
        {
            SceneSerialiser::Deserialise(sceneLines, entities);
            REQUIRE(entities.empty());
        }

        SECTION("with bad data it should throw an exception")
        {
            sceneLines = {
                    "TesPOSITIOTA:this is some custom data|\n"
                    "T some custom data|\n"
            };
            REQUIRE_THROWS_AS(SceneSerialiser::Deserialise(sceneLines, entities), std::length_error);
            REQUIRE(entities.empty());
        }

        SECTION("with a single entity it should deserialise correctly")
        {
            sceneLines = {
                    "TestEntity1|"
                    "POSITION:1.00,2.00,3.00|"
                    "ROTATION:25.000000|"
                    "Z-INDEX:-3|"
                    "CUSTOM_DATA:this is some custom data|\n"
            };
            SceneSerialiser::Deserialise(sceneLines, entities);
            REQUIRE(entities.size() == 1);
            REQUIRE(dynamic_cast<TestEntity1*>(entities[0]));

            SECTION("and retain its standard field values")
            {
                REQUIRE(entities[0]->GetName() == "TestEntity1");
                raylib::Vector3 pos(entities[0]->GetPosition());
                REQUIRE(pos.x == 1.f);
                REQUIRE(pos.y == 2.f);
                REQUIRE(pos.z == 3.f);
                REQUIRE(entities[0]->GetRotation() == 25.f);
                REQUIRE(entities[0]->GetZIndex() == -3);
            }

            SECTION("and retain its custom data")
            {
                sceneLines = {
                        "TestEntity2|"
                        "POSITION:0.00,0.00,0.00|"
                        "ROTATION:0.000000|"
                        "Z-INDEX:0|"
                        "CUSTOM_DATA:this is some other custom data|\n",
                };
                SceneSerialiser::Deserialise(sceneLines, entities);
                REQUIRE(entities.size() == 2);
                auto e2 = dynamic_cast<TestEntity2*>(entities[1]);
                REQUIRE(e2);
                REQUIRE(e2->customData == "this is some other custom data");
            }
        }

        SECTION("with multiple entities, it should deserialise in order of position")
        {
            sceneLines = {
                    "TestEntity2|"
                    "POSITION:0.00,0.00,0.00|"
                    "ROTATION:0.000000|"
                    "Z-INDEX:0|"
                    "CUSTOM_DATA:this is some other custom data|\n",
                    "TestEntity1|"
                    "POSITION:0.00,0.00,0.00|"
                    "ROTATION:0.000000|"
                    "Z-INDEX:0|"
                    "CUSTOM_DATA:this is some custom data|\n"
            };
            SceneSerialiser::Deserialise(sceneLines, entities);
            REQUIRE(entities.size() == 2);
            REQUIRE(dynamic_cast<TestEntity2*>(entities[0]));
            REQUIRE(dynamic_cast<TestEntity1*>(entities[1]));

            sceneLines = {
                    "TestEntity1|"
                    "POSITION:0.00,0.00,0.00|"
                    "ROTATION:0.000000|"
                    "Z-INDEX:0|"
                    "CUSTOM_DATA:this is some custom data|\n",
                    "TestEntity2|"
                    "POSITION:0.00,0.00,0.00|"
                    "ROTATION:0.000000|"
                    "Z-INDEX:0|"
                    "CUSTOM_DATA:this is some other custom data|\n"
            };
            SceneSerialiser::Deserialise(sceneLines, entities);
            REQUIRE(entities.size() == 4);
            REQUIRE(dynamic_cast<TestEntity1*>(entities[2]));
            REQUIRE(dynamic_cast<TestEntity2*>(entities[3]));
        }

        SECTION("with entities that do not define a deserialiser, it should not deserialise")
        {
            sceneLines = {
                    "TestEntity3|"
                    "POSITION:0.00,0.00,0.00|"
                    "ROTATION:0.000000|"
                    "Z-INDEX:0|"
                    "CUSTOM_DATA:this is some other custom data|\n",
                    "TestEntity1|"
                    "POSITION:0.00,0.00,0.00|"
                    "ROTATION:0.000000|"
                    "Z-INDEX:0|"
                    "CUSTOM_DATA:this is some custom data|\n"
            };
            SceneSerialiser::Deserialise(sceneLines, entities);
            REQUIRE(entities.size() == 1);
            REQUIRE(dynamic_cast<TestEntity1*>(entities[0]));
        }

        SECTION("with entities that do not define a serialisation interface, it should not deserialise")
        {
            sceneLines = {
                    "TestEntity1|"
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
                    "CUSTOM_DATA:this is some custom data|\n"
            };
            SceneSerialiser::Deserialise(sceneLines, entities);
            REQUIRE(entities.size() == 2);
            REQUIRE(dynamic_cast<TestEntity1*>(entities[0]));
            REQUIRE(dynamic_cast<TestEntity2*>(entities[1]));
        }

        for (auto& entity : entities) delete entity;
    }
}