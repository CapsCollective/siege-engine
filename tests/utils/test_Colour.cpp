//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include <utest.h>
#include <utils/Colour.h>

using Siege::FColour;
using Siege::IColour;

UTEST(test_FColour, CreateFColour)
{
    FColour colour = {255.f, 1.f, 0.4f};

    ASSERT_EQ(colour.r, 255.f);
    ASSERT_EQ(colour.g, 1.f);
    ASSERT_EQ(colour.b, 0.4f);
    ASSERT_EQ(colour.a, 0.f);
}

UTEST(test_FColour, CreateConstexprFColour)
{
    constexpr FColour colour = {255.f, 1.f, 0.4f, 0.f};

    ASSERT_EQ(colour.r, 255.f);
    ASSERT_EQ(colour.g, 1.f);
    ASSERT_EQ(colour.b, 0.4f);
    ASSERT_EQ(colour.a, 0.f);
}

UTEST(test_FColour, CreateFColourFromIColour)
{
    constexpr IColour colour = {255, 1, 26, 0};

    FColour fColour = colour;

    ASSERT_EQ(fColour.r, 255.f);
    ASSERT_EQ(fColour.g, 1.f);
    ASSERT_EQ(fColour.b, 26.f);
    ASSERT_EQ(fColour.a, 0.f);
}

UTEST(test_FColour, CreateIColourFromFColour)
{
    constexpr FColour colour = {255.f, 1.f, 26.f, 0.f};

    IColour fColour = colour;

    ASSERT_EQ(fColour.r, 255);
    ASSERT_EQ(fColour.g, 1);
    ASSERT_EQ(fColour.b, 26);
    ASSERT_EQ(fColour.a, 0);
}