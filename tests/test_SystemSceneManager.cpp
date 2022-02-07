#include <engine/entity/Entity.h>
#include <engine/entity/EntitySystem.h>
#include <engine/render/ResourceSystem.h>
#include <engine/scene/SceneFile.h>
#include <engine/scene/SceneSystem.h>
#include <engine/utils/FileSystem.h>
#include <engine/utils/Statics.h>
#include <engine/utils/String.h>

#include "catch.hpp"

// Const declarations
static constexpr const char* SCENE_DIR = "tests/data/";
static constexpr const char* TEST_FILE = "test.scene";
static constexpr const char* UNTITLED_FILE = "untitled.scene";

// Helper methods

String Filepath(const char* filename)
{
    return String(SCENE_DIR) + filename;
}

// Test entity
class TestEntity : public Entity
{
public:

    static const String ENTITY_NAME;

    TestEntity() : Entity(ENTITY_NAME) {}

    explicit TestEntity(Xform transform) : Entity(ENTITY_NAME, transform) {}
};

const String TestEntity::ENTITY_NAME("TestEntity");

REGISTER_SERIALISATION_INTERFACE(
    TestEntity::ENTITY_NAME,
    [](Entity* entity) -> String { return ""; },
    [](const EntityData& data, const std::vector<String>& args) -> Entity* {
        return new TestEntity(Xform(data.position, data.rotation));
    });

TEST_CASE("Scenes can be saved to a file", "[SceneSystem]")
{
    Statics::Resource().SetBaseDirectory(SCENE_DIR);
    Statics::Scene().NewScene();

    SECTION("when an empty scene is saved it should create a file in the correct directory")
    {
        Statics::Scene().SaveScene("test");

        REQUIRE(FileSystem::Exists(Filepath(TEST_FILE)));
        String content = FileSystem::Read(Filepath(TEST_FILE));
        REQUIRE(!content);
    }

    SECTION("When an entity is added to the scene, it should be saved to the scene file.")
    {
        Statics::Entity().Add(new TestEntity());
        Statics::Entity().RegisterEntities();

        Statics::Scene().SaveScene("test");

        REQUIRE(FileSystem::Exists(Filepath(TEST_FILE)));
        String content = FileSystem::Read(Filepath(TEST_FILE));
        REQUIRE(!content.IsEmpty());
        REQUIRE(content.Find("TestEntity") != -1);
    }

    SECTION("When an entity is removed from the scene, it should remove it from the file as well.")
    {
        Statics::Entity().Reset();

        Statics::Scene().SaveScene("test");

        REQUIRE(FileSystem::Exists(Filepath(TEST_FILE)));
        String content = FileSystem::Read(Filepath(TEST_FILE));
        REQUIRE(content.IsEmpty());
        REQUIRE(content.Find("TestEntity") == -1);
    }

    SECTION("when a non-serialisable entity is added to the scene it should not be saved")
    {
        Statics::Entity().Add(new Entity());
        Statics::Scene().SaveScene("test");

        REQUIRE(FileSystem::Exists(Filepath(TEST_FILE)));
        String content = FileSystem::Read(Filepath(TEST_FILE));
        REQUIRE(content.IsEmpty());
    }

    SECTION("when an empty string is passed into SaveScene, it should create 'untitled.scene'")
    {
        Statics::Scene().SaveScene("");
        String filepath = Filepath(UNTITLED_FILE);
        REQUIRE(FileSystem::Exists(filepath));
        remove(filepath);
    }

    Statics::Entity().Reset();
    remove(Filepath(TEST_FILE));
}

TEST_CASE("scenes are erased when a new scene is created", "[SceneSystem]")
{
    SECTION("when NewScene is called it should remove all existing non-tool entities")
    {
        Statics::Entity().Add(new TestEntity());
        Statics::Entity().RegisterEntities();

        Statics::Scene().NewScene();

        SECTION("and wait until the end of frame to remove all others")
        {
            REQUIRE(Statics::Entity().GetEntities().size() == 1);
            Statics::Entity().FreeEntities();
        }

        REQUIRE(Statics::Entity().GetEntities().empty());
        REQUIRE(Statics::Entity().GetTools().empty());
    }
}

TEST_CASE("scenes can be loaded from a file", "[SceneSystem]")
{
    Statics::Resource().SetBaseDirectory(SCENE_DIR);
    SECTION("when populated, it should populate the EntitySystem correctly")
    {
        Statics::Scene().QueueNextScene("scene1");
        Statics::Scene().LoadNextScene();
        Statics::Entity().RegisterEntities();

        REQUIRE(Statics::Entity().GetEntities().size() == 3);
    }

    SECTION("when a new scene is queued it should not delete entities until the end of the frame")
    {
        Statics::Scene().QueueNextScene("scene2");
        REQUIRE(Statics::Entity().GetEntities().size() == 3);

        Statics::Entity().FreeEntities();
        REQUIRE(Statics::Entity().GetEntities().empty());
    }

    SECTION("when a new scene is loaded in it should add all entities to the EntitySystem")
    {
        Statics::Scene().LoadNextScene();

        Statics::Entity().RegisterEntities();
        REQUIRE(Statics::Entity().GetEntities().size() == 1);
    }

    SECTION("when loading a scene from a non-existent file it should reset the scene")
    {
        REQUIRE(Statics::Entity().GetEntities().size() == 1);

        Statics::Scene().QueueNextScene("nonexistent");
        Statics::Entity().FreeEntities();

        Statics::Scene().LoadNextScene();
        Statics::Entity().RegisterEntities();

        REQUIRE(Statics::Entity().GetEntities().empty());
    }

    SECTION("when loading a scene from an empty string it should reset the scene")
    {
        Statics::Scene().QueueNextScene("");
        Statics::Scene().LoadNextScene();
        Statics::Entity().RegisterEntities();

        REQUIRE(Statics::Entity().GetEntities().empty());
    }

    SECTION("when loading a scene with garbage data it should reset the scene")
    {
        Statics::Scene().QueueNextScene("garbage.scene");
        Statics::Scene().LoadNextScene();
        Statics::Entity().RegisterEntities();

        REQUIRE(Statics::Entity().GetEntities().empty());
    }
}