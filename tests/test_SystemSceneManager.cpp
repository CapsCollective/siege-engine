#include <entity/Entity.h>
#include <entity/EntityStorage.h>
#include <scene/SceneFile.h>
#include <scene/SceneManager.h>

#include <filesystem>
#include <fstream>
#include <string>

#include "catch.hpp"

// Const declarations
static constexpr const char* SCENE_DIR = "tests/data/";
static constexpr const char* TEST_FILE = "test.scene";
static constexpr const char* UNTITLED_FILE = "untitled.scene";

// Helper methods

std::string Filepath(const char* filename)
{
    return std::string(SCENE_DIR) + filename;
}

bool FileExists(const std::string& dir)
{
    namespace fs = std::filesystem;
    fs::path f {dir};
    return fs::exists(f);
}

std::string GetFileContent(const std::string& dir)
{
    std::ifstream ifs(dir);
    std::string content({
        std::istreambuf_iterator<char>(ifs),
        std::istreambuf_iterator<char>(),
    });
    return content;
}

// Test entity
class TestEntity : public Entity
{
public:

    static const std::string ENTITY_NAME;

    TestEntity() : Entity(ENTITY_NAME) {}

    explicit TestEntity(Xform transform) : Entity(ENTITY_NAME, transform) {}
};

const std::string TestEntity::ENTITY_NAME("TestEntity");

REGISTER_SERIALISATION_INTERFACE(
    TestEntity::ENTITY_NAME,
    [](Entity* entity) -> String { return ""; },
    [](const EntityData& data, const std::vector<String>& args) -> Entity* {
        return new TestEntity(Xform(data.position, data.rotation));
    });

TEST_CASE("Scenes can be saved to a file", "[SceneManager]")
{
    ResourceManagerFacade::SetBaseDirectory(SCENE_DIR);
    SceneManager::NewScene();

    SECTION("when an empty scene is saved it should create a file in the correct directory")
    {
        SceneManager::SaveScene("test");

        REQUIRE(FileExists(Filepath(TEST_FILE)));
        REQUIRE(GetFileContent(Filepath(TEST_FILE)).empty());
    }

    SECTION("When an entity is added to the scene, it should be saved to the scene file.")
    {
        EntityStorage::Add(new TestEntity());
        EntityStorage::RegisterEntities();

        SceneManager::SaveScene("test");

        REQUIRE(FileExists(Filepath(TEST_FILE)));
        std::string content = GetFileContent(Filepath(TEST_FILE));
        REQUIRE(!content.empty());
        REQUIRE(content.find("TestEntity") != std::string::npos);
    }

    SECTION("When an entity is removed from the scene, it should remove it from the file as well.")
    {
        EntityStorage::Reset();

        SceneManager::SaveScene("test");

        REQUIRE(FileExists(Filepath(TEST_FILE)));
        std::string content = GetFileContent(Filepath(TEST_FILE));
        REQUIRE(content.empty());
        REQUIRE(content.find("TestEntity") == std::string::npos);
    }

    SECTION("when a non-serialisable entity is added to the scene it should not be saved")
    {
        EntityStorage::Add(new Entity());
        SceneManager::SaveScene("test");

        REQUIRE(FileExists(Filepath(TEST_FILE)));
        std::string content = GetFileContent(Filepath(TEST_FILE));
        REQUIRE(content.empty());
    }

    SECTION("when an empty string is passed into SaveScene, it should create 'untitled.scene'")
    {
        SceneManager::SaveScene("");
        std::string filepath = Filepath(UNTITLED_FILE);
        REQUIRE(FileExists(filepath));
        remove(filepath.c_str());
    }

    EntityStorage::Reset();
    remove(Filepath(TEST_FILE).c_str());
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
    ResourceManagerFacade::SetBaseDirectory(SCENE_DIR);
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