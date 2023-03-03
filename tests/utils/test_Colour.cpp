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

    ASSERT_EQ(255.f, colour.r);
    ASSERT_EQ(1.f, colour.g);
    ASSERT_EQ(0.4f, colour.b);
    ASSERT_EQ(0.f, colour.a);
}

UTEST(test_FColour, CreateConstexprFColour)
{
    constexpr FColour colour = {255.f, 1.f, 0.4f, 0.f};

    ASSERT_EQ(255.f, colour.r);
    ASSERT_EQ(1.f, colour.g);
    ASSERT_EQ(0.4f, colour.b);
    ASSERT_EQ(0.f, colour.a);
}

UTEST(test_FColour, CreateFColourFromIColour)
{
    constexpr IColour colour = {255, 1, 26, 0};

    FColour fColour = colour;

    ASSERT_EQ(255.f, fColour.r);
    ASSERT_EQ(1.f, fColour.g);
    ASSERT_EQ(26.f, fColour.b);
    ASSERT_EQ(0.f, fColour.a);
}

UTEST(test_FColour, CreateIColourFromFColour)
{
    constexpr FColour colour = {255.f, 1.f, 26.f, 0.f};

    IColour fColour = colour;

    ASSERT_EQ(255, fColour.r);
    ASSERT_EQ(1, fColour.g);
    ASSERT_EQ(26, fColour.b);
    ASSERT_EQ(0, fColour.a);
}

UTEST(test_FColour, ConvertFColourToIColour)
{
    constexpr FColour colour = {1.f, 0.f, .2f, .5f};

    IColour iColour = Siege::ToIColour(colour);

    ASSERT_EQ(255, iColour.r);
    ASSERT_EQ(0, iColour.g);
    ASSERT_EQ(51, iColour.b);
    ASSERT_EQ(127, iColour.a);
}

UTEST(test_FColour, ConvertIColourToFColour)
{
    constexpr IColour colour = {255, 0, 51, 127};

    FColour fColour = Siege::ToFColour(colour);

    ASSERT_EQ(1.f, fColour.r);
    ASSERT_EQ(0.f, fColour.g);
    ASSERT_EQ(.2f, fColour.b);
    ASSERT_EQ(.498039216f, fColour.a);
}