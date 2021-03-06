//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <utest.h>

// Require logging
#define CC_ENABLE_LOGGING

#include <utils/Logging.h>

UTEST(test_Logging, CorrectReplacementPoints)
{
    // When given the correct number of points and values it should format correctly
    Logging::VariantContainer vc[] {1, "$$", 1.2f, Vec3::Zero};
    String fmt = "{} is {} very well {} formatted {}!";
    Logging::VariantFormat(fmt, vc, sizeof(vc) / sizeof(Logging::VariantContainer));
    ASSERT_STREQ("1 is $$ very well 1.200000 formatted Vector3(0.00,0.00,0.00)!", fmt.Str());
}

UTEST(test_Logging, TooFewReplacementPoints)
{
    // When given too few replacement points for values it should only use those it can fit
    Logging::VariantContainer vc[] {"hello", 17.3, "$$"};
    String fmt = "This {} is {} too short!";
    Logging::VariantFormat(fmt, vc, sizeof(vc) / sizeof(Logging::VariantContainer));
    ASSERT_STREQ("This hello is 17.30000000000000071 too short!", fmt.Str());
}

UTEST(test_Logging, CloseTogetherReplacementPoints)
{
    // When close together replacement points and values it should format correctly
    Logging::VariantContainer vc[] {1, "__!", 17.3, 1.2f};
    String fmt = "{}This {}{} too short!{}";
    Logging::VariantFormat(fmt, vc, sizeof(vc) / sizeof(Logging::VariantContainer));
    ASSERT_STREQ("1This __!17.30000000000000071 too short!1.200000", fmt.Str());
}

UTEST(test_Logging, ReplacementPointsInValues)
{
    // When values contain replacement points it should still format correctly
    Logging::VariantContainer vc[] {"{}"};
    String fmt = "Simple {} format!";
    Logging::VariantFormat(fmt, vc, sizeof(vc) / sizeof(Logging::VariantContainer));
    ASSERT_STREQ("Simple {} format!", fmt.Str());
}

UTEST(test_Logging, TooManyReplacementPoints)
{
    // When given too many points for values it should leave the rest of the message alone
    Logging::VariantContainer vc[] {"foo"};
    String fmt = "This {} is {} too many!";
    Logging::VariantFormat(fmt, vc, sizeof(vc) / sizeof(Logging::VariantContainer));
    ASSERT_STREQ("This foo is {} too many!", fmt.Str());
}

UTEST(test_Logging, EmptyFormatString)
{
    // When given an empty string to format it should do nothing
    Logging::VariantContainer vc[] {"foo", 23};
    String fmt;
    Logging::VariantFormat(fmt, vc, sizeof(vc) / sizeof(Logging::VariantContainer));
    ASSERT_TRUE(fmt.IsEmpty());
}

UTEST(test_Logging, EmptyReplacementValue)
{
    // When given an empty replacement value string to format it should format correctly
    Logging::VariantContainer vc[] {"", "..."};
    String fmt = "Almost t{}oo quiet{}";
    Logging::VariantFormat(fmt, vc, sizeof(vc) / sizeof(Logging::VariantContainer));
    ASSERT_STREQ("Almost too quiet...", fmt.Str());
}