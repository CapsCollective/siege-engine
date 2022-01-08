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
            REQUIRE(s.Size() == 0);
            REQUIRE(s == "");
            REQUIRE("" == s);
            REQUIRE(std::strcmp(s.AsChar(), "") == 0);
        }

        SECTION("assigning via assignment operator should apply")
        {
            String h("hello");
            s = h;
            REQUIRE(!s.IsEmpty());
            REQUIRE(s.Size() == 5);
            REQUIRE(s == h);
            REQUIRE(h == s);
            REQUIRE(std::strcmp(s.AsChar(), h.AsChar()) == 0);
        }

        SECTION("assigning a c-string via assignment operator should apply")
        {
            s = "hello";
            REQUIRE(!s.IsEmpty());
            REQUIRE(s.Size() == 5);
            REQUIRE(s == "hello");
            REQUIRE("hello" == s);
            REQUIRE(std::strcmp(s.AsChar(), "hello") == 0);
        }

        SECTION("the string can be cleared")
        {
            s.Clear();
            REQUIRE(!s);
            REQUIRE(s.Size() == 0);
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
            REQUIRE(s2);
            REQUIRE(s2 == "hello");
            REQUIRE("hello" == s2);
            REQUIRE(std::strcmp(s2.AsChar(), "hello") == 0);
        }

        SECTION("the string can be reassigned")
        {
            s2 = "goodbye";
            REQUIRE(s2);
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
            REQUIRE(s);
            REQUIRE(s == "hello");
            REQUIRE("hello" == s);
            REQUIRE(std::strcmp(s.AsChar(), "hello") == 0);
        }

        SECTION("the string can be reassigned")
        {
            s = "goodbye";
            REQUIRE(s);
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
            REQUIRE(s2.Size() == 19);
        }

        SECTION("the string should correctly add its own type")
        {
            String h("hello");
            String g("goodbye");
            String s2 = s1 + h;
            REQUIRE(s2 == "hello");
            s2 = s2 + g;
            REQUIRE(s2 == "hellogoodbye");
            s2 = g + s2;
            REQUIRE(s2 == "goodbyehellogoodbye");
            REQUIRE(s2.Size() == 19);
        }

        SECTION("the string should correctly append c-strings")
        {
            s1 += "hello";
            REQUIRE(s1 == "hello");
            String s2 = "goodbye" + s1;
            REQUIRE(s2 == "goodbyehello");
            s2.Append("goodbye");
            REQUIRE(s2 == "goodbyehellogoodbye");
            REQUIRE(s2.Size() == 19);
        }

        SECTION("the string should correctly append its own type")
        {
            String h("hello");
            String g("goodbye");
            s1 += h;
            REQUIRE(s1 == h);
            String s2 = g;
            s2.Append(s1);
            REQUIRE(s2 == "goodbyehello");
            s2.Append("goodbye");
            REQUIRE(s2 == "goodbyehellogoodbye");
            REQUIRE(s1 == "hello");
            REQUIRE(s1 != "goodbyehello");
            REQUIRE(s2.Size() == 19);
        }

        SECTION("the string should index its characters correctly")
        {
            s1 = "abcdefg";
            REQUIRE(s1.At(-1) == '\0');
            REQUIRE(s1.At(0) == 'a');
            REQUIRE(s1.At(2) == 'c');
            REQUIRE(s1.At(6) == 'g');
            REQUIRE(s1.At(7) == '\0');
            REQUIRE(s1[0] == 'a');
            REQUIRE(s1[3] == 'd');
            REQUIRE(s1[6] == 'g');
            s1[1] = 'z';
            REQUIRE(s1.At(1) == 'z');

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

TEST_CASE("strings can perform search and replace operations", "[String]")
{
    String s("ruSt Is a MuCH SafEr lANgUagE...");
    REQUIRE(s.Size() == 32);
    String s1("lANgUagE");
    String s2("...");
    String s3("rUBy");

    SECTION("the string can be searched for substrings")
    {
        REQUIRE(s.Find("ruSt") == 0);
        REQUIRE(s.Find("a MuCH SafEr") == 8);
        REQUIRE(s.Find(s1) == 21);
        REQUIRE(s.Find(s2) == 29);
        REQUIRE(s.Find(s3) == -1);
    }

    SECTION("the string can replace by substring")
    {
        REQUIRE(s.Replace("ruSt", s3));
        REQUIRE(s == "rUBy Is a MuCH SafEr lANgUagE...");
        REQUIRE(s.Find(s3) == 0);
        REQUIRE(!s.Replace("ruSt", "NiCEr"));
        REQUIRE(s.Replace("SafEr", "NiCEr"));
        REQUIRE(s == "rUBy Is a MuCH NiCEr lANgUagE...");
        REQUIRE(s.Find("NiCEr") == 15);
        REQUIRE(s.Replace(" a MuCH NiCEr lANgUagE", ""));
        REQUIRE(s == "rUBy Is...");
        REQUIRE(s.Replace("...", " pyThoN wItH exTra sTepS"));
        REQUIRE(s == "rUBy Is pyThoN wItH exTra sTepS");
    }
}