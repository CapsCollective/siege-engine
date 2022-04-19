#include <core/Statics.h>
#include <core/entity/Entity.h>
#include <core/render/ResourceSystem.h>
#include <core/scene/SceneFile.h>
#include <core/scene/SceneSystem.h>
#include <utest.h>
#include <utils/FileSystem.h>
#include <utils/String.h>

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

// Define test fixture
struct test_SceneSystem
{};

UTEST_F_SETUP(test_SceneSystem)
{
    Statics::Resource().SetBaseDirectory(SCENE_DIR);
    Statics::Scene().NewScene();
}

UTEST_F_TEARDOWN(test_SceneSystem)
{
    Statics::Entity().Reset();
    remove(Filepath(TEST_FILE));
}

UTEST_F(test_SceneSystem, SaveEmptyScene)
{
    // When an empty scene is saved, it should create a file in the correct directory
    Statics::Scene().SaveScene("test");

    ASSERT_TRUE(FileSystem::Exists(Filepath(TEST_FILE)));
    String content = FileSystem::Read(Filepath(TEST_FILE));
    ASSERT_FALSE(content);
}

UTEST_F(test_SceneSystem, SaveNonEmptyScene)
{
    // When an entity is added to the scene, it should be saved to the scene file
    Statics::Entity().Add(new TestEntity());
    Statics::Entity().RegisterEntities();
    Statics::Scene().SaveScene("test");

    ASSERT_TRUE(FileSystem::Exists(Filepath(TEST_FILE)));
    String content = FileSystem::Read(Filepath(TEST_FILE));
    ASSERT_FALSE(content.IsEmpty());
    ASSERT_NE(-1, content.Find("TestEntity"));
}

UTEST_F(test_SceneSystem, SaveClearedScene)
{
    // When an entity is removed from the scene, it should not be serialised
    Statics::Entity().Add(new TestEntity());
    Statics::Entity().RegisterEntities();
    Statics::Entity().Reset();
    Statics::Scene().SaveScene("test");

    ASSERT_TRUE(FileSystem::Exists(Filepath(TEST_FILE)));
    String content = FileSystem::Read(Filepath(TEST_FILE));
    ASSERT_TRUE(content.IsEmpty());
    ASSERT_EQ(-1, content.Find("TestEntity"));
}

UTEST_F(test_SceneSystem, SaveNonSerialisableEntity)
{
    // When a non-serialisable entity is added to the scene it should not be saved
    Statics::Entity().Add(new Entity());
    Statics::Scene().SaveScene("test");

    ASSERT_TRUE(FileSystem::Exists(Filepath(TEST_FILE)));
    String content = FileSystem::Read(Filepath(TEST_FILE));
    ASSERT_TRUE(content.IsEmpty());
}

UTEST_F(test_SceneSystem, SaveUnnamedScene)
{
    // When an empty string is passed into SaveScene, it should create "untitled.scene"
    Statics::Scene().SaveScene("");
    String filepath = Filepath(UNTITLED_FILE);

    ASSERT_TRUE(FileSystem::Exists(filepath));
    remove(filepath);
}

UTEST_F(test_SceneSystem, EraseOnNewScene)
{
    // When a new scene is requested, the current scene should remove all non-tool entities
    Statics::Entity().Add(new TestEntity());
    Statics::Entity().RegisterEntities();
    Statics::Scene().NewScene();

    ASSERT_EQ(1, Statics::Entity().GetEntities().size());
    Statics::Entity().FreeEntities();

    // It should remove them at the end of the frame
    ASSERT_TRUE(Statics::Entity().GetEntities().empty());
}

UTEST_F(test_SceneSystem, LoadNonEmptyScene)
{
    // When loading a non-empty file, it should populate the EntitySystem correctly
    Statics::Scene().QueueNextScene("scene1");
    Statics::Scene().LoadNextScene();
    Statics::Entity().RegisterEntities();

    ASSERT_EQ(3, Statics::Entity().GetEntities().size());

    // However, it should not delete entities until the end of the frame
    Statics::Scene().QueueNextScene("scene2");
    ASSERT_EQ(3, Statics::Entity().GetEntities().size());
    Statics::Entity().FreeEntities();
    ASSERT_TRUE(Statics::Entity().GetEntities().empty());

    // It should then add all entities when a new scene is loaded
    Statics::Scene().LoadNextScene();
    Statics::Entity().RegisterEntities();
    ASSERT_EQ(1, Statics::Entity().GetEntities().size());
}

UTEST_F(test_SceneSystem, LoadNonExistentScene)
{
    // When loading a scene from a non-existent file it should reset the scene
    Statics::Entity().Add(new TestEntity());
    Statics::Entity().RegisterEntities();
    ASSERT_EQ(1, Statics::Entity().GetEntities().size());

    Statics::Scene().QueueNextScene("nonexistent");
    Statics::Entity().FreeEntities();
    Statics::Scene().LoadNextScene();
    Statics::Entity().RegisterEntities();
    ASSERT_TRUE(Statics::Entity().GetEntities().empty());
}

UTEST_F(test_SceneSystem, LoadUnnamedScene)
{
    // When loading a scene from an empty string it should reset the scene
    Statics::Entity().Add(new TestEntity());
    Statics::Entity().RegisterEntities();
    ASSERT_EQ(1, Statics::Entity().GetEntities().size());

    Statics::Scene().QueueNextScene("");
    Statics::Entity().FreeEntities();
    Statics::Scene().LoadNextScene();
    Statics::Entity().RegisterEntities();
    ASSERT_TRUE(Statics::Entity().GetEntities().empty());
}

UTEST_F(test_SceneSystem, LoadGarbageScene)
{
    // When loading a scene with garbage data it should reset the scene
    Statics::Entity().Add(new TestEntity());
    Statics::Entity().RegisterEntities();
    ASSERT_EQ(1, Statics::Entity().GetEntities().size());

    Statics::Scene().QueueNextScene("garbage.scene");
    Statics::Entity().FreeEntities();
    Statics::Scene().LoadNextScene();
    Statics::Entity().RegisterEntities();
    ASSERT_TRUE(Statics::Entity().GetEntities().empty());
}