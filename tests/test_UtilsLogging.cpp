#include "catch.hpp"
#include "../src/utils/Logging.h"

TEST_CASE("messages can be formatted correctly", "[Logging]")
{
    SECTION("when given the correct number of replacement points and values it should format correctly")
    {
        Logging::VariantContainer vc[] {1, "$$", 17.3, 1.2f, raylib::Vector3::Zero()};
        std::string fmt = "This {} is {} very {} well {} formatted {}!";
        Logging::VariantFormat(fmt, vc, sizeof(vc)/sizeof(Logging::VariantContainer));
        REQUIRE(fmt == "This 1 is $$ very 17.300000 well 1.200000 formatted Vector3(0.000000,0.000000,0.000000)!");
    }

    SECTION("when given too few replacement points for values it should only use those it can fit")
    {
        Logging::VariantContainer vc[] {"hello", 17.3, "$$"};
        std::string fmt = "This {} is {} too short!";
        Logging::VariantFormat(fmt, vc, sizeof(vc)/sizeof(Logging::VariantContainer));
        REQUIRE(fmt == "This hello is 17.300000 too short!");
    }

    SECTION("when close together replacement points and values it should format correctly")
    {
        Logging::VariantContainer vc[] {1, "__!", 17.3, 1.2f};
        std::string fmt = "{}This {}{} too short!{}";
        Logging::VariantFormat(fmt, vc, sizeof(vc)/sizeof(Logging::VariantContainer));
        REQUIRE(fmt == "1This __!17.300000 too short!1.200000");
    }

    SECTION("when values contain replacement points it should still format correctly")
    {
        Logging::VariantContainer vc[] {"{}"};
        std::string fmt = "Simple {} format!";
        Logging::VariantFormat(fmt, vc, sizeof(vc)/sizeof(Logging::VariantContainer));
        REQUIRE(fmt == "Simple {} format!");
    }

    SECTION("when given too many replacement points for values it should leave the rest of the message alone")
    {
        Logging::VariantContainer vc[] {"foo"};
        std::string fmt = "This {} is {} too many!";
        Logging::VariantFormat(fmt, vc, sizeof(vc)/sizeof(Logging::VariantContainer));
        REQUIRE(fmt == "This foo is {} too many!");
    }

    SECTION("when given an empty string to format it should do nothing")
    {
        Logging::VariantContainer vc[] {"foo", 23};
        std::string fmt;
        Logging::VariantFormat(fmt, vc, sizeof(vc)/sizeof(Logging::VariantContainer));
        REQUIRE(fmt.empty());
    }

    SECTION("when given an empty replacement value string to format it should format correctly")
    {
        Logging::VariantContainer vc[] {"", "..."};
        std::string fmt = "Almost t{}oo quiet{}";
        Logging::VariantFormat(fmt, vc, sizeof(vc)/sizeof(Logging::VariantContainer));
        REQUIRE(fmt == "Almost too quiet...");
    }
}