//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "utest.h"
#include "utils/math/vec/Vec4.h"

UTEST(test_Vec4, CreateEmptyVec4)
{
    Siege::Vec4 vec = {};
    ASSERT_EQ(0.f, vec.x);
    ASSERT_EQ(0.f, vec.y);
    ASSERT_EQ(0.f, vec.z);
    ASSERT_EQ(0.f, vec.w);
}

UTEST(test_Vec4, CreateVec4WithValues)
{
    Siege::Vec4 vec = {1.f, 2.f, 3.f, 4.f};
    ASSERT_EQ(1.f, vec.x);
    ASSERT_EQ(2.f, vec.y);
    ASSERT_EQ(3.f, vec.z);
    ASSERT_EQ(4.f, vec.w);
}

UTEST(test_Vec4, CreateZeroVector)
{
    Siege::Vec4 vec = Siege::Vec4::Zero;
    ASSERT_EQ(0.f, vec.x);
    ASSERT_EQ(0.f, vec.y);
    ASSERT_EQ(0.f, vec.z);
    ASSERT_EQ(0.f, vec.w);
}

UTEST(test_Vec4, CreateOneVector)
{
    Siege::Vec4 vec = Siege::Vec4::One;
    ASSERT_EQ(1.f, vec.x);
    ASSERT_EQ(1.f, vec.y);
    ASSERT_EQ(1.f, vec.z);
    ASSERT_EQ(1.f, vec.w);
}

UTEST(test_Vec4, CreateUpVector)
{
    Siege::Vec4 vec = Siege::Vec4::Up;
    ASSERT_EQ(0.f, vec.x);
    ASSERT_EQ(1.f, vec.y);
    ASSERT_EQ(0.f, vec.z);
    ASSERT_EQ(0.f, vec.w);
}

UTEST(test_Vec4, LerpVector)
{
    Siege::Vec4 expected = {1.5f, 1.5f, 1.5f, 1.5f};
    Siege::Vec4 vec = Siege::Vec4::One;

    ASSERT_TRUE(expected == Siege::Vec4::Lerp(vec, {2.f, 2.f, 2.f, 2.f}, .5f));
}

UTEST(test_Vec4, CreateVec4FromString)
{
    Siege::Vec4 vec;

    Siege::Vec4::FromString(vec, "2,3,4,5");

    ASSERT_EQ(2.f, vec.x);
    ASSERT_EQ(3.f, vec.y);
    ASSERT_EQ(4.f, vec.z);
    ASSERT_EQ(5.f, vec.w);
}

UTEST(test_Vec4, CreateVec4FromVec3)
{
    Siege::Vec3 vec = {1.f, 2.f, 3.f};

    Siege::Vec4 result = vec;

    ASSERT_EQ(1.f, result.x);
    ASSERT_EQ(2.f, result.y);
    ASSERT_EQ(3.f, result.z);
    ASSERT_EQ(0.f, result.w);
}

UTEST(test_Vec4, CreateVec4FromVec2)
{
    Siege::Vec2 vec = {1.f, 2.f};

    Siege::Vec4 result = vec;

    ASSERT_EQ(1.f, result.x);
    ASSERT_EQ(2.f, result.y);
    ASSERT_EQ(0.f, result.z);
    ASSERT_EQ(0.f, result.w);
}

UTEST(test_Vec4, GetXComp)
{
    Siege::Vec4 vec = {10, 15, 20, 25};

    Siege::Vec4 xComp = vec.XComp();

    ASSERT_EQ(10.f, xComp.x);
    ASSERT_EQ(0.f, xComp.y);
    ASSERT_EQ(0.f, xComp.z);
    ASSERT_EQ(0.f, xComp.w);
}

UTEST(test_Vec4, GetYComp)
{
    Siege::Vec4 vec = {10, 15, 20, 25};

    Siege::Vec4 yComp = vec.YComp();

    ASSERT_EQ(0.f, yComp.x);
    ASSERT_EQ(15.f, yComp.y);
    ASSERT_EQ(0.f, yComp.z);
    ASSERT_EQ(0.f, yComp.w);
}

UTEST(test_Vec4, GetZComp)
{
    Siege::Vec4 vec = {10, 15, 20, 25};

    Siege::Vec4 zComp = vec.ZComp();

    ASSERT_EQ(0.f, zComp.x);
    ASSERT_EQ(0.f, zComp.y);
    ASSERT_EQ(20.f, zComp.z);
    ASSERT_EQ(0.f, zComp.w);
}

UTEST(test_Vec4, GetWComp)
{
    Siege::Vec4 vec = {10, 15, 20, 25};

    Siege::Vec4 wComp = vec.WComp();

    ASSERT_EQ(0.f, wComp.x);
    ASSERT_EQ(0.f, wComp.y);
    ASSERT_EQ(0.f, wComp.z);
    ASSERT_EQ(25.f, wComp.w);
}

UTEST(test_Vec4, TestVectorEquality)
{
    Siege::Vec4 vec0 = Siege::Vec4::Zero;
    Siege::Vec4 vec1 = Siege::Vec4::Zero;
    Siege::Vec4 vec2 = Siege::Vec4::Up;

    ASSERT_TRUE(vec0 == vec1);
    ASSERT_TRUE(vec0 != vec2);
}

UTEST(test_Vec4, AddVectors)
{
    Siege::Vec4 vec0 = Siege::Vec4::Zero;
    Siege::Vec4 vec1 = Siege::Vec4::One;

    vec0 += vec1;

    ASSERT_EQ(1.f, vec0.x);
    ASSERT_EQ(1.f, vec0.y);
    ASSERT_EQ(1.f, vec0.z);
    ASSERT_EQ(1.f, vec0.w);

    Siege::Vec4 vec3 = vec0 + Siege::Vec4::One;

    ASSERT_EQ(2.f, vec3.x);
    ASSERT_EQ(2.f, vec3.y);
    ASSERT_EQ(2.f, vec3.z);
    ASSERT_EQ(2.f, vec3.w);
}

UTEST(test_Vec4, SubtractVectors)
{
    Siege::Vec4 vec0 = Siege::Vec4::One;
    Siege::Vec4 vec1 = Siege::Vec4::Zero;

    vec0 -= vec1;

    ASSERT_EQ(1.f, vec0.x);
    ASSERT_EQ(1.f, vec0.y);
    ASSERT_EQ(1.f, vec0.z);
    ASSERT_EQ(1.f, vec0.w);

    Siege::Vec4 vec3 = vec0 - Siege::Vec4::One;

    ASSERT_EQ(0.f, vec3.x);
    ASSERT_EQ(0.f, vec3.y);
    ASSERT_EQ(0.f, vec3.z);
    ASSERT_EQ(0.f, vec3.w);
}

UTEST(test_Vec4, MultiplyVectors)
{
    Siege::Vec4 vec0 = {3, 5, 6, 7};
    Siege::Vec4 vec1 = {2, 5, 1, 4};

    vec0 *= vec1;

    ASSERT_EQ(6.f, vec0.x);
    ASSERT_EQ(25.f, vec0.y);
    ASSERT_EQ(6.f, vec0.z);
    ASSERT_EQ(28.f, vec0.w);

    Siege::Vec4 vec3 = vec0 * Siege::Vec4::One;

    ASSERT_EQ(6.f, vec3.x);
    ASSERT_EQ(25.f, vec3.y);
    ASSERT_EQ(6.f, vec0.z);
    ASSERT_EQ(28.f, vec0.w);
}

UTEST(test_Vec4, DivideVectors)
{
    Siege::Vec4 vec0 = {10, 20, 30, 40};
    Siege::Vec4 vec1 = {2, 5, 1, 5};

    vec0 /= vec1;

    ASSERT_EQ(5.f, vec0.x);
    ASSERT_EQ(4.f, vec0.y);
    ASSERT_EQ(30.f, vec0.z);
    ASSERT_EQ(8.f, vec0.w);

    Siege::Vec4 vec3 = vec0 / Siege::Vec4::One;

    ASSERT_EQ(5.f, vec3.x);
    ASSERT_EQ(4.f, vec3.y);
    ASSERT_EQ(30.f, vec0.z);
    ASSERT_EQ(8.f, vec0.w);
}

UTEST(test_Vec4, AddVector3)
{
    Siege::Vec4 vec0 = Siege::Vec4::Zero;
    Siege::Vec3 vec1 = {5, 2, 1};

    vec0 += vec1;

    ASSERT_EQ(5.f, vec0.x);
    ASSERT_EQ(2.f, vec0.y);
    ASSERT_EQ(1.f, vec0.z);
    ASSERT_EQ(0.f, vec0.w);

    Siege::Vec4 vec3 = vec0 + Siege::Vec3::One;

    ASSERT_EQ(6.f, vec3.x);
    ASSERT_EQ(3.f, vec3.y);
    ASSERT_EQ(2.f, vec3.z);
    ASSERT_EQ(0.f, vec3.w);
}

UTEST(test_Vec4, SubtractVector3)
{
    Siege::Vec4 vec0 = Siege::Vec4::One;
    Siege::Vec3 vec1 = {1.f, 1.f, 1.f};

    vec0 -= vec1;

    ASSERT_EQ(0.f, vec0.x);
    ASSERT_EQ(0.f, vec0.y);
    ASSERT_EQ(0.f, vec0.z);
    ASSERT_EQ(1.f, vec0.w);

    Siege::Vec4 vec3 = vec0 - Siege::Vec3::One;

    ASSERT_EQ(-1.f, vec3.x);
    ASSERT_EQ(-1.f, vec3.y);
    ASSERT_EQ(-1.f, vec3.z);
    ASSERT_EQ(1.f, vec3.w);
}

UTEST(test_Vec4, MultiplyVector3)
{
    Siege::Vec4 vec0 = {3, 5, 4, 2};
    Siege::Vec3 vec1 = {2, 5, 6};

    vec0 *= vec1;

    ASSERT_EQ(6.f, vec0.x);
    ASSERT_EQ(25.f, vec0.y);
    ASSERT_EQ(24.f, vec0.z);
    ASSERT_EQ(2.f, vec0.w);

    Siege::Vec4 vec3 = vec0 * Siege::Vec3::One;

    ASSERT_EQ(6.f, vec3.x);
    ASSERT_EQ(25.f, vec3.y);
    ASSERT_EQ(24.f, vec3.z);
    ASSERT_EQ(2.f, vec3.w);
}

UTEST(test_Vec4, DivideVector3)
{
    Siege::Vec4 vec0 = {10, 20, 30, 40};
    Siege::Vec3 vec1 = {2, 5, 10};

    vec0 /= vec1;

    ASSERT_EQ(5.f, vec0.x);
    ASSERT_EQ(4.f, vec0.y);
    ASSERT_EQ(3.f, vec0.z);
    ASSERT_EQ(40.f, vec0.w);

    Siege::Vec4 vec3 = vec0 / Siege::Vec3::One;

    ASSERT_EQ(5.f, vec3.x);
    ASSERT_EQ(4.f, vec3.y);
    ASSERT_EQ(3.f, vec3.z);
    ASSERT_EQ(40.f, vec3.w);
}

UTEST(test_Vec4, AddVector2)
{
    Siege::Vec4 vec0 = Siege::Vec4::Zero;
    Siege::Vec2 vec1 = {5, 2};

    vec0 += vec1;

    ASSERT_EQ(5.f, vec0.x);
    ASSERT_EQ(2.f, vec0.y);
    ASSERT_EQ(0.f, vec0.z);
    ASSERT_EQ(0.f, vec0.w);

    Siege::Vec4 vec3 = vec0 + Siege::Vec2::One;

    ASSERT_EQ(6.f, vec3.x);
    ASSERT_EQ(3.f, vec3.y);
    ASSERT_EQ(0.f, vec3.z);
    ASSERT_EQ(0.f, vec3.w);
}

UTEST(test_Vec4, SubtractVector2)
{
    Siege::Vec4 vec0 = Siege::Vec4::One;
    Siege::Vec2 vec1 = {1.f, 1.f};

    vec0 -= vec1;

    ASSERT_EQ(0.f, vec0.x);
    ASSERT_EQ(0.f, vec0.y);
    ASSERT_EQ(1.f, vec0.z);
    ASSERT_EQ(1.f, vec0.w);

    Siege::Vec4 vec3 = vec0 - Siege::Vec2::One;

    ASSERT_EQ(-1.f, vec3.x);
    ASSERT_EQ(-1.f, vec3.y);
    ASSERT_EQ(1.f, vec3.z);
    ASSERT_EQ(1.f, vec3.w);
}

UTEST(test_Vec4, MultiplyVector2)
{
    Siege::Vec4 vec0 = {3, 5, 4, 2};
    Siege::Vec2 vec1 = {2, 5};

    vec0 *= vec1;

    ASSERT_EQ(6.f, vec0.x);
    ASSERT_EQ(25.f, vec0.y);
    ASSERT_EQ(4.f, vec0.z);
    ASSERT_EQ(2.f, vec0.w);

    Siege::Vec4 vec3 = vec0 * Siege::Vec2::One;

    ASSERT_EQ(6.f, vec3.x);
    ASSERT_EQ(25.f, vec3.y);
    ASSERT_EQ(4.f, vec3.z);
    ASSERT_EQ(2.f, vec3.w);
}

UTEST(test_Vec4, DivideVector2)
{
    Siege::Vec4 vec0 = {10, 20, 30, 40};
    Siege::Vec2 vec1 = {2, 5};

    vec0 /= vec1;

    ASSERT_EQ(5.f, vec0.x);
    ASSERT_EQ(4.f, vec0.y);
    ASSERT_EQ(30.f, vec0.z);
    ASSERT_EQ(40.f, vec0.w);

    Siege::Vec4 vec3 = vec0 / Siege::Vec2::One;

    ASSERT_EQ(5.f, vec3.x);
    ASSERT_EQ(4.f, vec3.y);
    ASSERT_EQ(30.f, vec3.z);
    ASSERT_EQ(40.f, vec3.w);
}

UTEST(test_Vec4, AddScalar)
{
    Siege::Vec4 vec0 = {5, 10, 15, 20};

    vec0 += 2.f;

    ASSERT_EQ(7.f, vec0.x);
    ASSERT_EQ(12.f, vec0.y);
    ASSERT_EQ(17.f, vec0.z);
    ASSERT_EQ(22.f, vec0.w);

    Siege::Vec4 vec3 = vec0 + 2.f;

    ASSERT_EQ(9.f, vec3.x);
    ASSERT_EQ(14.f, vec3.y);
    ASSERT_EQ(19.f, vec3.z);
    ASSERT_EQ(24.f, vec3.w);
}

UTEST(test_Vec4, SubtractScalar)
{
    Siege::Vec4 vec0 = {5, 10, 15, 20};

    vec0 -= 2.f;

    ASSERT_EQ(3.f, vec0.x);
    ASSERT_EQ(8.f, vec0.y);
    ASSERT_EQ(13.f, vec0.z);
    ASSERT_EQ(18.f, vec0.w);

    Siege::Vec4 vec3 = vec0 - 2.f;

    ASSERT_EQ(1.f, vec3.x);
    ASSERT_EQ(6.f, vec3.y);
    ASSERT_EQ(11.f, vec3.z);
    ASSERT_EQ(16.f, vec3.w);
}

UTEST(test_Vec4, MultiplyScalar)
{
    Siege::Vec4 vec0 = {5, 10, 15, 20};

    vec0 *= 2.f;

    ASSERT_EQ(10.f, vec0.x);
    ASSERT_EQ(20.f, vec0.y);
    ASSERT_EQ(30.f, vec0.z);
    ASSERT_EQ(40.f, vec0.w);

    Siege::Vec4 vec3 = vec0 * 2.f;

    ASSERT_EQ(20.f, vec3.x);
    ASSERT_EQ(40.f, vec3.y);
    ASSERT_EQ(60.f, vec3.z);
    ASSERT_EQ(80.f, vec3.w);
}

UTEST(test_Vec4, GetString)
{
    Siege::String expected = "5.00,10.00,15.00,20.00";

    Siege::Vec4 vec0 = {5, 10, 15, 20};
    Siege::String result = vec0.ToString();

    ASSERT_STREQ(expected.Str(), result.Str());
}

UTEST(test_Vec4, GetVectorLength)
{
    Siege::Vec4 vec0 = {2, 3, 4, 5};

    float length = vec0.Length();

    ASSERT_TRUE((7.348469f - length) < std::numeric_limits<float>::epsilon());
}

UTEST(test_Vec4, NormaliseVector)
{
    Siege::Vec4 vec0 = {2, 3, 4, 5};

    Siege::Vec4 norm = vec0.Normalise();

    ASSERT_TRUE((norm.x - 0.272165537f) < std::numeric_limits<float>::epsilon());
    ASSERT_TRUE((norm.y - 0.408248276f) < std::numeric_limits<float>::epsilon());
    ASSERT_TRUE((norm.z - 0.544331074f) < std::numeric_limits<float>::epsilon());
    ASSERT_TRUE((norm.w - 0.680413842f) < std::numeric_limits<float>::epsilon());
}
