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

using namespace Siege;

UTEST(test_Vec2, CreateEmptyVec2)
{
    Vec2 vec = {};
    ASSERT_EQ(0.f, vec.x);
    ASSERT_EQ(0.f, vec.y);
}

UTEST(test_Vec2, CreateVec2WithValues)
{
    Vec2 vec = {1.f, 2.f};
    ASSERT_EQ(1.f, vec.x);
    ASSERT_EQ(2.f, vec.y);
}

UTEST(test_Vec2, CreateZeroVector)
{
    Vec2 vec = Vec2::Zero;
    ASSERT_EQ(0.f, vec.x);
    ASSERT_EQ(0.f, vec.y);
}

UTEST(test_Vec2, CreateOneVector)
{
    Vec2 vec = Vec2::One;
    ASSERT_EQ(1.f, vec.x);
    ASSERT_EQ(1.f, vec.y);
}

UTEST(test_Vec2, CreateUpVector)
{
    Vec2 vec = Vec2::Up;
    ASSERT_EQ(0.f, vec.x);
    ASSERT_EQ(1.f, vec.y);
}

UTEST(test_Vec2, CreateVec2FromString)
{
    Vec2 vec;

    Vec2::FromString(vec, "2,3");

    ASSERT_EQ(2.f, vec.x);
    ASSERT_EQ(3.f, vec.y);
}

UTEST(test_Vec2, CreateVec2FromVec3)
{
    Vec3 vec = {1.f, 2.f, 3.f};

    Vec2 result = Vec2(vec);

    ASSERT_EQ(1.f, result.x);
    ASSERT_EQ(2.f, result.y);
}

UTEST(test_Vec2, CreateVec2FromVec4)
{
    Vec4 vec = {1.f, 2.f, 3.f, 4.f};

    Vec2 result = Vec2(vec);

    ASSERT_EQ(1.f, result.x);
    ASSERT_EQ(2.f, result.y);
}

UTEST(test_Vec2, GetXComp)
{
    Vec2 vec = {10, 15};

    Vec2 xComp = vec.XComp();

    ASSERT_EQ(10.f, xComp.x);
    ASSERT_EQ(0.f, xComp.y);
}

UTEST(test_Vec2, GetYComp)
{
    Vec2 vec = {10, 15};

    Vec2 yComp = vec.YComp();

    ASSERT_EQ(0.f, yComp.x);
    ASSERT_EQ(15.f, yComp.y);
}

UTEST(test_Vec2, TestVectorEquality)
{
    Vec2 vec0 = Vec2::Zero;
    Vec2 vec1 = Vec2::Zero;
    Vec2 vec2 = Vec2::Up;

    ASSERT_TRUE(vec0 == vec1);
    ASSERT_TRUE(vec0 != vec2);
}

UTEST(test_Vec2, AddVectors)
{
    Vec2 vec0 = Vec2::Zero;
    Vec2 vec1 = Vec2::One;

    vec0 += vec1;

    ASSERT_EQ(1.f, vec0.x);
    ASSERT_EQ(1.f, vec0.y);

    Vec2 vec3 = vec0 + Vec2::One;

    ASSERT_EQ(2.f, vec3.x);
    ASSERT_EQ(2.f, vec3.y);
}

UTEST(test_Vec2, SubtractVectors)
{
    Vec2 vec0 = Vec2::One;
    Vec2 vec1 = Vec2::Zero;

    vec0 -= vec1;

    ASSERT_EQ(1.f, vec0.x);
    ASSERT_EQ(1.f, vec0.y);

    Vec2 vec3 = vec0 - Vec2::One;

    ASSERT_EQ(0.f, vec3.x);
    ASSERT_EQ(0.f, vec3.y);
}

UTEST(test_Vec2, MultiplyVectors)
{
    Vec2 vec0 = {3, 5};
    Vec2 vec1 = {2, 5};

    vec0 *= vec1;

    ASSERT_EQ(6.f, vec0.x);
    ASSERT_EQ(25.f, vec0.y);

    Vec2 vec3 = vec0 * Vec2::One;

    ASSERT_EQ(6.f, vec3.x);
    ASSERT_EQ(25.f, vec3.y);
}

UTEST(test_Vec2, DivideVectors)
{
    Vec2 vec0 = {10, 20};
    Vec2 vec1 = {2, 5};

    vec0 /= vec1;

    ASSERT_EQ(5.f, vec0.x);
    ASSERT_EQ(4.f, vec0.y);

    Vec2 vec3 = vec0 / Vec2::One;

    ASSERT_EQ(5.f, vec3.x);
    ASSERT_EQ(4.f, vec3.y);
}

UTEST(test_Vec2, AddVector3)
{
    Vec2 vec0 = Vec2::Zero;
    Vec3 vec1 = {5, 2, 1};

    vec0 += vec1;

    ASSERT_EQ(5.f, vec0.x);
    ASSERT_EQ(2.f, vec0.y);

    Vec2 vec3 = vec0 + Vec3::One;

    ASSERT_EQ(6.f, vec3.x);
    ASSERT_EQ(3.f, vec3.y);
}

UTEST(test_Vec2, SubtractVector3)
{
    Vec2 vec0 = Vec2::One;
    Vec3 vec1 = {1.f, 1.f, 1.f};

    vec0 -= vec1;

    ASSERT_EQ(0.f, vec0.x);
    ASSERT_EQ(0.f, vec0.y);

    Vec2 vec3 = vec0 - Vec3::One;

    ASSERT_EQ(-1.f, vec3.x);
    ASSERT_EQ(-1.f, vec3.y);
}

UTEST(test_Vec2, MultiplyVector3)
{
    Vec2 vec0 = {3, 5};
    Vec3 vec1 = {2, 5, 6};

    vec0 *= vec1;

    ASSERT_EQ(6.f, vec0.x);
    ASSERT_EQ(25.f, vec0.y);

    Vec2 vec3 = vec0 * Vec3::One;

    ASSERT_EQ(6.f, vec3.x);
    ASSERT_EQ(25.f, vec3.y);
}

UTEST(test_Vec2, DivideVector3)
{
    Vec2 vec0 = {10, 20};
    Vec3 vec1 = {2, 5, 10};

    vec0 /= vec1;

    ASSERT_EQ(5.f, vec0.x);
    ASSERT_EQ(4.f, vec0.y);

    Vec2 vec3 = vec0 / Vec3::One;

    ASSERT_EQ(5.f, vec3.x);
    ASSERT_EQ(4.f, vec3.y);
}

UTEST(test_Vec2, AddScalar)
{
    Vec2 vec0 = {5, 10};

    vec0 += 2.f;

    ASSERT_EQ(7.f, vec0.x);
    ASSERT_EQ(12.f, vec0.y);

    Vec2 vec3 = vec0 + 2.f;

    ASSERT_EQ(9.f, vec3.x);
    ASSERT_EQ(14.f, vec3.y);
}

UTEST(test_Vec2, SubtractScalar)
{
    Vec2 vec0 = {5, 10};

    vec0 -= 2.f;

    ASSERT_EQ(3.f, vec0.x);
    ASSERT_EQ(8.f, vec0.y);

    Vec2 vec3 = vec0 - 2.f;

    ASSERT_EQ(1.f, vec3.x);
    ASSERT_EQ(6.f, vec3.y);
}

UTEST(test_Vec2, MultiplyScalar)
{
    Vec2 vec0 = {5, 10};

    vec0 *= 2.f;

    ASSERT_EQ(10.f, vec0.x);
    ASSERT_EQ(20.f, vec0.y);

    Vec2 vec3 = vec0 * 2.f;

    ASSERT_EQ(20.f, vec3.x);
    ASSERT_EQ(40.f, vec3.y);
}

UTEST(test_Vec2, GetString)
{
    String expected = "5.00,10.00";

    Vec2 vec0 = {5, 10};
    String result = vec0.ToString();

    ASSERT_STREQ(expected.Str(), result.Str());
}

UTEST(test_Vec2, GetVectorLength)
{
    Vec2 vec0 = {2, 3};

    float length = vec0.Length();

    ASSERT_EQ(3.60555127546f, length);
}

UTEST(test_Vec2, NormaliseVector)
{
    Vec2 vec0 = {2, 3};

    Vec2 norm = vec0.Normalise();

    ASSERT_TRUE((norm.x - 0.554700196f) < std::numeric_limits<float>::epsilon());
    ASSERT_TRUE((norm.y - 0.832050323f) < std::numeric_limits<float>::epsilon());
}

UTEST(test_Vec2, CalculateDotProduct)
{
    Vec2 vecA = {2, 3};
    Vec2 VecB = {5, 6};

    auto dot = vecA.Dot(VecB);

    ASSERT_EQ(dot, 28.f);
}