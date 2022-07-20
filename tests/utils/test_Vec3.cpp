//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include <cmath>
#include <limits>

#include <utest.h>
#include <utils/math/vec/Vec2.h>
#include <utils/math/vec/Vec3.h>
#include <utils/math/vec/Vec4.h>

using namespace Siege;

UTEST(test_Vec3, CreateEmptyVec2)
{
    Vec3 vec = {};
    ASSERT_EQ(0.f, vec.x);
    ASSERT_EQ(0.f, vec.y);
    ASSERT_EQ(0.f, vec.z);
}

UTEST(test_Vec3, CreateVec2WithValues)
{
    Vec3 vec = {1.f, 2.f, 3.f};
    ASSERT_EQ(1.f, vec.x);
    ASSERT_EQ(2.f, vec.y);
    ASSERT_EQ(3.f, vec.z);
}

UTEST(test_Vec3, CreateZeroVector)
{
    Vec3 vec = Vec3::Zero;
    ASSERT_EQ(0.f, vec.x);
    ASSERT_EQ(0.f, vec.y);
    ASSERT_EQ(0.f, vec.z);
}

UTEST(test_Vec3, CreateOneVector)
{
    Vec3 vec = Vec3::One;
    ASSERT_EQ(1.f, vec.x);
    ASSERT_EQ(1.f, vec.y);
    ASSERT_EQ(1.f, vec.z);
}

UTEST(test_Vec3, CreateUpVector)
{
    Vec3 vec = Vec3::Up;
    ASSERT_EQ(0.f, vec.x);
    ASSERT_EQ(1.f, vec.y);
    ASSERT_EQ(0.f, vec.z);
}

UTEST(test_Vec3, CreateVec2FromString)
{
    Vec3 vec;

    Vec3::FromString(vec, "2,3,4");

    ASSERT_EQ(2.f, vec.x);
    ASSERT_EQ(3.f, vec.y);
    ASSERT_EQ(4.f, vec.z);
}

UTEST(test_Vec3, CreateVec3FromVec2)
{
    Vec2 vec = {1.f, 2.f};

    Vec3 result = vec;

    ASSERT_EQ(1.f, result.x);
    ASSERT_EQ(2.f, result.y);
    ASSERT_EQ(0.f, result.z);
}

UTEST(test_Vec3, CreateVec3FromVec4)
{
    Vec4 vec = {1.f, 2.f, 3.f, 4.f};

    Vec3 result = vec;

    ASSERT_EQ(1.f, result.x);
    ASSERT_EQ(2.f, result.y);
    ASSERT_EQ(3.f, result.z);
}

UTEST(test_Vec3, GetXComp)
{
    Vec3 vec = {10, 15, 20};

    Vec3 xComp = vec.XComp();

    ASSERT_EQ(10.f, xComp.x);
    ASSERT_EQ(0.f, xComp.y);
    ASSERT_EQ(0.f, xComp.z);
}

UTEST(test_Vec3, GetYComp)
{
    Vec3 vec = {10, 15, 20};

    Vec3 yComp = vec.YComp();

    ASSERT_EQ(0.f, yComp.x);
    ASSERT_EQ(15.f, yComp.y);
    ASSERT_EQ(0.f, yComp.z);
}

UTEST(test_Vec3, GetZComp)
{
    Vec3 vec = {10, 15, 20};

    Vec3 zComp = vec.ZComp();

    ASSERT_EQ(0.f, zComp.x);
    ASSERT_EQ(0.f, zComp.y);
    ASSERT_EQ(20.f, zComp.z);
}

UTEST(test_Vec3, TestVectorEquality)
{
    Vec3 vec0 = Vec3::Zero;
    Vec3 vec1 = Vec3::Zero;
    Vec3 vec2 = Vec3::Up;

    ASSERT_TRUE(vec0 == vec1);
    ASSERT_TRUE(vec0 != vec2);
}

UTEST(test_Vec3, AddVectors)
{
    Vec3 vec0 = Vec3::Zero;
    Vec3 vec1 = Vec3::One;

    vec0 += vec1;

    ASSERT_EQ(1.f, vec0.x);
    ASSERT_EQ(1.f, vec0.y);
    ASSERT_EQ(1.f, vec0.z);

    Vec3 vec3 = vec0 + Vec3::One;

    ASSERT_EQ(2.f, vec3.x);
    ASSERT_EQ(2.f, vec3.y);
    ASSERT_EQ(2.f, vec3.z);
}

UTEST(test_Vec3, SubtractVectors)
{
    Vec3 vec0 = Vec3::One;
    Vec3 vec1 = Vec3::Zero;

    vec0 -= vec1;

    ASSERT_EQ(1.f, vec0.x);
    ASSERT_EQ(1.f, vec0.y);
    ASSERT_EQ(1.f, vec0.z);

    Vec3 vec3 = vec0 - Vec3::One;

    ASSERT_EQ(0.f, vec3.x);
    ASSERT_EQ(0.f, vec3.y);
    ASSERT_EQ(0.f, vec3.z);
}

UTEST(test_Vec3, MultiplyVectors)
{
    Vec3 vec0 = {3, 5, 2};
    Vec3 vec1 = {2, 5, 4};

    vec0 *= vec1;

    ASSERT_EQ(6.f, vec0.x);
    ASSERT_EQ(25.f, vec0.y);
    ASSERT_EQ(8.f, vec0.z);

    Vec3 vec3 = vec0 * Vec3::One;

    ASSERT_EQ(6.f, vec3.x);
    ASSERT_EQ(25.f, vec3.y);
    ASSERT_EQ(8.f, vec0.z);
}

UTEST(test_Vec3, DivideVectors)
{
    Vec3 vec0 = {10, 20, 30};
    Vec3 vec1 = {2, 5, 10};

    vec0 /= vec1;

    ASSERT_EQ(5.f, vec0.x);
    ASSERT_EQ(4.f, vec0.y);
    ASSERT_EQ(3.f, vec0.z);

    Vec3 vec3 = vec0 / Vec3::One;

    ASSERT_EQ(5.f, vec3.x);
    ASSERT_EQ(4.f, vec3.y);
    ASSERT_EQ(3.f, vec0.z);
}

UTEST(test_Vec3, CaculateDotProduct)
{
    Vec3 vecA = {2, 3, 4};
    Vec3 VecB = {5, 6, 7};

    auto dot = vecA.Dot(VecB);

    ASSERT_EQ(dot, 56.f);
}

UTEST(test_Vec3, CaculateCrossProduct)
{
    Vec3 vecA = {2, 3, 4};
    Vec3 VecB = {5, 6, 7};

    auto cross = vecA.Cross(VecB);

    ASSERT_EQ(cross.x, -3.f);
    ASSERT_EQ(cross.y, 6.f);
    ASSERT_EQ(cross.z, -3.f);
}

UTEST(test_Vec3, AddVector3)
{
    Vec3 vec0 = Vec3::Zero;
    Vec3 vec1 = {5, 2, 1};

    vec0 += vec1;

    ASSERT_EQ(5.f, vec0.x);
    ASSERT_EQ(2.f, vec0.y);
    ASSERT_EQ(1.f, vec0.z);

    Vec3 vec3 = vec0 + Vec3::One;

    ASSERT_EQ(6.f, vec3.x);
    ASSERT_EQ(3.f, vec3.y);
    ASSERT_EQ(2.f, vec3.z);
}

UTEST(test_Vec3, SubtractVector3)
{
    Vec3 vec0 = Vec3::One;
    Vec3 vec1 = {1.f, 1.f, 1.f};

    vec0 -= vec1;

    ASSERT_EQ(0.f, vec0.x);
    ASSERT_EQ(0.f, vec0.y);
    ASSERT_EQ(0.f, vec0.z);

    Vec3 vec3 = vec0 - Vec3::One;

    ASSERT_EQ(-1.f, vec3.x);
    ASSERT_EQ(-1.f, vec3.y);
    ASSERT_EQ(-1.f, vec3.z);
}

UTEST(test_Vec3, MultiplyVector2)
{
    Vec3 vec0 = {2, 5, 6};
    Vec2 vec1 = {3, 5};

    vec0 *= vec1;

    ASSERT_EQ(6.f, vec0.x);
    ASSERT_EQ(25.f, vec0.y);
    ASSERT_EQ(6.f, vec0.z);

    Vec3 vec3 = vec0 * Vec2::One;

    ASSERT_EQ(6.f, vec3.x);
    ASSERT_EQ(25.f, vec3.y);
    ASSERT_EQ(6.f, vec3.z);
}

UTEST(test_Vec3, DivideVector3)
{
    Vec3 vec0 = {2, 5, 10};
    Vec2 vec1 = {10, 20};

    vec0 /= vec1;

    ASSERT_EQ(.2f, vec0.x);
    ASSERT_EQ(.25f, vec0.y);
    ASSERT_EQ(10.f, vec0.z);

    Vec3 vec3 = vec0 / Vec2::One;

    ASSERT_EQ(.2f, vec3.x);
    ASSERT_EQ(.25f, vec3.y);
    ASSERT_EQ(10.f, vec0.z);
}

UTEST(test_Vec3, AddScalar)
{
    Vec3 vec0 = {5, 10, 20};

    vec0 += 2.f;

    ASSERT_EQ(7.f, vec0.x);
    ASSERT_EQ(12.f, vec0.y);
    ASSERT_EQ(22.f, vec0.z);

    Vec3 vec3 = vec0 + 2.f;

    ASSERT_EQ(9.f, vec3.x);
    ASSERT_EQ(14.f, vec3.y);
    ASSERT_EQ(24.f, vec3.z);
}

UTEST(test_Vec3, SubtractScalar)
{
    Vec3 vec0 = {5, 10, 20};

    vec0 -= 2.f;

    ASSERT_EQ(3.f, vec0.x);
    ASSERT_EQ(8.f, vec0.y);
    ASSERT_EQ(18.f, vec0.z);

    Vec3 vec3 = vec0 - 2.f;

    ASSERT_EQ(1.f, vec3.x);
    ASSERT_EQ(6.f, vec3.y);
    ASSERT_EQ(16.f, vec3.z);
}

UTEST(test_Vec3, MultiplyScalar)
{
    Vec3 vec0 = {5, 10, 20};

    vec0 *= 2.f;

    ASSERT_EQ(10.f, vec0.x);
    ASSERT_EQ(20.f, vec0.y);
    ASSERT_EQ(40.f, vec0.z);

    Vec3 vec3 = vec0 * 2.f;

    ASSERT_EQ(20.f, vec3.x);
    ASSERT_EQ(40.f, vec3.y);
    ASSERT_EQ(80.f, vec3.z);
}

UTEST(test_Vec3, GetString)
{
    String expected = "5.00,10.00,20.00";

    Vec3 vec0 = {5, 10, 20};
    String result = vec0.ToString();

    ASSERT_STREQ(expected.Str(), result.Str());
}

UTEST(test_Vec3, GetVectorLength)
{
    Vec3 vec0 = {2, 3, 4};

    float length = vec0.Length();

    ASSERT_TRUE((length - 5.38516474f) < std::numeric_limits<float>::epsilon());
}

UTEST(test_Vec3, NormaliseVector)
{
    Vec3 vec0 = {2, 3, 4};

    Vec3 norm = vec0.Normalise();

    ASSERT_TRUE((norm.x - 0.37181139f) < std::numeric_limits<float>::epsilon());
    ASSERT_TRUE((norm.y - 0.557717085f) < std::numeric_limits<float>::epsilon());
    ASSERT_TRUE((norm.z - 0.743622779f) < std::numeric_limits<float>::epsilon());
}