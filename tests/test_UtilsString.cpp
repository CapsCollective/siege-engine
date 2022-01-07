#include "catch.hpp"
#include <utils/String.h>

TEST_CASE("strings can perform basic storage operations", "[String]")
{
    SECTION("when default constructing")
    {
        String s;

        SECTION("the string should be initialised correctly")
        {
            REQUIRE(s.IsEmpty());
            REQUIRE(s == "");
            REQUIRE("" == s);
            REQUIRE(std::strcmp(s.AsChar(), "") == 0);
        }

        SECTION("assigning a value via assignment operator should apply")
        {
            s = "hello";
            REQUIRE(!s.IsEmpty());
            REQUIRE(s == "hello");
            REQUIRE("hello" == s);
            REQUIRE(std::strcmp(s.AsChar(), "hello") == 0);
        }

        SECTION("the string can be cleared")
        {
            s.Clear();
            REQUIRE(s);
            REQUIRE(s == "");
            REQUIRE("" == s);
            REQUIRE(std::strcmp(s.AsChar(), "") == 0);
        }
    }

    SECTION("when copy constructing")
    {
        String s1("hello");
        String s2(s1);

        SECTION("the string should be initialised correctly")
        {
            REQUIRE(!s2);
            REQUIRE(s2 == "hello");
            REQUIRE("hello" == s2);
            REQUIRE(std::strcmp(s2.AsChar(), "hello") == 0);
        }

        SECTION("the string can be reassigned")
        {
            s2 = "goodbye";
            REQUIRE(!s2);
            REQUIRE(s2 == "goodbye");
            REQUIRE("goodbye" == s2);
            REQUIRE(std::strcmp(s2.AsChar(), "goodbye") == 0);
        }
    }

    SECTION("when copy constructing from a c-string")
    {
        String s("hello");

        SECTION("the string should be initialised correctly")
        {
            REQUIRE(!s);
            REQUIRE(s == "hello");
            REQUIRE("hello" == s);
            REQUIRE(std::strcmp(s.AsChar(), "hello") == 0);
        }

        SECTION("the string can be reassigned")
        {
            s = "goodbye";
            REQUIRE(!s);
            REQUIRE(s == "goodbye");
            REQUIRE("goodbye" == s);
            REQUIRE(std::strcmp(s.AsChar(), "goodbye") == 0);
        }
    }
}

TEST_CASE("strings can perform basic manipulation operations", "[String]")
{
    SECTION("when default constructing")
    {
        String s1;

        SECTION("the string should correctly add c-strings")
        {
            String s2 = s1 + "hello";
            REQUIRE(s2 == "hello");
            s2 = s2 + "goodbye";
            REQUIRE(s2 == "hellogoodbye");
            s2 = "goodbye" + s2;
            REQUIRE(s2 == "goodbyehellogoodbye");
        }

        SECTION("the string should correctly append c-strings")
        {
            s1 += "hello";
            REQUIRE(s1 == "hello");
            String s2 = "goodbye" += s1;
            REQUIRE(s2 == "goodbyehello");
        }

        SECTION("the string should cast correctly to c-strings")
        {
            s1 = "hellogoodbye";
            const char* cstr = s1;
            REQUIRE(std::strcmp(cstr, "hellogoodbye") == 0);
        }

        SECTION("the string should cast correctly to standard strings")
        {
            s1 = "hellogoodbye";
            std::string str = s1;
            REQUIRE(str == "hellogoodbye");
        }
    }
}