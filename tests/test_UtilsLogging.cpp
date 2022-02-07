#include "catch.hpp"

// Require logging
#define CC_ENABLE_LOGGING

#include <engine/utils/Logging.h>

TEST_CASE("messages can be formatted correctly", "[Logging]")
{
    SECTION("when given the correct number of points and values it should format correctly")
    {
        Logging::VariantContainer vc[] {1, "$$", 12.300000f, 1.2f, Vec3::Zero};
        String fmt = "{} is {} very {} well {} formatted {}!";
        Logging::VariantFormat(fmt, vc, sizeof(vc) / sizeof(Logging::VariantContainer));
        REQUIRE(fmt == "1 is $$ very 12.300000 well 1.200000 formatted Vector3(0.00,0.00,0.00)!");
    }

    SECTION("when given too few replacement points for values it should only use those it can fit")
    {
        Logging::VariantContainer vc[] {"hello", 17.3, "$$"};
        String fmt = "This {} is {} too short!";
        Logging::VariantFormat(fmt, vc, sizeof(vc) / sizeof(Logging::VariantContainer));
        REQUIRE(fmt == "This hello is 17.30000000000000071 too short!");
    }

    SECTION("when close together replacement points and values it should format correctly")
    {
        Logging::VariantContainer vc[] {1, "__!", 17.3, 1.2f};
        String fmt = "{}This {}{} too short!{}";
        Logging::VariantFormat(fmt, vc, sizeof(vc) / sizeof(Logging::VariantContainer));
        REQUIRE(fmt == "1This __!17.30000000000000071 too short!1.200000");
    }

    SECTION("when values contain replacement points it should still format correctly")
    {
        Logging::VariantContainer vc[] {"{}"};
        String fmt = "Simple {} format!";
        Logging::VariantFormat(fmt, vc, sizeof(vc) / sizeof(Logging::VariantContainer));
        REQUIRE(fmt == "Simple {} format!");
    }

    SECTION("when given too many points for values it should leave the rest of the message alone")
    {
        Logging::VariantContainer vc[] {"foo"};
        String fmt = "This {} is {} too many!";
        Logging::VariantFormat(fmt, vc, sizeof(vc) / sizeof(Logging::VariantContainer));
        REQUIRE(fmt == "This foo is {} too many!");
    }

    SECTION("when given an empty string to format it should do nothing")
    {
        Logging::VariantContainer vc[] {"foo", 23};
        String fmt;
        Logging::VariantFormat(fmt, vc, sizeof(vc) / sizeof(Logging::VariantContainer));
        REQUIRE(fmt.IsEmpty());
    }

    SECTION("when given an empty replacement value string to format it should format correctly")
    {
        Logging::VariantContainer vc[] {"", "..."};
        String fmt = "Almost t{}oo quiet{}";
        Logging::VariantFormat(fmt, vc, sizeof(vc) / sizeof(Logging::VariantContainer));
        REQUIRE(fmt == "Almost too quiet...");
    }
}