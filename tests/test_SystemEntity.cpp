#include <engine/entity/Entity.h>
#include <engine/entity/EntitySystem.h>

#include "catch.hpp"

TEST_CASE("entities can be added to storage", "[EntitySystem]")
{
    REQUIRE(Statics::Entity().GetEntities().empty());
    REQUIRE(Statics::Entity().GetTools().empty());

    SECTION("when adding one entity to storage")
    {
        auto e1 = new Entity();
        Statics::Entity().Add(e1);

        SECTION("storage should not be increased before registration")
        {
            REQUIRE(Statics::Entity().GetEntities().empty());
            REQUIRE(Statics::Entity().GetTools().empty());
        }

        Statics::Entity().RegisterEntities();

        SECTION("storage should be increased by one after registration")
        {
            REQUIRE(Statics::Entity().GetEntities().size() == 1);
            REQUIRE(Statics::Entity().GetTools().empty());
        }

        SECTION("and adding two more entities to storage")
        {
            auto e2 = new Entity();
            auto e3 = new Entity();
            Statics::Entity().Add({e2, e3});

            SECTION("storage should not be increased before registration")
            {
                REQUIRE(Statics::Entity().GetEntities().size() == 1);
                REQUIRE(Statics::Entity().GetTools().empty());
            }

            Statics::Entity().RegisterEntities();

            SECTION("storage should be increased by two after registration")
            {
                REQUIRE(Statics::Entity().GetEntities().size() == 3);
                REQUIRE(Statics::Entity().GetTools().empty());
            }
        }
    }

    SECTION("when adding one tool entity to storage")
    {
        auto e1 = new Entity();
        Statics::Entity().Add(e1, true);

        SECTION("storage should not be increased before registration")
        {
            REQUIRE(Statics::Entity().GetEntities().empty());
            REQUIRE(Statics::Entity().GetTools().empty());
        }

        Statics::Entity().RegisterEntities();

        SECTION("storage should be increased by one after registration")
        {
            REQUIRE(Statics::Entity().GetEntities().empty());
            REQUIRE(Statics::Entity().GetTools().size() == 1);
        }

        SECTION("and adding two more tool entities to storage")
        {
            auto e2 = new Entity();
            auto e3 = new Entity();
            Statics::Entity().Add({e2, e3}, true);

            SECTION("storage should not be increased before registration")
            {
                REQUIRE(Statics::Entity().GetEntities().empty());
                REQUIRE(Statics::Entity().GetTools().size() == 1);
            }

            Statics::Entity().RegisterEntities();

            SECTION("storage should be increased by two after registration")
            {
                REQUIRE(Statics::Entity().GetEntities().empty());
                REQUIRE(Statics::Entity().GetTools().size() == 3);
            }
        }
    }

    Statics::Entity().Reset();
}

TEST_CASE("entities can be freed from storage", "[EntitySystem]")
{
    REQUIRE(Statics::Entity().GetEntities().empty());
    REQUIRE(Statics::Entity().GetTools().empty());

    auto e1 = new Entity();
    auto e2 = new Entity();
    auto e3 = new Entity();
    Statics::Entity().Add({e1, e2, e3});
    Statics::Entity().RegisterEntities();

    REQUIRE(Statics::Entity().GetEntities().size() == 3);
    REQUIRE(Statics::Entity().GetTools().empty());

    SECTION("when queuing one entity to be freed")
    {
        e1->QueueFree();

        SECTION("storage should have the same amount of entities before freeing")
        {
            REQUIRE(Statics::Entity().GetEntities().size() == 3);
            REQUIRE(Statics::Entity().GetTools().empty());
        }

        SECTION("storage should have one less entity after freeing")
        {
            Statics::Entity().FreeEntities();
            REQUIRE(Statics::Entity().GetEntities().size() == 2);
            REQUIRE(Statics::Entity().GetTools().empty());
        }

        SECTION("and queuing another two entities to be freed")
        {
            e2->QueueFree();
            e3->QueueFree();

            SECTION("storage should have the same amount of entities before freeing")
            {
                REQUIRE(Statics::Entity().GetEntities().size() == 3);
                REQUIRE(Statics::Entity().GetTools().empty());
            }

            Statics::Entity().FreeEntities();

            SECTION("storage should have no entities after freeing")
            {
                REQUIRE(Statics::Entity().GetEntities().empty());
                REQUIRE(Statics::Entity().GetTools().empty());
            }
        }
    }

    Statics::Entity().Reset();
}

TEST_CASE("entities stay sorted by z-index", "[EntitySystem]")
{
    REQUIRE(Statics::Entity().GetEntities().empty());
    REQUIRE(Statics::Entity().GetTools().empty());

    SECTION("when three adding entities with specific z-indices")
    {
        auto* e1 = new Entity("e1", Xform(), 2);
        auto* e2 = new Entity("e2", Xform(), 5);
        auto* e3 = new Entity("e3", Xform(), 1);
        auto* e4 = new Entity("e4", Xform(), 3);
        Statics::Entity().Add({e1, e2, e3, e4});
        Statics::Entity().RegisterEntities();

        REQUIRE(Statics::Entity().GetEntities().size() == 4);
        REQUIRE(Statics::Entity().GetTools().empty());

        SECTION("entities should be sorted from lowest to highest index")
        {
            const std::vector<Entity*>& entities = Statics::Entity().GetEntities();
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

    Statics::Entity().Reset();
}
