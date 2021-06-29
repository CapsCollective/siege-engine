#include "catch.hpp"
#include "../src/utils/StringHelpers.h"

TEST_CASE("vectors can be converted to strings", "[StringHelpers]")
{
    SECTION("when a vector is provided to the function it should return a string with three decimal values")
    {
        std::string result = StringHelpers::VectorToString(raylib::Vector3::Zero());

        REQUIRE(result == "0.000000,0.000000,0.000000");
    }
}

TEST_CASE("strings can be converted into vectors", "[StringHelpers]")
{
    SECTION("when a string is provided to the function")
    {
        raylib::Vector3 result = StringHelpers::StringToVector("1.000000,5.000000,0.000000");

        SECTION("it should result in a raylib vector with the same values")
        {
            REQUIRE(result.x == 1.);
            REQUIRE(result.y == 5.);
            REQUIRE(result.z == 0.);
        }
    }

    SECTION("when a string with floats containing values with decimals is provided to the function")
    {
        raylib::Vector3 result = StringHelpers::StringToVector("1.54,3.27,0.65");

        SECTION("it should return a raylib vector with the same float values")
        {
            REQUIRE(result.x == 1.54f);
            REQUIRE(result.y == 3.27f);
            REQUIRE(result.z == 0.65f);
        }
    }

    SECTION("when a string with spaces between values is passed in into the function")
    {
        raylib::Vector3 result = StringHelpers::StringToVector("0.0, 3.27, 5.67");

        SECTION("it should return a raylib vector with the same float values")
        {
            REQUIRE(result.x == 0.0f);
            REQUIRE(result.y == 3.27f);
            REQUIRE(result.z == 5.67f);
        }
    }

    SECTION("when a string with non numeric characters is passed in it should throw an invalid_argument exception")
    {
        REQUIRE_THROWS_AS(StringHelpers::StringToVector("blah,3.27,BREAK"), std::invalid_argument);
    }

    SECTION("when a string with only two values is passed in it should throw a length_error")
    {
        REQUIRE_THROWS_AS(StringHelpers::StringToVector("0.0,3.27"), std::length_error);
    }

    SECTION("when a string with four values is passed in it should throw a length_error")
    {
        REQUIRE_THROWS_AS(StringHelpers::StringToVector("0.0,3.27,5.67,10.0"), std::length_error);
    }
}

TEST_CASE("strings can be split by a delimiter", "[StringHelpers]")
{
    SECTION("when a string with multiple values with delimiters are passed in")
    {
        std::vector<std::string> results = StringHelpers::SplitString("hello:goodbye:blah", ':');

        SECTION("it should return a vector with three strings")
        {
            REQUIRE(!results.empty());
            REQUIRE(results.size() == 3);

            SECTION("it should contain substrings equal to the values in the original string")
            {
                REQUIRE(results[0] == "hello");
                REQUIRE(results[1] == "goodbye");
                REQUIRE(results[2] == "blah");
            }
        }
    }

    SECTION("when a string with a delimiter at the front is passed into the function")
    {
        std::string str = "hello+goodbye+blah";
        SECTION("it should add an empty string to the front if preceded by a delimiter")
        {
            std::vector<std::string> results = StringHelpers::SplitString('+' + str, '+');
            REQUIRE(results.size() == 4);
            REQUIRE(results[0].empty());
        }

        SECTION("it should append an empty string to the end if a delimiter is at the end")
        {
            std::vector<std::string> results = StringHelpers::SplitString( str + '+', '+');
            REQUIRE(results.size() == 4);
            REQUIRE(results[3].empty());
        }

        SECTION("it should return an empty vector if an empty string is passed in")
        {
            std::vector<std::string> results = StringHelpers::SplitString( "", '+');
            REQUIRE(results.empty());
        }

        SECTION("it should return the original string if an empty delimiter is passed in")
        {
            std::vector<std::string> results = StringHelpers::SplitString( "blah&bleh", '\0');
            REQUIRE(results.size() == 1);
            REQUIRE(results[0] == "blah&bleh");
        }
    }
}

TEST_CASE("strings can be converted to lowercase", "[StringHelpers]")
{
    SECTION("when a string with uppercase characters is passed in it should return the same string in lower case")
    {
        std::string lowercase = StringHelpers::LowercaseString("THIS IS AN UPPERCASE STRING");
        REQUIRE(lowercase == "this is an uppercase string");
    }

    SECTION("when an empty string is passed in")
    {
        std::string lowercase = StringHelpers::LowercaseString("");
        REQUIRE(lowercase.empty());
    }

    SECTION("when a string without any alpha characters is passed in")
    {
        std::string lowercase = StringHelpers::LowercaseString("34143523322421@#@#!@##%");
        REQUIRE(lowercase == "34143523322421@#@#!@##%");
    }
}

TEST_CASE("strings can have subsections replaced by other strings", "[StringHelpers]")
{
    SECTION("when a string is passed into the function with a substring")
    {
        std::string original = "I LOVE Mondays!";
        std::string replacement = "HATE";

        SECTION("it should return a new string with the target replaced if the target is valid")
        {
            std::string newStr = StringHelpers::Replace(original, "LOVE", replacement);
            REQUIRE(newStr == "I HATE Mondays!");
        }

        SECTION("it should return the original string if the target is invalid")
        {
            std::string newStr = StringHelpers::Replace(original, "cake", replacement);
            REQUIRE(newStr == "I LOVE Mondays!");
        }

        SECTION("it should replace the first target with the replacement value")
        {
            std::string newStr = StringHelpers::Replace(original, " ", replacement);
            REQUIRE(newStr == "IHATELOVE Mondays!");
        }

        SECTION("it should return the original value if an empty string is passed in")
        {
            std::string newStr = StringHelpers::Replace(original, "", replacement);
            REQUIRE(newStr == "I LOVE Mondays!");
        }

        SECTION("it should remove the target with an empty string if replacement string is empty")
        {
            std::string newStr = StringHelpers::Replace(original, "LOVE", "");
            REQUIRE(newStr == "I  Mondays!");
        }

        SECTION("it should return an empty string if an empty string is inputted")
        {
            std::string newStr = StringHelpers::Replace("", "", "NOT EMPTY");
            REQUIRE(newStr.empty());
        }
    }
}