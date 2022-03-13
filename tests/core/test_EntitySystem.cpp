#include <core/entity/Entity.h>
#include <core/entity/EntitySystem.h>
#include <utest.h>

// Define test fixture
struct test_EntitySystem
{};

UTEST_F_SETUP(test_EntitySystem)
{
    ASSERT_TRUE(Statics::Entity().GetEntities().empty());
    ASSERT_TRUE(Statics::Entity().GetTools().empty());
}

UTEST_F_TEARDOWN(test_EntitySystem)
{
    Statics::Entity().Reset();
}

UTEST_F(test_EntitySystem, AddBasicEntities)
{
    // The entity storage should support the adding of entities
    auto e1 = new Entity();
    Statics::Entity().Add(e1);

    // The storage should not contain the new entity before registration
    ASSERT_TRUE(Statics::Entity().GetEntities().empty());
    ASSERT_TRUE(Statics::Entity().GetTools().empty());

    // The storage should contain the new entity after registration
    Statics::Entity().RegisterEntities();
    ASSERT_EQ(1, Statics::Entity().GetEntities().size());
    ASSERT_TRUE(Statics::Entity().GetTools().empty());

    // It should then support adding further entities
    auto e2 = new Entity();
    auto e3 = new Entity();
    Statics::Entity().Add({e2, e3});

    // The storage should not contain the new entities before registration
    ASSERT_EQ(1, Statics::Entity().GetEntities().size());
    ASSERT_TRUE(Statics::Entity().GetTools().empty());

    // The storage should contain the new entities after registration
    Statics::Entity().RegisterEntities();
    ASSERT_EQ(3, Statics::Entity().GetEntities().size());
    ASSERT_TRUE(Statics::Entity().GetTools().empty());
}

UTEST_F(test_EntitySystem, AddToolEntities)
{
    // The entity storage should support the adding of tool entities
    auto e1 = new Entity();
    Statics::Entity().Add(e1);

    // The storage should not contain the new entity before registration
    ASSERT_TRUE(Statics::Entity().GetEntities().empty());
    ASSERT_TRUE(Statics::Entity().GetTools().empty());

    // The storage should contain the new entity after registration
    Statics::Entity().RegisterEntities();
    ASSERT_EQ(1, Statics::Entity().GetEntities().size());
    ASSERT_TRUE(Statics::Entity().GetTools().empty());

    // It should then support adding further tool entities
    auto e2 = new Entity();
    auto e3 = new Entity();
    Statics::Entity().Add({e2, e3});

    // The storage should not contain the new entities before registration
    ASSERT_EQ(1, Statics::Entity().GetEntities().size());
    ASSERT_TRUE(Statics::Entity().GetTools().empty());

    // The storage should contain the new entities after registration
    Statics::Entity().RegisterEntities();
    ASSERT_EQ(3, Statics::Entity().GetEntities().size());
    ASSERT_TRUE(Statics::Entity().GetTools().empty());
}

UTEST_F(test_EntitySystem, FreeBasicEntities)
{
    // The entity storage should support the freeing of entities
    auto e1 = new Entity();
    auto e2 = new Entity();
    auto e3 = new Entity();
    Statics::Entity().Add({e1, e2, e3});
    Statics::Entity().RegisterEntities();
    ASSERT_EQ(3, Statics::Entity().GetEntities().size());
    ASSERT_TRUE(Statics::Entity().GetTools().empty());
    e1->QueueFree();

    // The storage should still contain the entity before deregistration
    ASSERT_EQ(3, Statics::Entity().GetEntities().size());
    ASSERT_TRUE(Statics::Entity().GetTools().empty());

    // The storage should not contain the entity after deregistration
    Statics::Entity().FreeEntities();
    ASSERT_EQ(2, Statics::Entity().GetEntities().size());
    ASSERT_TRUE(Statics::Entity().GetTools().empty());

    // It should then support the freeing of further entities
    e2->QueueFree();
    e3->QueueFree();

    // The storage should still contain the entities before deregistration
    ASSERT_EQ(2, Statics::Entity().GetEntities().size());
    ASSERT_TRUE(Statics::Entity().GetTools().empty());

    // The storage should not contain the entities after deregistration
    Statics::Entity().FreeEntities();
    ASSERT_TRUE(Statics::Entity().GetEntities().empty());
    ASSERT_TRUE(Statics::Entity().GetTools().empty());
}

UTEST_F(test_EntitySystem, FreeToolEntities)
{
    // The entity storage should support the freeing of entities
    auto e1 = new Entity();
    auto e2 = new Entity();
    auto e3 = new Entity();
    Statics::Entity().Add({e1, e2, e3}, true);
    Statics::Entity().RegisterEntities();
    ASSERT_TRUE(Statics::Entity().GetEntities().empty());
    ASSERT_EQ(3, Statics::Entity().GetTools().size());
    e1->QueueFree();

    // The storage should still contain the entity before deregistration
    ASSERT_TRUE(Statics::Entity().GetEntities().empty());
    ASSERT_EQ(3, Statics::Entity().GetTools().size());

    // The storage should not contain the entity after deregistration
    Statics::Entity().FreeEntities();
    ASSERT_TRUE(Statics::Entity().GetEntities().empty());
    ASSERT_EQ(3, Statics::Entity().GetTools().size());

    // It should then support the freeing of further entities
    e2->QueueFree();
    e3->QueueFree();

    // The storage should still contain the entities before deregistration
    ASSERT_TRUE(Statics::Entity().GetEntities().empty());
    ASSERT_EQ(3, Statics::Entity().GetTools().size());

    // The storage should not contain the entities after deregistration
    Statics::Entity().FreeEntities();
    ASSERT_TRUE(Statics::Entity().GetEntities().empty());
    ASSERT_EQ(3, Statics::Entity().GetTools().size());
}

UTEST_F(test_EntitySystem, SortEntitiesByZIndex)
{
    // Entities in storage should stay sorted by z-index
    auto* e1 = new Entity("e1", Xform(), 2);
    auto* e2 = new Entity("e2", Xform(), 5);
    auto* e3 = new Entity("e3", Xform(), 1);
    auto* e4 = new Entity("e4", Xform(), 3);
    Statics::Entity().Add({e1, e2, e3, e4});
    Statics::Entity().RegisterEntities();
    ASSERT_EQ(4, Statics::Entity().GetEntities().size());
    ASSERT_TRUE(Statics::Entity().GetTools().empty());

    // Entities should be sorted from lowest to highest index
    const std::vector<Entity*>& entities = Statics::Entity().GetEntities();
    ASSERT_EQ(e3, entities[0]);
    ASSERT_EQ(e1, entities[1]);
    ASSERT_EQ(e4, entities[2]);
    ASSERT_EQ(e2, entities[3]);

    // Lowering the z-index of one entity should re-sort the rest
    e1->SetZIndex(0);
    ASSERT_EQ(e1, entities[0]);
    ASSERT_EQ(e3, entities[1]);
    ASSERT_EQ(e4, entities[2]);
    ASSERT_EQ(e2, entities[3]);

    // Raising the z-index of one entity should re-sort the rest
    e1->SetZIndex(20);
    ASSERT_EQ(e3, entities[0]);
    ASSERT_EQ(e4, entities[1]);
    ASSERT_EQ(e2, entities[2]);
    ASSERT_EQ(e1, entities[3]);
}