#include "catch.hpp"
#include "../src/systems/scene/SceneSerialiser.h"
#include "../src/systems/entity/Entity.h"

class TestEntity1 : public Entity
{
public:

    static const std::string ENTITY_NAME;

    TestEntity1() :
            Entity(ENTITY_NAME)
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
        return new TestEntity1();
    };

    auto serialise2 = [](Entity* entity)->std::string {
        return DefineField("CUSTOM_DATA", "this is some other custom data");
    };
    auto deserialise2 = [](const EntityData& data, const std::vector<std::string>& args)->Entity* {
        return new TestEntity2();
    };

    SceneSerialiser::RegisterSerialisable(TestEntity1::ENTITY_NAME, serialise1, deserialise1);
    SceneSerialiser::RegisterSerialisable(TestEntity2::ENTITY_NAME, serialise2, deserialise2);
    SceneSerialiser::RegisterSerialisable(TestEntity3::ENTITY_NAME, nullptr, nullptr);

    auto e1 = new TestEntity1();
    auto e2 = new TestEntity2();
    auto e3 = new TestEntity3();

    SECTION("when serialising a single entity it should serialise and deserialise correctly")
    {
        std::string sceneData = SceneSerialiser::Serialise({e1});
        REQUIRE(sceneData == "TestEntity1|"
                             "POSITION:0.000000,0.000000,0.000000|"
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
                                 "POSITION:1.000000,2.000000,3.000000|"
                                 "ROTATION:25.000000|"
                                 "Z-INDEX:-3|"
                                 "CUSTOM_DATA:this is some custom data|\n");
        }
    }

    SECTION("when serialising multiple entities, it should serialise in order of position")
    {
        std::string sceneData = SceneSerialiser::Serialise({e2, e1});
        REQUIRE(sceneData == "TestEntity2|"
                             "POSITION:0.000000,0.000000,0.000000|"
                             "ROTATION:0.000000|"
                             "Z-INDEX:0|"
                             "CUSTOM_DATA:this is some other custom data|\n"
                             "TestEntity1|"
                             "POSITION:0.000000,0.000000,0.000000|"
                             "ROTATION:0.000000|"
                             "Z-INDEX:0|"
                             "CUSTOM_DATA:this is some custom data|\n");

        sceneData = SceneSerialiser::Serialise({e1, e2});
        REQUIRE(sceneData == "TestEntity1|"
                             "POSITION:0.000000,0.000000,0.000000|"
                             "ROTATION:0.000000|"
                             "Z-INDEX:0|"
                             "CUSTOM_DATA:this is some custom data|\n"
                             "TestEntity2|"
                             "POSITION:0.000000,0.000000,0.000000|"
                             "ROTATION:0.000000|"
                             "Z-INDEX:0|"
                             "CUSTOM_DATA:this is some other custom data|\n");
    }

    SECTION("when serialising entities that do not define a serialiser, it should have only basic serialisation")
    {
        std::string sceneData = SceneSerialiser::Serialise({e2, e3, e1});
        REQUIRE(sceneData == "TestEntity2|"
                             "POSITION:0.000000,0.000000,0.000000|"
                             "ROTATION:0.000000|"
                             "Z-INDEX:0|"
                             "CUSTOM_DATA:this is some other custom data|\n"
                             "TestEntity3|"
                             "POSITION:0.000000,0.000000,0.000000|"
                             "ROTATION:0.000000|"
                             "Z-INDEX:0|\n"
                             "TestEntity1|"
                             "POSITION:0.000000,0.000000,0.000000|"
                             "ROTATION:0.000000|"
                             "Z-INDEX:0|"
                             "CUSTOM_DATA:this is some custom data|\n");
    }

    SECTION("when serialising entities that do not define a serialisation interface, it should not serialise")
    {
        auto e4 = new Entity();
        std::string sceneData = SceneSerialiser::Serialise({e2, e4, e1});
        REQUIRE(sceneData == "TestEntity2|"
                             "POSITION:0.000000,0.000000,0.000000|"
                             "ROTATION:0.000000|"
                             "Z-INDEX:0|"
                             "CUSTOM_DATA:this is some other custom data|\n"
                             "TestEntity1|"
                             "POSITION:0.000000,0.000000,0.000000|"
                             "ROTATION:0.000000|"
                             "Z-INDEX:0|"
                             "CUSTOM_DATA:this is some custom data|\n");
    }
}