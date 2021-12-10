#include "catch.hpp"
#include <scene/SceneManager.h>
#include <entity/EntityStorage.h>
#include <entity/Entity.h>
#include <scene/SceneSerialiser.h>
#include <fstream>
#include <filesystem>
#include <string>

// Const declarations
const std::string SCENE_DIR = "tests/data/";
const std::string FILE_NAME = "test.scene";
const std::string FULL_TEST_PATH = SCENE_DIR + FILE_NAME;

// Helper methods
bool FileExists(const std::string& dir)
{
    namespace fs = std::filesystem;
    fs::path f{ dir };
    return fs::exists(f);
}

std::string GetFileContent(const std::string& dir)
{
    std::ifstream ifs(dir);
    std::string content(
            (std::istreambuf_iterator<char>(ifs)),
            (std::istreambuf_iterator<char>()));
    return content;
}

// Test entity
class TestEntity : public Entity
{
public:

    static const std::string ENTITY_NAME;

    TestEntity() = default;

    explicit TestEntity(Xform transform) :
        Entity(ENTITY_NAME, transform) {}
};

const std::string TestEntity::ENTITY_NAME("TestEntity");

REGISTER_SERIALISATION_INTERFACE(TestEntity::ENTITY_NAME,
        [](Entity* entity) -> std::string {
            return "";
        },
        [](const EntityData& data, const std::vector<std::string>& args) -> Entity* {
            return new TestEntity(Xform(data.position, data.rotation));
        });


TEST_CASE("Scenes can be saved to a file", "[SceneManager]")
{
    SceneManager::SetBaseDirectory(SCENE_DIR);
    SceneManager::NewScene();

    SECTION("when an empty scene is saved it should create a file in the correct directory")
    {
        SceneManager::SaveScene("test");

        REQUIRE(FileExists(FULL_TEST_PATH));
        REQUIRE(GetFileContent(FULL_TEST_PATH).empty());
    }

    SECTION("When an entity is added to the scene, it should be saved to the scene file.")
    {
        EntityStorage::Add(new TestEntity());
        EntityStorage::RegisterEntities();

        SceneManager::SaveScene("test");

        REQUIRE(FileExists(FULL_TEST_PATH));
        std::string content = GetFileContent(FULL_TEST_PATH);
        REQUIRE(!content.empty());
        REQUIRE(content.find("TestEntity") != std::string::npos);
    }

    SECTION("When an entity is removed from the scene, it should remove it from the file as well.")
    {
        EntityStorage::Reset();

        SceneManager::SaveScene("test");

        REQUIRE(FileExists(FULL_TEST_PATH));
        std::string content = GetFileContent(FULL_TEST_PATH);
        REQUIRE(content.empty());
        REQUIRE(content.find("TestEntity") == std::string::npos);
    }

    SECTION("when a non-serialisable entity is added to the scene it should not be saved")
    {
        EntityStorage::Add(new Entity());
        SceneManager::SaveScene("test");

        REQUIRE(FileExists(FULL_TEST_PATH));
        std::string content = GetFileContent(FULL_TEST_PATH);
        REQUIRE(content.empty());
    }

    SECTION("when an empty string is passed into SaveScene, it should create a file called 'untitled.scene'")
    {
        SceneManager::SaveScene("");
        REQUIRE(FileExists(SCENE_DIR + "untitled.scene"));
        remove(std::string(SCENE_DIR + "untitled.scene").c_str());
    }

    // TODO refactor this to test the default
//    SECTION("when an empty string is passed in for the directory it should create the file in the current directory")
//    {
//        SceneManager::SaveScene("test", "");
//        REQUIRE(FileExists(FILE_NAME));
//        remove(FILE_NAME.c_str());
//    }

    EntityStorage::Reset();
    remove(FULL_TEST_PATH.c_str());
}

TEST_CASE("scenes are erased when a new scene is created", "[SceneManager]")
{
    SECTION("when NewScene is called it should remove all existing non-tool entities")
    {
        EntityStorage::Add(new TestEntity());
        EntityStorage::RegisterEntities();

        SceneManager::NewScene();

        SECTION("when a new scene is declared it should not immediately remove all entities but rather "
                "wait until the end of frame to remove them")
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
    SceneManager::SetBaseDirectory(SCENE_DIR);
    SECTION("when a scene is loaded from a populated file it should populate the EntityStorage correctly")
    {
        SceneManager::QueueNextScene("scene1");
        SceneManager::LoadNextScene();
        EntityStorage::RegisterEntities();

        REQUIRE(EntityStorage::GetEntities().size() == 3);
    }

    SECTION("when a new scene is queued it should not delete all entities until the end of the frame")
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