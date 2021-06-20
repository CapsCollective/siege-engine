#include "catch_amalgamated.hpp"
#include "../src/systems/entity/EntityStorage.h"
#include "../src/entities/SplashScreen.h"

TEST_CASE( "entities can be added to storage", "[EntityStorage]" ) {
    SplashScreen e;
    EntityStorage::Register(&e);

    REQUIRE(EntityStorage::GetEntities().empty());

    EntityStorage::RegisterEntities();

    REQUIRE(EntityStorage::GetEntities().size() == 1);
}