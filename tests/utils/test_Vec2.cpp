//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "utest.h"
#include "utils/vec/Vec2.h"
#include "utils/vec/Vec3.h"
#include "utils/vec/Vec4.h"

UTEST(test_Vec2, CreateEmptyVec2)
{
    Siege::Vec2 vec = {};
    ASSERT_EQ(0.f, vec.x);
    ASSERT_EQ(0.f, vec.y);
}

UTEST(test_Vec2, CreateVec2WithValues)
{
    Siege::Vec2 vec = {1.f, 2.f};
    ASSERT_EQ(1.f, vec.x);
    ASSERT_EQ(2.f, vec.y);
}

UTEST(test_Vec2, CreateZeroVector)
{
    Siege::Vec2 vec = Siege::Vec2::Zero;
    ASSERT_EQ(0.f, vec.x);
    ASSERT_EQ(0.f, vec.y);
}

UTEST(test_Vec2, CreateOneVector)
{
    Siege::Vec2 vec = Siege::Vec2::One;
    ASSERT_EQ(1.f, vec.x);
    ASSERT_EQ(1.f, vec.y);
}

UTEST(test_Vec2, CreateUpVector)
{
    Siege::Vec2 vec = Siege::Vec2::Up;
    ASSERT_EQ(0.f, vec.x);
    ASSERT_EQ(1.f, vec.y);
}

UTEST(test_Vec2, CreateVec2FromString)
{
    Siege::Vec2 vec;

    Siege::Vec2::FromString(vec, "2,3");

    ASSERT_EQ(2.f, vec.x);
    ASSERT_EQ(3.f, vec.y);
}

UTEST(test_Vec2, CreateVec2FromVec3)
{
    Siege::Vec3 vec = {1.f, 2.f, 3.f};

    Siege::Vec2 result = vec;

    ASSERT_EQ(1.f, result.x);
    ASSERT_EQ(2.f, result.y);
}

UTEST(test_Vec2, CreateVec2FromVec4)
{
    Siege::Vec4 vec = {1.f, 2.f, 3.f, 4.f};

    Siege::Vec2 result = vec;

    ASSERT_EQ(1.f, result.x);
    ASSERT_EQ(2.f, result.y);
}

UTEST(test_Vec2, GetXComp)
{
    Siege::Vec2 vec = {10, 15};

    Siege::Vec2 xComp = vec.XComp();

    ASSERT_EQ(10.f, xComp.x);
    ASSERT_EQ(0.f, xComp.y);
}

UTEST(test_Vec2, GetYComp)
{
    Siege::Vec2 vec = {10, 15};

    Siege::Vec2 yComp = vec.YComp();

    ASSERT_EQ(0.f, yComp.x);
    ASSERT_EQ(15.f, yComp.y);
}

UTEST(test_Vec2, TestVectorEquality)
{
    Siege::Vec2 vec0 = Siege::Vec2::Zero;
    Siege::Vec2 vec1 = Siege::Vec2::Zero;
    Siege::Vec2 vec2 = Siege::Vec2::Up;

    ASSERT_TRUE(vec0 == vec1);
    ASSERT_TRUE(vec0 != vec2);
}

UTEST(test_Vec2, AddVectors)
{
    Siege::Vec2 vec0 = Siege::Vec2::Zero;
    Siege::Vec2 vec1 = Siege::Vec2::One;

    vec0 += vec1;

    ASSERT_EQ(1.f, vec0.x);
    ASSERT_EQ(1.f, vec0.y);

    Siege::Vec2 vec3 = vec0 + Siege::Vec2::One;

    ASSERT_EQ(2.f, vec3.x);
    ASSERT_EQ(2.f, vec3.y);
}

UTEST(test_Vec2, SubtractVectors)
{
    Siege::Vec2 vec0 = Siege::Vec2::One;
    Siege::Vec2 vec1 = Siege::Vec2::Zero;

    vec0 -= vec1;

    ASSERT_EQ(1.f, vec0.x);
    ASSERT_EQ(1.f, vec0.y);

    Siege::Vec2 vec3 = vec0 - Siege::Vec2::One;

    ASSERT_EQ(0.f, vec3.x);
    ASSERT_EQ(0.f, vec3.y);
}

UTEST(test_Vec2, MultiplyVectors)
{
    Siege::Vec2 vec0 = {3, 5};
    Siege::Vec2 vec1 = {2, 5};

    vec0 *= vec1;

    ASSERT_EQ(6.f, vec0.x);
    ASSERT_EQ(25.f, vec0.y);

    Siege::Vec2 vec3 = vec0 * Siege::Vec2::One;

    ASSERT_EQ(6.f, vec3.x);
    ASSERT_EQ(25.f, vec3.y);
}

UTEST(test_Vec2, DivideVectors)
{
    Siege::Vec2 vec0 = {10, 20};
    Siege::Vec2 vec1 = {2, 5};

    vec0 /= vec1;

    ASSERT_EQ(5.f, vec0.x);
    ASSERT_EQ(4.f, vec0.y);

    Siege::Vec2 vec3 = vec0 / Siege::Vec2::One;

    ASSERT_EQ(5.f, vec3.x);
    ASSERT_EQ(4.f, vec3.y);
}

UTEST(test_Vec2, AddVector3)
{
    Siege::Vec2 vec0 = Siege::Vec2::Zero;
    Siege::Vec3 vec1 = {5, 2, 1};

    vec0 += vec1;

    ASSERT_EQ(5.f, vec0.x);
    ASSERT_EQ(2.f, vec0.y);

    Siege::Vec2 vec3 = vec0 + Siege::Vec3::One;

    ASSERT_EQ(6.f, vec3.x);
    ASSERT_EQ(3.f, vec3.y);
}

UTEST(test_Vec2, SubtractVector3)
{
    Siege::Vec2 vec0 = Siege::Vec2::One;
    Siege::Vec3 vec1 = {1.f, 1.f, 1.f};

    vec0 -= vec1;

    ASSERT_EQ(0.f, vec0.x);
    ASSERT_EQ(0.f, vec0.y);

    Siege::Vec2 vec3 = vec0 - Siege::Vec3::One;

    ASSERT_EQ(-1.f, vec3.x);
    ASSERT_EQ(-1.f, vec3.y);
}

UTEST(test_Vec2, MultiplyVector3)
{
    Siege::Vec2 vec0 = {3, 5};
    Siege::Vec3 vec1 = {2, 5, 6};

    vec0 *= vec1;

    ASSERT_EQ(6.f, vec0.x);
    ASSERT_EQ(25.f, vec0.y);

    Siege::Vec2 vec3 = vec0 * Siege::Vec3::One;

    ASSERT_EQ(6.f, vec3.x);
    ASSERT_EQ(25.f, vec3.y);
}

UTEST(test_Vec2, DivideVector3)
{
    Siege::Vec2 vec0 = {10, 20};
    Siege::Vec3 vec1 = {2, 5, 10};

    vec0 /= vec1;

    ASSERT_EQ(5.f, vec0.x);
    ASSERT_EQ(4.f, vec0.y);

    Siege::Vec2 vec3 = vec0 / Siege::Vec3::One;

    ASSERT_EQ(5.f, vec3.x);
    ASSERT_EQ(4.f, vec3.y);
}

UTEST(test_Vec2, AddScalar)
{
    Siege::Vec2 vec0 = {5, 10};

    vec0 += 2.f;

    ASSERT_EQ(7.f, vec0.x);
    ASSERT_EQ(12.f, vec0.y);

    Siege::Vec2 vec3 = vec0 + 2.f;

    ASSERT_EQ(9.f, vec3.x);
    ASSERT_EQ(14.f, vec3.y);
}

UTEST(test_Vec2, SubtractScalar)
{
    Siege::Vec2 vec0 = {5, 10};

    vec0 -= 2.f;

    ASSERT_EQ(3.f, vec0.x);
    ASSERT_EQ(8.f, vec0.y);

    Siege::Vec2 vec3 = vec0 - 2.f;

    ASSERT_EQ(1.f, vec3.x);
    ASSERT_EQ(6.f, vec3.y);
}

UTEST(test_Vec2, MultiplyScalar)
{
    Siege::Vec2 vec0 = {5, 10};

    vec0 *= 2.f;

    ASSERT_EQ(10.f, vec0.x);
    ASSERT_EQ(20.f, vec0.y);

    Siege::Vec2 vec3 = vec0 * 2.f;

    ASSERT_EQ(20.f, vec3.x);
    ASSERT_EQ(40.f, vec3.y);
}

UTEST(test_Vec2, GetString)
{
    Siege::String expected = "5.00,10.00";

    Siege::Vec2 vec0 = {5, 10};
    Siege::String result = vec0.ToString();

    ASSERT_STREQ(expected.Str(), result.Str());
}

UTEST(test_Vec2, GetVectorLength)
{
    Siege::Vec2 vec0 = {2, 3};

    float length = vec0.Length();

    ASSERT_EQ(3.60555127546f, length);
}

UTEST(test_Vec2, NormaliseVector)
{
    Siege::Vec2 vec0 = {2, 3};

    Siege::Vec2 norm = vec0.Normalise();

    ASSERT_TRUE((norm.x - 0.554700196f) < std::numeric_limits<float>::epsilon());
    ASSERT_TRUE((norm.y - 0.832050323f) < std::numeric_limits<float>::epsilon());
}