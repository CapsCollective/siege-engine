//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <utest.h>
#include <utils/math/Float.h>

UTEST(test_Float, Epsilon)
{
    ASSERT_EQ(std::numeric_limits<float>::epsilon(), Siege::Float::Epsilon());
}

UTEST(test_Float, Cos)
{
    ASSERT_EQ(0.98480775f, Siege::Float::Cos(Siege::Float::Radians(10.f)));
}

UTEST(test_Float, Sin)
{
    ASSERT_EQ(0.17364818f, Siege::Float::Sin(Siege::Float::Radians(10.f)));
}

UTEST(test_Float, Tan)
{
    ASSERT_EQ(0.17632698f, Siege::Float::Tan(Siege::Float::Radians(10.f)));
}

UTEST(test_Float, DegreestoRadians)
{
    ASSERT_TRUE((1.570796f - Siege::Float::Radians(90.f)) < Siege::Float::Epsilon());
}

UTEST(test_Float, RadiansToDegrees)
{
    ASSERT_TRUE((Siege::Float::Degrees(1.570796f) - 90.f) < Siege::Float::Epsilon());
}

UTEST(test_Float, Sign)
{
    ASSERT_EQ(-1, Siege::Float::Sign(-4.f));
    ASSERT_EQ(0, Siege::Float::Sign(0.f));
    ASSERT_EQ(1, Siege::Float::Sign(4.f));
}

UTEST(test_Float, Normalise)
{
    ASSERT_EQ(0.f, Siege::Float::Normalise(50.f, 0.f, 100.f));
    ASSERT_EQ(0.f, Siege::Float::Normalise(.5f));
}

UTEST(test_Float, Lerp)
{
    ASSERT_EQ(0.5f, Siege::Float::Lerp(0.f, 1.f, .5f));
}

UTEST(test_Float, Abs)
{
    ASSERT_EQ(1.f, Siege::Float::Abs(1.f));
    ASSERT_EQ(0.f, Siege::Float::Abs(0.f));
    ASSERT_EQ(1.f, Siege::Float::Abs(-1.f));
}

UTEST(test_Float, Max)
{
    ASSERT_EQ(5.f, Siege::Float::Max(1.f, 5.f));
    ASSERT_NE(1.f, Siege::Float::Max(1.f, 5.f));
}

UTEST(test_Float, Min)
{
    ASSERT_EQ(1.f, Siege::Float::Min(1.f, 5.f));
    ASSERT_NE(5.f, Siege::Float::Min(1.f, 5.f));
}

UTEST(test_Float, Clamp)
{
    ASSERT_EQ(10.f, Siege::Float::Clamp(20.f, 0.f, 10.f));
}
