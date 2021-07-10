#include "catch.hpp"
#include "../src/engine/entity/Entity.h"
#include "../src/engine/entity/EntityStorage.h"

TEST_CASE("entities can be added to storage", "[EntityStorage]")
{
    REQUIRE(EntityStorage::GetEntities().empty());
    REQUIRE(EntityStorage::GetTools().empty());

    SECTION("when adding one entity to storage")
    {
        auto e1 = new Entity();
        EntityStorage::Add(e1);

        SECTION("storage should not be increased before registration")
        {
            REQUIRE(EntityStorage::GetEntities().empty());
            REQUIRE(EntityStorage::GetTools().empty());
        }

        EntityStorage::RegisterEntities();

        SECTION("storage should be increased by one after registration")
        {
            REQUIRE(EntityStorage::GetEntities().size() == 1);
            REQUIRE(EntityStorage::GetTools().empty());
        }

        SECTION("and adding two more entities to storage")
        {
            auto e2 = new Entity();
            auto e3 = new Entity();
            EntityStorage::Add({e2, e3});

            SECTION("storage should not be increased before registration")
            {
                REQUIRE(EntityStorage::GetEntities().size() == 1);
                REQUIRE(EntityStorage::GetTools().empty());
            }

            EntityStorage::RegisterEntities();

            SECTION("storage should be increased by two after registration")
            {
                REQUIRE(EntityStorage::GetEntities().size() == 3);
                REQUIRE(EntityStorage::GetTools().empty());
            }
        }
    }

    SECTION("when adding one tool entity to storage")
    {
        auto e1 = new Entity();
        EntityStorage::Add(e1, true);

        SECTION("storage should not be increased before registration")
        {
            REQUIRE(EntityStorage::GetEntities().empty());
            REQUIRE(EntityStorage::GetTools().empty());
        }

        EntityStorage::RegisterEntities();

        SECTION("storage should be increased by one after registration")
        {
            REQUIRE(EntityStorage::GetEntities().empty());
            REQUIRE(EntityStorage::GetTools().size() == 1);
        }

        SECTION("and adding two more tool entities to storage")
        {
            auto e2 = new Entity();
            auto e3 = new Entity();
            EntityStorage::Add({e2, e3}, true);

            SECTION("storage should not be increased before registration")
            {
                REQUIRE(EntityStorage::GetEntities().empty());
                REQUIRE(EntityStorage::GetTools().size() == 1);
            }

            EntityStorage::RegisterEntities();

            SECTION("storage should be increased by two after registration")
            {
                REQUIRE(EntityStorage::GetEntities().empty());
                REQUIRE(EntityStorage::GetTools().size() == 3);
            }
        }
    }

    EntityStorage::Reset();
}

TEST_CASE("entities can be freed from storage", "[EntityStorage]")
{
    REQUIRE(EntityStorage::GetEntities().empty());
    REQUIRE(EntityStorage::GetTools().empty());

    auto e1 = new Entity();
    auto e2 = new Entity();
    auto e3 = new Entity();
    EntityStorage::Add({e1, e2, e3});
    EntityStorage::RegisterEntities();

    REQUIRE(EntityStorage::GetEntities().size() == 3);
    REQUIRE(EntityStorage::GetTools().empty());

    SECTION("when queuing one entity to be freed")
    {
        e1->QueueFree();

        SECTION("storage should have the same amount of entities before freeing")
        {
            REQUIRE(EntityStorage::GetEntities().size() == 3);
            REQUIRE(EntityStorage::GetTools().empty());
        }

        SECTION("storage should have one less entity after freeing")
        {
            EntityStorage::FreeEntities();
            REQUIRE(EntityStorage::GetEntities().size() == 2);
            REQUIRE(EntityStorage::GetTools().empty());
        }

        SECTION("and queuing another two entities to be freed")
        {
            e2->QueueFree();
            e3->QueueFree();

            SECTION("storage should have the same amount of entities before freeing")
            {
                REQUIRE(EntityStorage::GetEntities().size() == 3);
                REQUIRE(EntityStorage::GetTools().empty());
            }

            EntityStorage::FreeEntities();

            SECTION("storage should have no entities after freeing")
            {
                REQUIRE(EntityStorage::GetEntities().empty());
                REQUIRE(EntityStorage::GetTools().empty());
            }
        }
    }

    EntityStorage::Reset();
}

TEST_CASE("entities stay sorted by z-index", "[EntityStorage]")
{
    REQUIRE(EntityStorage::GetEntities().empty());
    REQUIRE(EntityStorage::GetTools().empty());

    SECTION("when three adding entities with specific z-indices")
    {
        Entity* e1 = new Entity("e1", raylib::Vector3::Zero(), 0.f, 2);
        Entity* e2 = new Entity("e2", raylib::Vector3::Zero(), 0.f, 5);
        Entity* e3 = new Entity("e3", raylib::Vector3::Zero(), 0.f, 1);
        Entity* e4 = new Entity("e4", raylib::Vector3::Zero(), 0.f, 3);
        EntityStorage::Add({e1, e2, e3, e4});
        EntityStorage::RegisterEntities();

        REQUIRE(EntityStorage::GetEntities().size() == 4);
        REQUIRE(EntityStorage::GetTools().empty());

        SECTION("entities should be sorted from lowest to highest index")
        {
            const std::vector<Entity*>& entities = EntityStorage::GetEntities();
            REQUIRE(entities[0] == e3);
            REQUIRE(entities[1] == e1);
            REQUIRE(entities[2] == e4);
            REQUIRE(entities[3] == e2);

            SECTION("and lowering the z-index of one entity should re-sort the rest")
            {
                e1->SetZIndex(0);

                SECTION("entities should be still be sorted from lowest to highest index")
                {
                    REQUIRE(entities[0] == e1);
                    REQUIRE(entities[1] == e3);
                    REQUIRE(entities[2] == e4);
                    REQUIRE(entities[3] == e2);
                }
            }

            SECTION("and raising the z-index of one entity should re-sort the rest")
            {
                e1->SetZIndex(20);

                SECTION("entities should be still be sorted from lowest to highest index")
                {
                    REQUIRE(entities[0] == e3);
                    REQUIRE(entities[1] == e4);
                    REQUIRE(entities[2] == e2);
                    REQUIRE(entities[3] == e1);
                }
            }
        }
    }

    EntityStorage::Reset();
}
