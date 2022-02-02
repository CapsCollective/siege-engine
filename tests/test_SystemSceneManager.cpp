#include <entity/Entity.h>
#include <entity/EntityStorage.h>
#include <render/ResourceManager.h>
#include <scene/SceneFile.h>
#include <scene/SceneManager.h>
#include <utils/FileSystem.h>
#include <utils/Statics.h>
#include <utils/String.h>

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

TEST_CASE("Scenes can be saved to a file", "[SceneManager]")
{
    Statics::ResourceManager.SetBaseDirectory(SCENE_DIR);
    SceneManager::NewScene();

    SECTION("when an empty scene is saved it should create a file in the correct directory")
    {
        SceneManager::SaveScene("test");

        REQUIRE(FileSystem::Exists(Filepath(TEST_FILE)));
        String content = FileSystem::Read(Filepath(TEST_FILE));
        REQUIRE(!content);
    }

    SECTION("When an entity is added to the scene, it should be saved to the scene file.")
    {
        EntityStorage::Add(new TestEntity());
        EntityStorage::RegisterEntities();

        SceneManager::SaveScene("test");

        REQUIRE(FileSystem::Exists(Filepath(TEST_FILE)));
        String content = FileSystem::Read(Filepath(TEST_FILE));
        REQUIRE(!content.IsEmpty());
        REQUIRE(content.Find("TestEntity") != -1);
    }

    SECTION("When an entity is removed from the scene, it should remove it from the file as well.")
    {
        EntityStorage::Reset();

        SceneManager::SaveScene("test");

        REQUIRE(FileSystem::Exists(Filepath(TEST_FILE)));
        String content = FileSystem::Read(Filepath(TEST_FILE));
        REQUIRE(content.IsEmpty());
        REQUIRE(content.Find("TestEntity") == -1);
    }

    SECTION("when a non-serialisable entity is added to the scene it should not be saved")
    {
        EntityStorage::Add(new Entity());
        SceneManager::SaveScene("test");

        REQUIRE(FileSystem::Exists(Filepath(TEST_FILE)));
        String content = FileSystem::Read(Filepath(TEST_FILE));
        REQUIRE(content.IsEmpty());
    }

    SECTION("when an empty string is passed into SaveScene, it should create 'untitled.scene'")
    {
        SceneManager::SaveScene("");
        String filepath = Filepath(UNTITLED_FILE);
        REQUIRE(FileSystem::Exists(filepath));
        remove(filepath);
    }

    EntityStorage::Reset();
    remove(Filepath(TEST_FILE));
}

TEST_CASE("scenes are erased when a new scene is created", "[SceneManager]")
{
    SECTION("when NewScene is called it should remove all existing non-tool entities")
    {
        EntityStorage::Add(new TestEntity());
        EntityStorage::RegisterEntities();

        SceneManager::NewScene();

        SECTION("and wait until the end of frame to remove all others")
        {
            REQUIRE(EntityStorage::GetEntities().size() == 1);
            EntityStorage::FreeEntities();
        }

        REQUIRE(EntityStorage::GetEntities().empty());
        REQUIRE(EntityStorage::GetTools().empty());
    }
}

TEST_CASE("scenes can be loaded from a file", "[SceneManager]")
{
    Statics::ResourceManager.SetBaseDirectory(SCENE_DIR);
    SECTION("when populated, it should populate the EntityStorage correctly")
    {
        SceneManager::QueueNextScene("scene1");
        SceneManager::LoadNextScene();
        EntityStorage::RegisterEntities();

        REQUIRE(EntityStorage::GetEntities().size() == 3);
    }

    SECTION("when a new scene is queued it should not delete entities until the end of the frame")
    {
        SceneManager::QueueNextScene("scene2");
        REQUIRE(EntityStorage::GetEntities().size() == 3);

        EntityStorage::FreeEntities();
        REQUIRE(EntityStorage::GetEntities().empty());
    }

    SECTION("when a new scene is loaded in it should add all entities to the EntityStorage")
    {
        SceneManager::LoadNextScene();

        EntityStorage::RegisterEntities();
        REQUIRE(EntityStorage::GetEntities().size() == 1);
    }

    SECTION("when loading a scene from a non-existent file it should reset the scene")
    {
        REQUIRE(EntityStorage::GetEntities().size() == 1);

        SceneManager::QueueNextScene("nonexistent");
        EntityStorage::FreeEntities();

        SceneManager::LoadNextScene();
        EntityStorage::RegisterEntities();

        REQUIRE(EntityStorage::GetEntities().empty());
    }

    SECTION("when loading a scene from an empty string it should reset the scene")
    {
        SceneManager::QueueNextScene("");
        SceneManager::LoadNextScene();
        EntityStorage::RegisterEntities();

        REQUIRE(EntityStorage::GetEntities().empty());
    }

    SECTION("when loading a scene with garbage data it should reset the scene")
    {
        SceneManager::QueueNextScene("garbage.scene");
        SceneManager::LoadNextScene();
        EntityStorage::RegisterEntities();

        REQUIRE(EntityStorage::GetEntities().empty());
    }
}