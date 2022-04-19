#include <core/entity/Entity.h>
#include <core/entity/EntitySystem.h>
#include <utest.h>

UTEST(test_EntitySystem, AddEntities)
{
    // The entity storage should support the adding of entities
    EntitySystem system;
    auto e1 = new Entity();
    system.Add(e1);

    // The storage should not contain the new entity before registration
    ASSERT_TRUE(system.GetEntities().empty());

    // The storage should contain the new entity after registration
    system.RegisterEntities();
    ASSERT_EQ(1, system.GetEntities().size());

    // It should then support adding further entities
    auto e2 = new Entity();
    auto e3 = new Entity();
    system.Add({e2, e3});

    // The storage should not contain the new entities before registration
    ASSERT_EQ(1, system.GetEntities().size());

    // The storage should contain the new entities after registration
    system.RegisterEntities();
    ASSERT_EQ(3, system.GetEntities().size());
}

UTEST(test_EntitySystem, FreeEntities)
{
    // The entity storage should support the freeing of entities
    EntitySystem system;
    auto e1 = new Entity();
    auto e2 = new Entity();
    auto e3 = new Entity();
    system.Add({e1, e2, e3});
    system.RegisterEntities();

    GenerationalIndex idx1 = e1->GetIndex();
    GenerationalIndex idx2 = e2->GetIndex();
    GenerationalIndex idx3 = e3->GetIndex();

    // The entities should all be live
    ASSERT_TRUE(system.IsLive(idx1));
    ASSERT_TRUE(system.IsLive(idx2));
    ASSERT_TRUE(system.IsLive(idx3));

    ASSERT_EQ(3, system.GetEntities().size());
    e1->QueueFree();

    // The storage should still contain the entity before deregistration
    ASSERT_EQ(3, system.GetEntities().size());

    // The storage should not contain the entity after deregistration
    system.FreeEntities();
    ASSERT_EQ(2, system.GetEntities().size());

    // The entity should no longer be live after freeing
    ASSERT_FALSE(system.IsLive(idx1));
    ASSERT_TRUE(system.IsLive(idx2));
    ASSERT_TRUE(system.IsLive(idx3));

    // It should then support the freeing of further entities
    e2->QueueFree();
    e3->QueueFree();

    // The storage should still contain the entities before deregistration
    ASSERT_EQ(2, system.GetEntities().size());

    // The storage should not contain the entities after deregistration
    system.FreeEntities();
    ASSERT_TRUE(system.GetEntities().empty());

    // The entities should no longer be live after freeing
    ASSERT_FALSE(system.IsLive(idx1));
    ASSERT_FALSE(system.IsLive(idx2));
    ASSERT_FALSE(system.IsLive(idx3));

    auto e4 = new Entity();
    system.Add(e4);
    GenerationalIndex idx4 = e4->GetIndex();

    // Only the added entity should be live
    ASSERT_FALSE(system.IsLive(idx1));
    ASSERT_FALSE(system.IsLive(idx2));
    ASSERT_FALSE(system.IsLive(idx3));
    ASSERT_TRUE(system.IsLive(idx4));

    // The storage should contain the newly registered entity
    system.RegisterEntities();
    ASSERT_EQ(1, system.GetEntities().size());

    // The storage should support temporarily disabling deregistration
    system.SetAllowDeregistration(false);
    e4->QueueFree();

    // The storage should still contain the entity after deregistration
    system.FreeEntities();
    ASSERT_TRUE(system.IsLive(idx4));
    ASSERT_EQ(1, system.GetEntities().size());

    // The storage should support re-enabling deregistration
    system.SetAllowDeregistration(true);
    e4->QueueFree();

    // The storage should no longer contain the entity after deregistration
    system.FreeEntities();
    ASSERT_FALSE(system.IsLive(idx4));
    ASSERT_TRUE(system.GetEntities().empty());
}

UTEST(test_EntitySystem, SortEntitiesByZIndex)
{
    // Entities in storage should stay sorted by z-index
    EntitySystem system;
    auto* e1 = new Entity("e1", Xform(), 2);
    auto* e2 = new Entity("e2", Xform(), 5);
    auto* e3 = new Entity("e3", Xform(), 1);
    auto* e4 = new Entity("e4", Xform(), 3);
    system.Add({e1, e2, e3, e4});
    system.RegisterEntities();
    ASSERT_EQ(4, system.GetEntities().size());

    // Entities should be sorted from lowest to highest index
    const std::vector<Entity*>& entities = system.GetEntities();
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