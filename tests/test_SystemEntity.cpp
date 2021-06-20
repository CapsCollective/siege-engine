#include "catch.hpp"
#include "../src/systems/entity/Entity.h"
#include "../src/systems/entity/EntityStorage.h"

TEST_CASE( "entities can be added to storage", "[EntityStorage]" ) {
    Entity e;
    EntityStorage::Register(&e);

    REQUIRE(EntityStorage::GetEntities().empty());

    EntityStorage::RegisterEntities();

    REQUIRE(EntityStorage::GetEntities().size() == 1);
}