//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <time.h>
#include <utest.h>
#include <utils/math/Maths.h>
#include <utils/math/vec/Equality.h>
#include <utils/math/vec/Format.h>
#include <utils/math/vec/Vec2.h>
#include <utils/math/vec/Vec3.h>
#include <utils/math/vec/Vec4.h>

#define ASSERT_EQ_VEC2(a, b)                                               \
    ASSERT_LE(std::abs(a.x - b.x), std::numeric_limits<float>::epsilon()); \
    ASSERT_LE(std::abs(a.y - b.y), std::numeric_limits<float>::epsilon());

using namespace Siege;

UTEST(test_Vec2, CreateEmptyVec2)
{
    Vec2 vec {};

    ASSERT_EQ(0, vec.x);
    ASSERT_EQ(0, vec.y);
}

UTEST(test_Vec2, CreateVec2WithValues)
{
    Vec2 vec {1, 2};

    ASSERT_EQ(1, vec.x);
    ASSERT_EQ(2, vec.y);
}

UTEST(test_Vec2, CreateVec2One)
{
    Vec2 vec = Vec2::One();

    ASSERT_EQ(1, vec.x);
    ASSERT_EQ(1, vec.y);
}

UTEST(test_Vec2, CreateVec2UpVector)
{
    Vec2 vec = Vec2::Up();

    ASSERT_EQ(0, vec.x);
    ASSERT_EQ(-1, vec.y);
}

UTEST(test_Vec2, CreateVec2RightVector)
{
    Vec2 vec = Vec2::Right();

    ASSERT_EQ(1, vec.x);
    ASSERT_EQ(0, vec.y);
}

UTEST(test_Vec2, CreateVec2WithVec3)
{
    Vec2 expected = {1.f, 2.f};

    Vec3 vec = {1.f, 2.f, 3.f};

    Vec2 result = vec;

    ASSERT_EQ_VEC2(result, expected);
}

UTEST(test_Vec2, CreateVec2WithVec4)
{
    Vec2 expected = {1.f, 2.f};

    Vec4 vec = {1.f, 2.f, 3.f, 4.f};

    Vec2 result = vec;

    ASSERT_EQ_VEC2(result, expected);
}

UTEST(test_Vec2, SubscriptOperator)
{
    Vec2 vec = {1.f, 2.f};

    ASSERT_EQ(1.f, vec[0]);
    ASSERT_EQ(2.f, vec[1]);
}

UTEST(test_Vec2, TestSubscriptAssignmentOperator)
{
    Vec2 vec {};

    vec[0] = 2.f;
    vec[1] = 1.f;

    ASSERT_EQ(2.f, vec[0]);
    ASSERT_EQ(1.f, vec[1]);
}

UTEST(test_Vec2, TestAddVectors)
{
    Vec2 expected = {4.f, 2.f};

    Vec2 vecA = {2.f, 1.f};

    Vec2 result = vecA + vecA;

    ASSERT_EQ_VEC2(result, expected);
}

UTEST(test_Vec2, TestAddScalar)
{
    Vec2 expected = {4.f, 2.f};

    Vec2 vecA = {2.f, 0.f};

    Vec2 result = vecA + 2.f;

    ASSERT_EQ_VEC2(result, expected);
}

UTEST(test_Vec2, TestSubtractVectors)
{
    Vec2 expected = {4.f, 2.f};

    Vec2 vecA = {8.f, 4.f};

    Vec2 vecB = {4.f, 2.f};

    Vec2 result = vecA - vecB;

    ASSERT_EQ_VEC2(result, expected);
}

UTEST(test_Vec2, TestSubtractScalar)
{
    Vec2 expected = {4.f, 2.f};

    Vec2 vecA = {6.f, 4.f};

    Vec2 result = vecA - 2.f;

    ASSERT_EQ_VEC2(result, expected);
}

UTEST(test_Vec2, TestMultiplyVectors)
{
    Vec2 expected = {4.f, 2.f};

    Vec2 vecA = {2.f, 1.f};

    Vec2 vecB = {2.f, 2.f};

    Vec2 result = vecA * vecB;

    ASSERT_EQ_VEC2(result, expected);
}

UTEST(test_Vec2, TestMultiplyScalar)
{
    Vec2 expected = {4.f, 2.f};

    Vec2 vecA = {2.f, 1.f};

    Vec2 result = vecA * 2.f;

    ASSERT_EQ_VEC2(result, expected);
}

UTEST(test_Vec2, TestDivideVectors)
{
    Vec2 expected = {4.f, 2.f};

    Vec2 vecA = {8.f, 6.f};

    Vec2 vecB = {2.f, 3.f};

    Vec2 result = vecA / vecB;

    ASSERT_EQ_VEC2(result, expected);
}

UTEST(test_Vec2, TestDivideScalar)
{
    Vec2 expected = {4.f, 2.f};

    Vec2 vecA = {8.f, 4.f};

    Vec2 result = vecA / 2.f;

    ASSERT_EQ_VEC2(result, expected);
}

UTEST(test_Vec2, TestGetVectorLength)
{
    Vec2 vec = {2, 3};

    float length = Vec2::Length(vec);

    ASSERT_EQ(3.60555124f, length);
}

UTEST(test_Vec2, TestGetNormalisedVector)
{
    Vec2 vec = {2, 3};

    Vec2 norm = Vec2::Normalise(vec);

    ASSERT_LE((norm.x - 0.554775417f), std::numeric_limits<float>::epsilon());
    ASSERT_LE((norm.y - 0.832163095f), std::numeric_limits<float>::epsilon());
}

UTEST(test_Vec2, TestGetDotProduct)
{
    Vec2 vecA = {2, 3};
    Vec2 vecB = {5, 6};

    auto dot = Vec2::Dot(vecA, vecB);

    ASSERT_EQ(dot, 28.f);
}

UTEST(test_Vec2, TestVec3CosOperation)
{
    Vec2 expected = {0.540302336f, -0.416146845};

    Vec2 vec = {1.f, 2.f};

    auto result = Vec2::Cos(vec);

    ASSERT_EQ_VEC2(result, expected);
}

UTEST(test_Vec2, TestVec3SinOperation)
{
    Vec2 expected = {0.841471016f, 0.909297406};

    Vec2 vec = {1.f, 2.f};

    auto result = Vec2::Sin(vec);

    ASSERT_EQ_VEC2(result, expected);
}

UTEST(test_Vec2, TestVec3TanOperation)
{
    Vec2 expected = {1.55740774f, -2.18503976};

    Vec2 vec = {1.f, 2.f};

    auto result = Vec2::Tan(vec);

    ASSERT_EQ_VEC2(result, expected);
}

UTEST(test_Vec2, TestSwizzleOperators)
{
    srand(time(NULL));
    float x = rand() % 1000;
    float y = rand() % 1000;
    Vec2 vec = {x, y};
    auto vecXX = vec.XX();
    auto vecXY = vec.XY();
    auto vecYX = vec.YX();
    auto vecYY = vec.YY();

    ASSERT_EQ(x, vecXX.x);
    ASSERT_EQ(x, vecXX.y);
    ASSERT_EQ(x, vecXY.x);
    ASSERT_EQ(y, vecXY.y);
    ASSERT_EQ(y, vecYX.x);
    ASSERT_EQ(x, vecYX.y);
    ASSERT_EQ(y, vecYY.x);
    ASSERT_EQ(y, vecYY.y);

    auto vecXXX = vec.XXX();
    auto vecXXY = vec.XXY();
    auto vecXYX = vec.XYX();
    auto vecXYY = vec.XYY();
    auto vecYXX = vec.YXX();
    auto vecYXY = vec.YXY();
    auto vecYYX = vec.YYX();
    auto vecYYY = vec.YYY();

    ASSERT_EQ(x, vecXXX.x);
    ASSERT_EQ(x, vecXXX.y);
    ASSERT_EQ(x, vecXXX.z);
    ASSERT_EQ(x, vecXXY.x);
    ASSERT_EQ(x, vecXXY.y);
    ASSERT_EQ(y, vecXXY.z);
    ASSERT_EQ(x, vecXYX.x);
    ASSERT_EQ(y, vecXYX.y);
    ASSERT_EQ(x, vecXYX.z);
    ASSERT_EQ(x, vecXYY.x);
    ASSERT_EQ(y, vecXYY.y);
    ASSERT_EQ(y, vecXYY.z);
    ASSERT_EQ(y, vecYXX.x);
    ASSERT_EQ(x, vecYXX.y);
    ASSERT_EQ(x, vecYXX.z);
    ASSERT_EQ(y, vecYXY.x);
    ASSERT_EQ(x, vecYXY.y);
    ASSERT_EQ(y, vecYXY.z);
    ASSERT_EQ(y, vecYYX.x);
    ASSERT_EQ(y, vecYYX.y);
    ASSERT_EQ(x, vecYYX.z);
    ASSERT_EQ(y, vecYYY.x);
    ASSERT_EQ(y, vecYYY.y);
    ASSERT_EQ(y, vecYYY.z);

    auto vecXXXX = vec.XXXX();
    auto vecXXXY = vec.XXXY();
    auto vecXXYX = vec.XXYX();
    auto vecXXYY = vec.XXYY();
    auto vecXYXX = vec.XYXX();
    auto vecXYXY = vec.XYXY();
    auto vecXYYX = vec.XYYX();
    auto vecXYYY = vec.XYYY();
    auto vecYXXX = vec.YXXX();
    auto vecYXXY = vec.YXXY();
    auto vecYXYX = vec.YXYX();
    auto vecYXYY = vec.YXYY();
    auto vecYYXX = vec.YYXX();
    auto vecYYXY = vec.YYXY();
    auto vecYYYX = vec.YYYX();
    auto vecYYYY = vec.YYYY();

    ASSERT_EQ(x, vecXXXX.x);
    ASSERT_EQ(x, vecXXXX.y);
    ASSERT_EQ(x, vecXXXX.z);
    ASSERT_EQ(x, vecXXXX.w);
    ASSERT_EQ(x, vecXXXY.x);
    ASSERT_EQ(x, vecXXXY.y);
    ASSERT_EQ(x, vecXXXY.z);
    ASSERT_EQ(y, vecXXXY.w);
    ASSERT_EQ(x, vecXXYX.x);
    ASSERT_EQ(x, vecXXYX.y);
    ASSERT_EQ(y, vecXXYX.z);
    ASSERT_EQ(x, vecXXYX.w);
    ASSERT_EQ(x, vecXXYY.x);
    ASSERT_EQ(x, vecXXYY.y);
    ASSERT_EQ(y, vecXXYY.z);
    ASSERT_EQ(y, vecXXYY.w);
    ASSERT_EQ(x, vecXYXX.x);
    ASSERT_EQ(y, vecXYXX.y);
    ASSERT_EQ(x, vecXYXX.z);
    ASSERT_EQ(x, vecXYXX.w);
    ASSERT_EQ(x, vecXYXY.x);
    ASSERT_EQ(y, vecXYXY.y);
    ASSERT_EQ(x, vecXYXY.z);
    ASSERT_EQ(y, vecXYXY.w);
    ASSERT_EQ(x, vecXYYX.x);
    ASSERT_EQ(y, vecXYYX.y);
    ASSERT_EQ(y, vecXYYX.z);
    ASSERT_EQ(x, vecXYYX.w);
    ASSERT_EQ(x, vecXYYY.x);
    ASSERT_EQ(y, vecXYYY.y);
    ASSERT_EQ(y, vecXYYY.z);
    ASSERT_EQ(y, vecXYYY.w);
    ASSERT_EQ(y, vecYXXX.x);
    ASSERT_EQ(x, vecYXXX.y);
    ASSERT_EQ(x, vecYXXX.z);
    ASSERT_EQ(x, vecYXXX.w);
    ASSERT_EQ(y, vecYXXY.x);
    ASSERT_EQ(x, vecYXXY.y);
    ASSERT_EQ(x, vecYXXY.z);
    ASSERT_EQ(y, vecYXXY.w);
    ASSERT_EQ(y, vecYXYX.x);
    ASSERT_EQ(x, vecYXYX.y);
    ASSERT_EQ(y, vecYXYX.z);
    ASSERT_EQ(x, vecYXYX.w);
    ASSERT_EQ(y, vecYXYY.x);
    ASSERT_EQ(x, vecYXYY.y);
    ASSERT_EQ(y, vecYXYY.z);
    ASSERT_EQ(y, vecYXYY.w);
    ASSERT_EQ(y, vecYYXX.x);
    ASSERT_EQ(y, vecYYXX.y);
    ASSERT_EQ(x, vecYYXX.z);
    ASSERT_EQ(x, vecYYXX.w);
    ASSERT_EQ(y, vecYYXY.x);
    ASSERT_EQ(y, vecYYXY.y);
    ASSERT_EQ(x, vecYYXY.z);
    ASSERT_EQ(y, vecYYXY.w);
    ASSERT_EQ(y, vecYYYX.x);
    ASSERT_EQ(y, vecYYYX.y);
    ASSERT_EQ(y, vecYYYX.z);
    ASSERT_EQ(x, vecYYYX.w);
    ASSERT_EQ(y, vecYYYY.x);
    ASSERT_EQ(y, vecYYYY.y);
    ASSERT_EQ(y, vecYYYY.z);
    ASSERT_EQ(y, vecYYYY.w);
}

UTEST(test_Vec2, TestToString)
{
    String expected = "1.00,2.00";

    Vec2 vec = {1.f, 2.f};

    String result = ToString(vec);

    ASSERT_STREQ(result.Str(), expected.Str());
}

UTEST(test_Vec2, TestFromString)
{
    Vec2 expected = {1.f, 2.f};

    String inputString = "1.00,2.00";

    Vec2 result {};

    ASSERT_TRUE(FromString(result, inputString));

    ASSERT_EQ_VEC2(result, expected);
}

UTEST(test_Vec2, CheckVectorEquality)
{
    Vec2 vecA = {1.f, 2.f};

    Vec2 vecB = {0.9999999999999912, 2.0000000000000023f};

    ASSERT_TRUE(Equals(vecA, vecB, Epsilon<float>()));

    vecA = {-1.f, -2.f};

    vecB = {-0.9999999999999912, -2.0000000000000023f};

    ASSERT_TRUE(Equals(vecA, vecB, Epsilon<float>()));
}

UTEST(test_Vec2, LerpVector)
{
    Vec2 expected = {1.5f, 1.5f};
    Vec2 vec = Vec2::One();

    ASSERT_EQ_VEC2(expected, Vec2::Lerp(vec, {2.f, 2.f}, .5f));
}