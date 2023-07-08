//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include <time.h>
#include <utest.h>
#include <utils/math/vec/Vec2.h>
#include <utils/math/vec/Vec3.h>
#include <utils/math/vec/Vec4.h>

#include <utils/math/vec/Equality.h>
#include <utils/math/vec/Format.h>

using namespace Siege;

UTEST(test_Vec4F, CreateVec4)
{
    Vec4 vec {};

    ASSERT_EQ(0, vec.x);
    ASSERT_EQ(0, vec.y);
    ASSERT_EQ(0, vec.z);
    ASSERT_EQ(0, vec.w);
}

UTEST(test_Vec4, TestCreateVec4WithValues)
{
    Vec4 vec {1.f, 2.f, 3.f, 4.f};

    ASSERT_EQ(1.f, vec.x);
    ASSERT_EQ(2.f, vec.y);
    ASSERT_EQ(3.f, vec.z);
    ASSERT_EQ(4.f, vec.w);
}

UTEST(test_Vec4, TestCreateVec2One)
{
    Vec4 vec = Vec4::One();

    ASSERT_EQ(1, vec.x);
    ASSERT_EQ(1, vec.y);
    ASSERT_EQ(1, vec.z);
    ASSERT_EQ(1, vec.w);
}

UTEST(test_Vec3, TestCreateVec2UpVector)
{
    Vec4 vec = Vec4::Up();

    ASSERT_EQ(0, vec.x);
    ASSERT_EQ(-1, vec.y);
    ASSERT_EQ(0, vec.z);
    ASSERT_EQ(0, vec.w);
}

UTEST(test_Vec4, TestCreateVec2RightVector)
{
    Vec4 vec = Vec4::Right();

    ASSERT_EQ(1, vec.x);
    ASSERT_EQ(0, vec.y);
    ASSERT_EQ(0, vec.z);
    ASSERT_EQ(0, vec.w);
}

UTEST(test_Vec4, TestCreateVec4WithVec2)
{
    Vec4 expected = {1.f, 2.f, 0.f, 0.f};

    Vec4 vec = {1.f, 2.f};

    Vec4 result = vec;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Vec4, TestCreateVec4WithVec3)
{
    Vec4 expected = {1.f, 2.f, 3.f, 0.f};

    Vec3 vec = {1.f, 2.f, 3.f};

    Vec4 result = vec;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Vec4, TestSubscriptOperator)
{
    Vec4 vec = {1.f, 2.f, 3.f, 4.f};

    ASSERT_EQ(1.f, vec[0]);
    ASSERT_EQ(2.f, vec[1]);
    ASSERT_EQ(3.f, vec[2]);
    ASSERT_EQ(4.f, vec[3]);
}

UTEST(test_Vec4, TestSubscriptAssignmentOperator)
{
    Vec4 vec {};

    vec[0] = 2.f;
    vec[1] = 1.f;
    vec[2] = 3.f;
    vec[3] = 4.f;

    ASSERT_EQ(2.f, vec[0]);
    ASSERT_EQ(1.f, vec[1]);
    ASSERT_EQ(3.f, vec[2]);
    ASSERT_EQ(4.f, vec[3]);
}

UTEST(test_Vec4, TestAddVectors)
{
    Vec4 expected = {4.f, 2.f, 6.f, 8.f};

    Vec4 vecA = {2.f, 1.f, 3.f, 4.f};

    Vec4 result = vecA + vecA;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Vec4, TestAddScalar)
{
    Vec4 expected = {4.f, 2.f, 6.f, 8.f};

    Vec4 vecA = {2.f, 0.f, 4.f, 6.f};

    Vec4 result = vecA + 2.f;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Vec4, TestSubtractVectors)
{
    Vec4 expected = {4.f, 2.f, 6.f, 8.f};

    Vec4 vecA = {8.f, 4.f, 12.f, 13.f};

    Vec4 vecB = {4.f, 2.f, 6.f, 5.f};

    Vec4 result = vecA - vecB;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Vec4, TestSubtractScalar)
{
    Vec4 expected = {4.f, 2.f, 6.f, 8.f};

    Vec4 vecA = {6.f, 4.f, 8.f, 10.f};

    Vec4 result = vecA - 2.f;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Vec4, TestMultiplyVectors)
{
    Vec4 expected = {4.f, 2.f, 6.f, 12.f};

    Vec4 vecA = {2.f, 1.f, 2.f, 3.f};

    Vec4 vecB = {2.f, 2.f, 3.f, 4.f};

    Vec4 result = vecA * vecB;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Vec4, TestMultiplyScalar)
{
    Vec4 expected = {4.f, 2.f, 6.f, 12.f};

    Vec4 vecA = {2.f, 1.f, 3.f, 6.f};

    Vec4 result = vecA * 2.f;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Vec4, TestDivideVectors)
{
    Vec4 expected = {4.f, 2.f, 6.f, 4.f};

    Vec4 vecA = {8.f, 6.f, 12.f, 16.f};

    Vec4 vecB = {2.f, 3.f, 2.f, 4.f};

    Vec4 result = vecA / vecB;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Vec4, TestDivideScalar)
{
    Vec4 expected = {4.f, 2.f, 6.f, 8.f};

    Vec4 vecA = {8.f, 4.f, 12.f, 16.f};

    Vec4 result = vecA / 2.f;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Vec4, GetVectorLength)
{
    Vec4 vec = {2, 3, 4, 5};

    float length = Vec4::Length(vec);

    ASSERT_TRUE((7.348469f - length) < std::numeric_limits<float>::epsilon());
}

UTEST(test_Vec4, NormaliseVector)
{
    Vec4 vec = {2, 3, 4, 5};

    Vec4 norm = Vec4::Normalise(vec);

    ASSERT_TRUE((norm.x - 0.272165537f) < std::numeric_limits<float>::epsilon());
    ASSERT_TRUE((norm.y - 0.408248276f) < std::numeric_limits<float>::epsilon());
    ASSERT_TRUE((norm.z - 0.544331074f) < std::numeric_limits<float>::epsilon());
    ASSERT_TRUE((norm.w - 0.680413842f) < std::numeric_limits<float>::epsilon());
}

UTEST(test_Vec4, CaculateDotProduct)
{
    Vec4 vecA = {2, 3, 4, 5};
    Vec4 vecB = {5, 6, 7, 8};

    auto dot = Vec4::Dot(vecA, vecB);

    ASSERT_EQ(dot, 96.f);
}

UTEST(test_Vec4, TestVec4CosOperation)
{
    Vec4 expected = {0.540302336f,-0.416146845, -0.989992499, -0.653643608};

    Vec4 vec = {1.f, 2.f, 3.f, 4.f};

    auto result = Vec4::Cos(vec);

    ASSERT_TRUE(result == expected);
}

UTEST(test_Vec4, TestVec4SinOperation)
{
    Vec4 expected = {0.841471016f,0.909297406, 0.141120002, -0.756802499};

    Vec4 vec = {1.f, 2.f, 3.f, 4.f};

    auto result = Vec4::Sin(vec);

    ASSERT_TRUE(result == expected);
}

UTEST(test_Vec4, TestVec4TanOperation)
{
    Vec4 expected = {1.55740774f,-2.18503976, -0.142546549, 1.1578213};

    Vec4 vec = {1.f, 2.f, 3.f, 4.f};

    auto result = Vec4::Tan(vec);

    ASSERT_TRUE(result == expected);
}

UTEST(test_Vec4F, TestSwizzleOperators)
{
    srand(time(NULL));
    float x = rand() % 1000;
    float y = rand() % 1000;
    float z = rand() % 1000;
    float w = rand() % 1000;
    Vec4 vec = {x, y, z, w};
    auto vecXX = vec.XX();
    auto vecXY = vec.XY();
    auto vecXZ = vec.XZ();
    auto vecXW = vec.XW();
    auto vecYX = vec.YX();
    auto vecYY = vec.YY();
    auto vecYZ = vec.YZ();
    auto vecYW = vec.YW();
    auto vecZX = vec.ZX();
    auto vecZY = vec.ZY();
    auto vecZZ = vec.ZZ();
    auto vecZW = vec.ZW();
    auto vecWX = vec.WX();
    auto vecWY = vec.WY();
    auto vecWZ = vec.WZ();
    auto vecWW = vec.WW();

    ASSERT_EQ(x, vecXX.x);
    ASSERT_EQ(x, vecXX.y);
    ASSERT_EQ(x, vecXY.x);
    ASSERT_EQ(y, vecXY.y);
    ASSERT_EQ(x, vecXZ.x);
    ASSERT_EQ(z, vecXZ.y);
    ASSERT_EQ(x, vecXW.x);
    ASSERT_EQ(w, vecXW.y);
    ASSERT_EQ(y, vecYX.x);
    ASSERT_EQ(x, vecYX.y);
    ASSERT_EQ(y, vecYY.x);
    ASSERT_EQ(y, vecYY.y);
    ASSERT_EQ(y, vecYZ.x);
    ASSERT_EQ(z, vecYZ.y);
    ASSERT_EQ(y, vecYW.x);
    ASSERT_EQ(w, vecYW.y);
    ASSERT_EQ(z, vecZX.x);
    ASSERT_EQ(x, vecZX.y);
    ASSERT_EQ(z, vecZY.x);
    ASSERT_EQ(y, vecZY.y);
    ASSERT_EQ(z, vecZZ.x);
    ASSERT_EQ(z, vecZZ.y);
    ASSERT_EQ(z, vecZW.x);
    ASSERT_EQ(w, vecZW.y);
    ASSERT_EQ(w, vecWX.x);
    ASSERT_EQ(x, vecWX.y);
    ASSERT_EQ(w, vecWY.x);
    ASSERT_EQ(y, vecWY.y);
    ASSERT_EQ(w, vecWZ.x);
    ASSERT_EQ(z, vecWZ.y);
    ASSERT_EQ(w, vecWW.x);
    ASSERT_EQ(w, vecWW.y);

    auto vecXXX = vec.XXX();
    auto vecXXY = vec.XXY();
    auto vecXXZ = vec.XXZ();
    auto vecXXW = vec.XXW();
    auto vecXYX = vec.XYX();
    auto vecXYY = vec.XYY();
    auto vecXYZ = vec.XYZ();
    auto vecXYW = vec.XYW();
    auto vecXZX = vec.XZX();
    auto vecXZY = vec.XZY();
    auto vecXZZ = vec.XZZ();
    auto vecXZW = vec.XZW();
    auto vecXWX = vec.XWX();
    auto vecXWY = vec.XWY();
    auto vecXWZ = vec.XWZ();
    auto vecXWW = vec.XWW();
    auto vecYXX = vec.YXX();
    auto vecYXY = vec.YXY();
    auto vecYXZ = vec.YXZ();
    auto vecYXW = vec.YXW();
    auto vecYYX = vec.YYX();
    auto vecYYY = vec.YYY();
    auto vecYYZ = vec.YYZ();
    auto vecYYW = vec.YYW();
    auto vecYZX = vec.YZX();
    auto vecYZY = vec.YZY();
    auto vecYZZ = vec.YZZ();
    auto vecYZW = vec.YZW();
    auto vecYWX = vec.YWX();
    auto vecYWY = vec.YWY();
    auto vecYWZ = vec.YWZ();
    auto vecYWW = vec.YWW();
    auto vecZXX = vec.ZXX();
    auto vecZXY = vec.ZXY();
    auto vecZXZ = vec.ZXZ();
    auto vecZXW = vec.ZXW();
    auto vecZYX = vec.ZYX();
    auto vecZYY = vec.ZYY();
    auto vecZYZ = vec.ZYZ();
    auto vecZYW = vec.ZYW();
    auto vecZZX = vec.ZZX();
    auto vecZZY = vec.ZZY();
    auto vecZZZ = vec.ZZZ();
    auto vecZZW = vec.ZZW();
    auto vecZWX = vec.ZWX();
    auto vecZWY = vec.ZWY();
    auto vecZWZ = vec.ZWZ();
    auto vecZWW = vec.ZWW();
    auto vecWXX = vec.WXX();
    auto vecWXY = vec.WXY();
    auto vecWXZ = vec.WXZ();
    auto vecWXW = vec.WXW();
    auto vecWYX = vec.WYX();
    auto vecWYY = vec.WYY();
    auto vecWYZ = vec.WYZ();
    auto vecWYW = vec.WYW();
    auto vecWZX = vec.WZX();
    auto vecWZY = vec.WZY();
    auto vecWZZ = vec.WZZ();
    auto vecWZW = vec.WZW();
    auto vecWWX = vec.WWX();
    auto vecWWY = vec.WWY();
    auto vecWWZ = vec.WWZ();
    auto vecWWW = vec.WWW();

    ASSERT_EQ(x, vecXXX.x);
    ASSERT_EQ(x, vecXXX.y);
    ASSERT_EQ(x, vecXXX.z);
    ASSERT_EQ(x, vecXXY.x);
    ASSERT_EQ(x, vecXXY.y);
    ASSERT_EQ(y, vecXXY.z);
    ASSERT_EQ(x, vecXXZ.x);
    ASSERT_EQ(x, vecXXZ.y);
    ASSERT_EQ(z, vecXXZ.z);
    ASSERT_EQ(x, vecXXW.x);
    ASSERT_EQ(x, vecXXW.y);
    ASSERT_EQ(w, vecXXW.z);
    ASSERT_EQ(x, vecXYX.x);
    ASSERT_EQ(y, vecXYX.y);
    ASSERT_EQ(x, vecXYX.z);
    ASSERT_EQ(x, vecXYY.x);
    ASSERT_EQ(y, vecXYY.y);
    ASSERT_EQ(y, vecXYY.z);
    ASSERT_EQ(x, vecXYZ.x);
    ASSERT_EQ(y, vecXYZ.y);
    ASSERT_EQ(z, vecXYZ.z);
    ASSERT_EQ(x, vecXYW.x);
    ASSERT_EQ(y, vecXYW.y);
    ASSERT_EQ(w, vecXYW.z);
    ASSERT_EQ(x, vecXZX.x);
    ASSERT_EQ(z, vecXZX.y);
    ASSERT_EQ(x, vecXZX.z);
    ASSERT_EQ(x, vecXZY.x);
    ASSERT_EQ(z, vecXZY.y);
    ASSERT_EQ(y, vecXZY.z);
    ASSERT_EQ(x, vecXZZ.x);
    ASSERT_EQ(z, vecXZZ.y);
    ASSERT_EQ(z, vecXZZ.z);
    ASSERT_EQ(x, vecXZW.x);
    ASSERT_EQ(z, vecXZW.y);
    ASSERT_EQ(w, vecXZW.z);
    ASSERT_EQ(x, vecXWX.x);
    ASSERT_EQ(w, vecXWX.y);
    ASSERT_EQ(x, vecXWX.z);
    ASSERT_EQ(x, vecXWY.x);
    ASSERT_EQ(w, vecXWY.y);
    ASSERT_EQ(y, vecXWY.z);
    ASSERT_EQ(x, vecXWZ.x);
    ASSERT_EQ(w, vecXWZ.y);
    ASSERT_EQ(z, vecXWZ.z);
    ASSERT_EQ(x, vecXWW.x);
    ASSERT_EQ(w, vecXWW.y);
    ASSERT_EQ(w, vecXWW.z);
    ASSERT_EQ(y, vecYXX.x);
    ASSERT_EQ(x, vecYXX.y);
    ASSERT_EQ(x, vecYXX.z);
    ASSERT_EQ(y, vecYXY.x);
    ASSERT_EQ(x, vecYXY.y);
    ASSERT_EQ(y, vecYXY.z);
    ASSERT_EQ(y, vecYXZ.x);
    ASSERT_EQ(x, vecYXZ.y);
    ASSERT_EQ(z, vecYXZ.z);
    ASSERT_EQ(y, vecYXW.x);
    ASSERT_EQ(x, vecYXW.y);
    ASSERT_EQ(w, vecYXW.z);
    ASSERT_EQ(y, vecYYX.x);
    ASSERT_EQ(y, vecYYX.y);
    ASSERT_EQ(x, vecYYX.z);
    ASSERT_EQ(y, vecYYY.x);
    ASSERT_EQ(y, vecYYY.y);
    ASSERT_EQ(y, vecYYY.z);
    ASSERT_EQ(y, vecYYZ.x);
    ASSERT_EQ(y, vecYYZ.y);
    ASSERT_EQ(z, vecYYZ.z);
    ASSERT_EQ(y, vecYYW.x);
    ASSERT_EQ(y, vecYYW.y);
    ASSERT_EQ(w, vecYYW.z);
    ASSERT_EQ(y, vecYZX.x);
    ASSERT_EQ(z, vecYZX.y);
    ASSERT_EQ(x, vecYZX.z);
    ASSERT_EQ(y, vecYZY.x);
    ASSERT_EQ(z, vecYZY.y);
    ASSERT_EQ(y, vecYZY.z);
    ASSERT_EQ(y, vecYZZ.x);
    ASSERT_EQ(z, vecYZZ.y);
    ASSERT_EQ(z, vecYZZ.z);
    ASSERT_EQ(y, vecYZW.x);
    ASSERT_EQ(z, vecYZW.y);
    ASSERT_EQ(w, vecYZW.z);
    ASSERT_EQ(y, vecYWX.x);
    ASSERT_EQ(w, vecYWX.y);
    ASSERT_EQ(x, vecYWX.z);
    ASSERT_EQ(y, vecYWY.x);
    ASSERT_EQ(w, vecYWY.y);
    ASSERT_EQ(y, vecYWY.z);
    ASSERT_EQ(y, vecYWZ.x);
    ASSERT_EQ(w, vecYWZ.y);
    ASSERT_EQ(z, vecYWZ.z);
    ASSERT_EQ(y, vecYWW.x);
    ASSERT_EQ(w, vecYWW.y);
    ASSERT_EQ(w, vecYWW.z);
    ASSERT_EQ(z, vecZXX.x);
    ASSERT_EQ(x, vecZXX.y);
    ASSERT_EQ(x, vecZXX.z);
    ASSERT_EQ(z, vecZXY.x);
    ASSERT_EQ(x, vecZXY.y);
    ASSERT_EQ(y, vecZXY.z);
    ASSERT_EQ(z, vecZXZ.x);
    ASSERT_EQ(x, vecZXZ.y);
    ASSERT_EQ(z, vecZXZ.z);
    ASSERT_EQ(z, vecZXW.x);
    ASSERT_EQ(x, vecZXW.y);
    ASSERT_EQ(w, vecZXW.z);
    ASSERT_EQ(z, vecZYX.x);
    ASSERT_EQ(y, vecZYX.y);
    ASSERT_EQ(x, vecZYX.z);
    ASSERT_EQ(z, vecZYY.x);
    ASSERT_EQ(y, vecZYY.y);
    ASSERT_EQ(y, vecZYY.z);
    ASSERT_EQ(z, vecZYZ.x);
    ASSERT_EQ(y, vecZYZ.y);
    ASSERT_EQ(z, vecZYZ.z);
    ASSERT_EQ(z, vecZYW.x);
    ASSERT_EQ(y, vecZYW.y);
    ASSERT_EQ(w, vecZYW.z);
    ASSERT_EQ(z, vecZZX.x);
    ASSERT_EQ(z, vecZZX.y);
    ASSERT_EQ(x, vecZZX.z);
    ASSERT_EQ(z, vecZZY.x);
    ASSERT_EQ(z, vecZZY.y);
    ASSERT_EQ(y, vecZZY.z);
    ASSERT_EQ(z, vecZZZ.x);
    ASSERT_EQ(z, vecZZZ.y);
    ASSERT_EQ(z, vecZZZ.z);
    ASSERT_EQ(z, vecZZW.x);
    ASSERT_EQ(z, vecZZW.y);
    ASSERT_EQ(w, vecZZW.z);
    ASSERT_EQ(z, vecZWX.x);
    ASSERT_EQ(w, vecZWX.y);
    ASSERT_EQ(x, vecZWX.z);
    ASSERT_EQ(z, vecZWY.x);
    ASSERT_EQ(w, vecZWY.y);
    ASSERT_EQ(y, vecZWY.z);
    ASSERT_EQ(z, vecZWZ.x);
    ASSERT_EQ(w, vecZWZ.y);
    ASSERT_EQ(z, vecZWZ.z);
    ASSERT_EQ(z, vecZWW.x);
    ASSERT_EQ(w, vecZWW.y);
    ASSERT_EQ(w, vecZWW.z);
    ASSERT_EQ(w, vecWXX.x);
    ASSERT_EQ(x, vecWXX.y);
    ASSERT_EQ(x, vecWXX.z);
    ASSERT_EQ(w, vecWXY.x);
    ASSERT_EQ(x, vecWXY.y);
    ASSERT_EQ(y, vecWXY.z);
    ASSERT_EQ(w, vecWXZ.x);
    ASSERT_EQ(x, vecWXZ.y);
    ASSERT_EQ(z, vecWXZ.z);
    ASSERT_EQ(w, vecWXW.x);
    ASSERT_EQ(x, vecWXW.y);
    ASSERT_EQ(w, vecWXW.z);
    ASSERT_EQ(w, vecWYX.x);
    ASSERT_EQ(y, vecWYX.y);
    ASSERT_EQ(x, vecWYX.z);
    ASSERT_EQ(w, vecWYY.x);
    ASSERT_EQ(y, vecWYY.y);
    ASSERT_EQ(y, vecWYY.z);
    ASSERT_EQ(w, vecWYZ.x);
    ASSERT_EQ(y, vecWYZ.y);
    ASSERT_EQ(z, vecWYZ.z);
    ASSERT_EQ(w, vecWYW.x);
    ASSERT_EQ(y, vecWYW.y);
    ASSERT_EQ(w, vecWYW.z);
    ASSERT_EQ(w, vecWZX.x);
    ASSERT_EQ(z, vecWZX.y);
    ASSERT_EQ(x, vecWZX.z);
    ASSERT_EQ(w, vecWZY.x);
    ASSERT_EQ(z, vecWZY.y);
    ASSERT_EQ(y, vecWZY.z);
    ASSERT_EQ(w, vecWZZ.x);
    ASSERT_EQ(z, vecWZZ.y);
    ASSERT_EQ(z, vecWZZ.z);
    ASSERT_EQ(w, vecWZW.x);
    ASSERT_EQ(z, vecWZW.y);
    ASSERT_EQ(w, vecWZW.z);
    ASSERT_EQ(w, vecWWX.x);
    ASSERT_EQ(w, vecWWX.y);
    ASSERT_EQ(x, vecWWX.z);
    ASSERT_EQ(w, vecWWY.x);
    ASSERT_EQ(w, vecWWY.y);
    ASSERT_EQ(y, vecWWY.z);
    ASSERT_EQ(w, vecWWZ.x);
    ASSERT_EQ(w, vecWWZ.y);
    ASSERT_EQ(z, vecWWZ.z);
    ASSERT_EQ(w, vecWWW.x);
    ASSERT_EQ(w, vecWWW.y);
    ASSERT_EQ(w, vecWWW.z);

    auto vecXXXX = vec.XXXX();
    auto vecXXXY = vec.XXXY();
    auto vecXXXZ = vec.XXXZ();
    auto vecXXXW = vec.XXXW();
    auto vecXXYX = vec.XXYX();
    auto vecXXYY = vec.XXYY();
    auto vecXXYZ = vec.XXYZ();
    auto vecXXYW = vec.XXYW();
    auto vecXXZX = vec.XXZX();
    auto vecXXZY = vec.XXZY();
    auto vecXXZZ = vec.XXZZ();
    auto vecXXZW = vec.XXZW();
    auto vecXXWX = vec.XXWX();
    auto vecXXWY = vec.XXWY();
    auto vecXXWZ = vec.XXWZ();
    auto vecXXWW = vec.XXWW();
    auto vecXYXX = vec.XYXX();
    auto vecXYXY = vec.XYXY();
    auto vecXYXZ = vec.XYXZ();
    auto vecXYXW = vec.XYXW();
    auto vecXYYX = vec.XYYX();
    auto vecXYYY = vec.XYYY();
    auto vecXYYZ = vec.XYYZ();
    auto vecXYYW = vec.XYYW();
    auto vecXYZX = vec.XYZX();
    auto vecXYZY = vec.XYZY();
    auto vecXYZZ = vec.XYZZ();
    auto vecXYZW = vec.XYZW();
    auto vecXYWX = vec.XYWX();
    auto vecXYWY = vec.XYWY();
    auto vecXYWZ = vec.XYWZ();
    auto vecXYWW = vec.XYWW();
    auto vecXZXX = vec.XZXX();
    auto vecXZXY = vec.XZXY();
    auto vecXZXZ = vec.XZXZ();
    auto vecXZXW = vec.XZXW();
    auto vecXZYX = vec.XZYX();
    auto vecXZYY = vec.XZYY();
    auto vecXZYZ = vec.XZYZ();
    auto vecXZYW = vec.XZYW();
    auto vecXZZX = vec.XZZX();
    auto vecXZZY = vec.XZZY();
    auto vecXZZZ = vec.XZZZ();
    auto vecXZZW = vec.XZZW();
    auto vecXZWX = vec.XZWX();
    auto vecXZWY = vec.XZWY();
    auto vecXZWZ = vec.XZWZ();
    auto vecXZWW = vec.XZWW();
    auto vecXWXX = vec.XWXX();
    auto vecXWXY = vec.XWXY();
    auto vecXWXZ = vec.XWXZ();
    auto vecXWXW = vec.XWXW();
    auto vecXWYX = vec.XWYX();
    auto vecXWYY = vec.XWYY();
    auto vecXWYZ = vec.XWYZ();
    auto vecXWYW = vec.XWYW();
    auto vecXWZX = vec.XWZX();
    auto vecXWZY = vec.XWZY();
    auto vecXWZZ = vec.XWZZ();
    auto vecXWZW = vec.XWZW();
    auto vecXWWX = vec.XWWX();
    auto vecXWWY = vec.XWWY();
    auto vecXWWZ = vec.XWWZ();
    auto vecXWWW = vec.XWWW();
    auto vecYXXX = vec.YXXX();
    auto vecYXXY = vec.YXXY();
    auto vecYXXZ = vec.YXXZ();
    auto vecYXXW = vec.YXXW();
    auto vecYXYX = vec.YXYX();
    auto vecYXYY = vec.YXYY();
    auto vecYXYZ = vec.YXYZ();
    auto vecYXYW = vec.YXYW();
    auto vecYXZX = vec.YXZX();
    auto vecYXZY = vec.YXZY();
    auto vecYXZZ = vec.YXZZ();
    auto vecYXZW = vec.YXZW();
    auto vecYXWX = vec.YXWX();
    auto vecYXWY = vec.YXWY();
    auto vecYXWZ = vec.YXWZ();
    auto vecYXWW = vec.YXWW();
    auto vecYYXX = vec.YYXX();
    auto vecYYXY = vec.YYXY();
    auto vecYYXZ = vec.YYXZ();
    auto vecYYXW = vec.YYXW();
    auto vecYYYX = vec.YYYX();
    auto vecYYYY = vec.YYYY();
    auto vecYYYZ = vec.YYYZ();
    auto vecYYYW = vec.YYYW();
    auto vecYYZX = vec.YYZX();
    auto vecYYZY = vec.YYZY();
    auto vecYYZZ = vec.YYZZ();
    auto vecYYZW = vec.YYZW();
    auto vecYYWX = vec.YYWX();
    auto vecYYWY = vec.YYWY();
    auto vecYYWZ = vec.YYWZ();
    auto vecYYWW = vec.YYWW();
    auto vecYZXX = vec.YZXX();
    auto vecYZXY = vec.YZXY();
    auto vecYZXZ = vec.YZXZ();
    auto vecYZXW = vec.YZXW();
    auto vecYZYX = vec.YZYX();
    auto vecYZYY = vec.YZYY();
    auto vecYZYZ = vec.YZYZ();
    auto vecYZYW = vec.YZYW();
    auto vecYZZX = vec.YZZX();
    auto vecYZZY = vec.YZZY();
    auto vecYZZZ = vec.YZZZ();
    auto vecYZZW = vec.YZZW();
    auto vecYZWX = vec.YZWX();
    auto vecYZWY = vec.YZWY();
    auto vecYZWZ = vec.YZWZ();
    auto vecYZWW = vec.YZWW();
    auto vecYWXX = vec.YWXX();
    auto vecYWXY = vec.YWXY();
    auto vecYWXZ = vec.YWXZ();
    auto vecYWXW = vec.YWXW();
    auto vecYWYX = vec.YWYX();
    auto vecYWYY = vec.YWYY();
    auto vecYWYZ = vec.YWYZ();
    auto vecYWYW = vec.YWYW();
    auto vecYWZX = vec.YWZX();
    auto vecYWZY = vec.YWZY();
    auto vecYWZZ = vec.YWZZ();
    auto vecYWZW = vec.YWZW();
    auto vecYWWX = vec.YWWX();
    auto vecYWWY = vec.YWWY();
    auto vecYWWZ = vec.YWWZ();
    auto vecYWWW = vec.YWWW();
    auto vecZXXX = vec.ZXXX();
    auto vecZXXY = vec.ZXXY();
    auto vecZXXZ = vec.ZXXZ();
    auto vecZXXW = vec.ZXXW();
    auto vecZXYX = vec.ZXYX();
    auto vecZXYY = vec.ZXYY();
    auto vecZXYZ = vec.ZXYZ();
    auto vecZXYW = vec.ZXYW();
    auto vecZXZX = vec.ZXZX();
    auto vecZXZY = vec.ZXZY();
    auto vecZXZZ = vec.ZXZZ();
    auto vecZXZW = vec.ZXZW();
    auto vecZXWX = vec.ZXWX();
    auto vecZXWY = vec.ZXWY();
    auto vecZXWZ = vec.ZXWZ();
    auto vecZXWW = vec.ZXWW();
    auto vecZYXX = vec.ZYXX();
    auto vecZYXY = vec.ZYXY();
    auto vecZYXZ = vec.ZYXZ();
    auto vecZYXW = vec.ZYXW();
    auto vecZYYX = vec.ZYYX();
    auto vecZYYY = vec.ZYYY();
    auto vecZYYZ = vec.ZYYZ();
    auto vecZYYW = vec.ZYYW();
    auto vecZYZX = vec.ZYZX();
    auto vecZYZY = vec.ZYZY();
    auto vecZYZZ = vec.ZYZZ();
    auto vecZYZW = vec.ZYZW();
    auto vecZYWX = vec.ZYWX();
    auto vecZYWY = vec.ZYWY();
    auto vecZYWZ = vec.ZYWZ();
    auto vecZYWW = vec.ZYWW();
    auto vecZZXX = vec.ZZXX();
    auto vecZZXY = vec.ZZXY();
    auto vecZZXZ = vec.ZZXZ();
    auto vecZZXW = vec.ZZXW();
    auto vecZZYX = vec.ZZYX();
    auto vecZZYY = vec.ZZYY();
    auto vecZZYZ = vec.ZZYZ();
    auto vecZZYW = vec.ZZYW();
    auto vecZZZX = vec.ZZZX();
    auto vecZZZY = vec.ZZZY();
    auto vecZZZZ = vec.ZZZZ();
    auto vecZZZW = vec.ZZZW();
    auto vecZZWX = vec.ZZWX();
    auto vecZZWY = vec.ZZWY();
    auto vecZZWZ = vec.ZZWZ();
    auto vecZZWW = vec.ZZWW();
    auto vecZWXX = vec.ZWXX();
    auto vecZWXY = vec.ZWXY();
    auto vecZWXZ = vec.ZWXZ();
    auto vecZWXW = vec.ZWXW();
    auto vecZWYX = vec.ZWYX();
    auto vecZWYY = vec.ZWYY();
    auto vecZWYZ = vec.ZWYZ();
    auto vecZWYW = vec.ZWYW();
    auto vecZWZX = vec.ZWZX();
    auto vecZWZY = vec.ZWZY();
    auto vecZWZZ = vec.ZWZZ();
    auto vecZWZW = vec.ZWZW();
    auto vecZWWX = vec.ZWWX();
    auto vecZWWY = vec.ZWWY();
    auto vecZWWZ = vec.ZWWZ();
    auto vecZWWW = vec.ZWWW();
    auto vecWXXX = vec.WXXX();
    auto vecWXXY = vec.WXXY();
    auto vecWXXZ = vec.WXXZ();
    auto vecWXXW = vec.WXXW();
    auto vecWXYX = vec.WXYX();
    auto vecWXYY = vec.WXYY();
    auto vecWXYZ = vec.WXYZ();
    auto vecWXYW = vec.WXYW();
    auto vecWXZX = vec.WXZX();
    auto vecWXZY = vec.WXZY();
    auto vecWXZZ = vec.WXZZ();
    auto vecWXZW = vec.WXZW();
    auto vecWXWX = vec.WXWX();
    auto vecWXWY = vec.WXWY();
    auto vecWXWZ = vec.WXWZ();
    auto vecWXWW = vec.WXWW();
    auto vecWYXX = vec.WYXX();
    auto vecWYXY = vec.WYXY();
    auto vecWYXZ = vec.WYXZ();
    auto vecWYXW = vec.WYXW();
    auto vecWYYX = vec.WYYX();
    auto vecWYYY = vec.WYYY();
    auto vecWYYZ = vec.WYYZ();
    auto vecWYYW = vec.WYYW();
    auto vecWYZX = vec.WYZX();
    auto vecWYZY = vec.WYZY();
    auto vecWYZZ = vec.WYZZ();
    auto vecWYZW = vec.WYZW();
    auto vecWYWX = vec.WYWX();
    auto vecWYWY = vec.WYWY();
    auto vecWYWZ = vec.WYWZ();
    auto vecWYWW = vec.WYWW();
    auto vecWZXX = vec.WZXX();
    auto vecWZXY = vec.WZXY();
    auto vecWZXZ = vec.WZXZ();
    auto vecWZXW = vec.WZXW();
    auto vecWZYX = vec.WZYX();
    auto vecWZYY = vec.WZYY();
    auto vecWZYZ = vec.WZYZ();
    auto vecWZYW = vec.WZYW();
    auto vecWZZX = vec.WZZX();
    auto vecWZZY = vec.WZZY();
    auto vecWZZZ = vec.WZZZ();
    auto vecWZZW = vec.WZZW();
    auto vecWZWX = vec.WZWX();
    auto vecWZWY = vec.WZWY();
    auto vecWZWZ = vec.WZWZ();
    auto vecWZWW = vec.WZWW();
    auto vecWWXX = vec.WWXX();
    auto vecWWXY = vec.WWXY();
    auto vecWWXZ = vec.WWXZ();
    auto vecWWXW = vec.WWXW();
    auto vecWWYX = vec.WWYX();
    auto vecWWYY = vec.WWYY();
    auto vecWWYZ = vec.WWYZ();
    auto vecWWYW = vec.WWYW();
    auto vecWWZX = vec.WWZX();
    auto vecWWZY = vec.WWZY();
    auto vecWWZZ = vec.WWZZ();
    auto vecWWZW = vec.WWZW();
    auto vecWWWX = vec.WWWX();
    auto vecWWWY = vec.WWWY();
    auto vecWWWZ = vec.WWWZ();
    auto vecWWWW = vec.WWWW();

    ASSERT_EQ(x, vecXXXX.x);
    ASSERT_EQ(x, vecXXXX.y);
    ASSERT_EQ(x, vecXXXX.z);
    ASSERT_EQ(x, vecXXXX.w);
    ASSERT_EQ(x, vecXXXY.x);
    ASSERT_EQ(x, vecXXXY.y);
    ASSERT_EQ(x, vecXXXY.z);
    ASSERT_EQ(y, vecXXXY.w);
    ASSERT_EQ(x, vecXXXZ.x);
    ASSERT_EQ(x, vecXXXZ.y);
    ASSERT_EQ(x, vecXXXZ.z);
    ASSERT_EQ(z, vecXXXZ.w);
    ASSERT_EQ(x, vecXXXW.x);
    ASSERT_EQ(x, vecXXXW.y);
    ASSERT_EQ(x, vecXXXW.z);
    ASSERT_EQ(w, vecXXXW.w);
    ASSERT_EQ(x, vecXXYX.x);
    ASSERT_EQ(x, vecXXYX.y);
    ASSERT_EQ(y, vecXXYX.z);
    ASSERT_EQ(x, vecXXYX.w);
    ASSERT_EQ(x, vecXXYY.x);
    ASSERT_EQ(x, vecXXYY.y);
    ASSERT_EQ(y, vecXXYY.z);
    ASSERT_EQ(y, vecXXYY.w);
    ASSERT_EQ(x, vecXXYZ.x);
    ASSERT_EQ(x, vecXXYZ.y);
    ASSERT_EQ(y, vecXXYZ.z);
    ASSERT_EQ(z, vecXXYZ.w);
    ASSERT_EQ(x, vecXXYW.x);
    ASSERT_EQ(x, vecXXYW.y);
    ASSERT_EQ(y, vecXXYW.z);
    ASSERT_EQ(w, vecXXYW.w);
    ASSERT_EQ(x, vecXXZX.x);
    ASSERT_EQ(x, vecXXZX.y);
    ASSERT_EQ(z, vecXXZX.z);
    ASSERT_EQ(x, vecXXZX.w);
    ASSERT_EQ(x, vecXXZY.x);
    ASSERT_EQ(x, vecXXZY.y);
    ASSERT_EQ(z, vecXXZY.z);
    ASSERT_EQ(y, vecXXZY.w);
    ASSERT_EQ(x, vecXXZZ.x);
    ASSERT_EQ(x, vecXXZZ.y);
    ASSERT_EQ(z, vecXXZZ.z);
    ASSERT_EQ(z, vecXXZZ.w);
    ASSERT_EQ(x, vecXXZW.x);
    ASSERT_EQ(x, vecXXZW.y);
    ASSERT_EQ(z, vecXXZW.z);
    ASSERT_EQ(w, vecXXZW.w);
    ASSERT_EQ(x, vecXXWX.x);
    ASSERT_EQ(x, vecXXWX.y);
    ASSERT_EQ(w, vecXXWX.z);
    ASSERT_EQ(x, vecXXWX.w);
    ASSERT_EQ(x, vecXXWY.x);
    ASSERT_EQ(x, vecXXWY.y);
    ASSERT_EQ(w, vecXXWY.z);
    ASSERT_EQ(y, vecXXWY.w);
    ASSERT_EQ(x, vecXXWZ.x);
    ASSERT_EQ(x, vecXXWZ.y);
    ASSERT_EQ(w, vecXXWZ.z);
    ASSERT_EQ(z, vecXXWZ.w);
    ASSERT_EQ(x, vecXXWW.x);
    ASSERT_EQ(x, vecXXWW.y);
    ASSERT_EQ(w, vecXXWW.z);
    ASSERT_EQ(w, vecXXWW.w);
    ASSERT_EQ(x, vecXYXX.x);
    ASSERT_EQ(y, vecXYXX.y);
    ASSERT_EQ(x, vecXYXX.z);
    ASSERT_EQ(x, vecXYXX.w);
    ASSERT_EQ(x, vecXYXY.x);
    ASSERT_EQ(y, vecXYXY.y);
    ASSERT_EQ(x, vecXYXY.z);
    ASSERT_EQ(y, vecXYXY.w);
    ASSERT_EQ(x, vecXYXZ.x);
    ASSERT_EQ(y, vecXYXZ.y);
    ASSERT_EQ(x, vecXYXZ.z);
    ASSERT_EQ(z, vecXYXZ.w);
    ASSERT_EQ(x, vecXYXW.x);
    ASSERT_EQ(y, vecXYXW.y);
    ASSERT_EQ(x, vecXYXW.z);
    ASSERT_EQ(w, vecXYXW.w);
    ASSERT_EQ(x, vecXYYX.x);
    ASSERT_EQ(y, vecXYYX.y);
    ASSERT_EQ(y, vecXYYX.z);
    ASSERT_EQ(x, vecXYYX.w);
    ASSERT_EQ(x, vecXYYY.x);
    ASSERT_EQ(y, vecXYYY.y);
    ASSERT_EQ(y, vecXYYY.z);
    ASSERT_EQ(y, vecXYYY.w);
    ASSERT_EQ(x, vecXYYZ.x);
    ASSERT_EQ(y, vecXYYZ.y);
    ASSERT_EQ(y, vecXYYZ.z);
    ASSERT_EQ(z, vecXYYZ.w);
    ASSERT_EQ(x, vecXYYW.x);
    ASSERT_EQ(y, vecXYYW.y);
    ASSERT_EQ(y, vecXYYW.z);
    ASSERT_EQ(w, vecXYYW.w);
    ASSERT_EQ(x, vecXYZX.x);
    ASSERT_EQ(y, vecXYZX.y);
    ASSERT_EQ(z, vecXYZX.z);
    ASSERT_EQ(x, vecXYZX.w);
    ASSERT_EQ(x, vecXYZY.x);
    ASSERT_EQ(y, vecXYZY.y);
    ASSERT_EQ(z, vecXYZY.z);
    ASSERT_EQ(y, vecXYZY.w);
    ASSERT_EQ(x, vecXYZZ.x);
    ASSERT_EQ(y, vecXYZZ.y);
    ASSERT_EQ(z, vecXYZZ.z);
    ASSERT_EQ(z, vecXYZZ.w);
    ASSERT_EQ(x, vecXYZW.x);
    ASSERT_EQ(y, vecXYZW.y);
    ASSERT_EQ(z, vecXYZW.z);
    ASSERT_EQ(w, vecXYZW.w);
    ASSERT_EQ(x, vecXYWX.x);
    ASSERT_EQ(y, vecXYWX.y);
    ASSERT_EQ(w, vecXYWX.z);
    ASSERT_EQ(x, vecXYWX.w);
    ASSERT_EQ(x, vecXYWY.x);
    ASSERT_EQ(y, vecXYWY.y);
    ASSERT_EQ(w, vecXYWY.z);
    ASSERT_EQ(y, vecXYWY.w);
    ASSERT_EQ(x, vecXYWZ.x);
    ASSERT_EQ(y, vecXYWZ.y);
    ASSERT_EQ(w, vecXYWZ.z);
    ASSERT_EQ(z, vecXYWZ.w);
    ASSERT_EQ(x, vecXYWW.x);
    ASSERT_EQ(y, vecXYWW.y);
    ASSERT_EQ(w, vecXYWW.z);
    ASSERT_EQ(w, vecXYWW.w);
    ASSERT_EQ(x, vecXZXX.x);
    ASSERT_EQ(z, vecXZXX.y);
    ASSERT_EQ(x, vecXZXX.z);
    ASSERT_EQ(x, vecXZXX.w);
    ASSERT_EQ(x, vecXZXY.x);
    ASSERT_EQ(z, vecXZXY.y);
    ASSERT_EQ(x, vecXZXY.z);
    ASSERT_EQ(y, vecXZXY.w);
    ASSERT_EQ(x, vecXZXZ.x);
    ASSERT_EQ(z, vecXZXZ.y);
    ASSERT_EQ(x, vecXZXZ.z);
    ASSERT_EQ(z, vecXZXZ.w);
    ASSERT_EQ(x, vecXZXW.x);
    ASSERT_EQ(z, vecXZXW.y);
    ASSERT_EQ(x, vecXZXW.z);
    ASSERT_EQ(w, vecXZXW.w);
    ASSERT_EQ(x, vecXZYX.x);
    ASSERT_EQ(z, vecXZYX.y);
    ASSERT_EQ(y, vecXZYX.z);
    ASSERT_EQ(x, vecXZYX.w);
    ASSERT_EQ(x, vecXZYY.x);
    ASSERT_EQ(z, vecXZYY.y);
    ASSERT_EQ(y, vecXZYY.z);
    ASSERT_EQ(y, vecXZYY.w);
    ASSERT_EQ(x, vecXZYZ.x);
    ASSERT_EQ(z, vecXZYZ.y);
    ASSERT_EQ(y, vecXZYZ.z);
    ASSERT_EQ(z, vecXZYZ.w);
    ASSERT_EQ(x, vecXZYW.x);
    ASSERT_EQ(z, vecXZYW.y);
    ASSERT_EQ(y, vecXZYW.z);
    ASSERT_EQ(w, vecXZYW.w);
    ASSERT_EQ(x, vecXZZX.x);
    ASSERT_EQ(z, vecXZZX.y);
    ASSERT_EQ(z, vecXZZX.z);
    ASSERT_EQ(x, vecXZZX.w);
    ASSERT_EQ(x, vecXZZY.x);
    ASSERT_EQ(z, vecXZZY.y);
    ASSERT_EQ(z, vecXZZY.z);
    ASSERT_EQ(y, vecXZZY.w);
    ASSERT_EQ(x, vecXZZZ.x);
    ASSERT_EQ(z, vecXZZZ.y);
    ASSERT_EQ(z, vecXZZZ.z);
    ASSERT_EQ(z, vecXZZZ.w);
    ASSERT_EQ(x, vecXZZW.x);
    ASSERT_EQ(z, vecXZZW.y);
    ASSERT_EQ(z, vecXZZW.z);
    ASSERT_EQ(w, vecXZZW.w);
    ASSERT_EQ(x, vecXZWX.x);
    ASSERT_EQ(z, vecXZWX.y);
    ASSERT_EQ(w, vecXZWX.z);
    ASSERT_EQ(x, vecXZWX.w);
    ASSERT_EQ(x, vecXZWY.x);
    ASSERT_EQ(z, vecXZWY.y);
    ASSERT_EQ(w, vecXZWY.z);
    ASSERT_EQ(y, vecXZWY.w);
    ASSERT_EQ(x, vecXZWZ.x);
    ASSERT_EQ(z, vecXZWZ.y);
    ASSERT_EQ(w, vecXZWZ.z);
    ASSERT_EQ(z, vecXZWZ.w);
    ASSERT_EQ(x, vecXZWW.x);
    ASSERT_EQ(z, vecXZWW.y);
    ASSERT_EQ(w, vecXZWW.z);
    ASSERT_EQ(w, vecXZWW.w);
    ASSERT_EQ(x, vecXWXX.x);
    ASSERT_EQ(w, vecXWXX.y);
    ASSERT_EQ(x, vecXWXX.z);
    ASSERT_EQ(x, vecXWXX.w);
    ASSERT_EQ(x, vecXWXY.x);
    ASSERT_EQ(w, vecXWXY.y);
    ASSERT_EQ(x, vecXWXY.z);
    ASSERT_EQ(y, vecXWXY.w);
    ASSERT_EQ(x, vecXWXZ.x);
    ASSERT_EQ(w, vecXWXZ.y);
    ASSERT_EQ(x, vecXWXZ.z);
    ASSERT_EQ(z, vecXWXZ.w);
    ASSERT_EQ(x, vecXWXW.x);
    ASSERT_EQ(w, vecXWXW.y);
    ASSERT_EQ(x, vecXWXW.z);
    ASSERT_EQ(w, vecXWXW.w);
    ASSERT_EQ(x, vecXWYX.x);
    ASSERT_EQ(w, vecXWYX.y);
    ASSERT_EQ(y, vecXWYX.z);
    ASSERT_EQ(x, vecXWYX.w);
    ASSERT_EQ(x, vecXWYY.x);
    ASSERT_EQ(w, vecXWYY.y);
    ASSERT_EQ(y, vecXWYY.z);
    ASSERT_EQ(y, vecXWYY.w);
    ASSERT_EQ(x, vecXWYZ.x);
    ASSERT_EQ(w, vecXWYZ.y);
    ASSERT_EQ(y, vecXWYZ.z);
    ASSERT_EQ(z, vecXWYZ.w);
    ASSERT_EQ(x, vecXWYW.x);
    ASSERT_EQ(w, vecXWYW.y);
    ASSERT_EQ(y, vecXWYW.z);
    ASSERT_EQ(w, vecXWYW.w);
    ASSERT_EQ(x, vecXWZX.x);
    ASSERT_EQ(w, vecXWZX.y);
    ASSERT_EQ(z, vecXWZX.z);
    ASSERT_EQ(x, vecXWZX.w);
    ASSERT_EQ(x, vecXWZY.x);
    ASSERT_EQ(w, vecXWZY.y);
    ASSERT_EQ(z, vecXWZY.z);
    ASSERT_EQ(y, vecXWZY.w);
    ASSERT_EQ(x, vecXWZZ.x);
    ASSERT_EQ(w, vecXWZZ.y);
    ASSERT_EQ(z, vecXWZZ.z);
    ASSERT_EQ(z, vecXWZZ.w);
    ASSERT_EQ(x, vecXWZW.x);
    ASSERT_EQ(w, vecXWZW.y);
    ASSERT_EQ(z, vecXWZW.z);
    ASSERT_EQ(w, vecXWZW.w);
    ASSERT_EQ(x, vecXWWX.x);
    ASSERT_EQ(w, vecXWWX.y);
    ASSERT_EQ(w, vecXWWX.z);
    ASSERT_EQ(x, vecXWWX.w);
    ASSERT_EQ(x, vecXWWY.x);
    ASSERT_EQ(w, vecXWWY.y);
    ASSERT_EQ(w, vecXWWY.z);
    ASSERT_EQ(y, vecXWWY.w);
    ASSERT_EQ(x, vecXWWZ.x);
    ASSERT_EQ(w, vecXWWZ.y);
    ASSERT_EQ(w, vecXWWZ.z);
    ASSERT_EQ(z, vecXWWZ.w);
    ASSERT_EQ(x, vecXWWW.x);
    ASSERT_EQ(w, vecXWWW.y);
    ASSERT_EQ(w, vecXWWW.z);
    ASSERT_EQ(w, vecXWWW.w);
    ASSERT_EQ(y, vecYXXX.x);
    ASSERT_EQ(x, vecYXXX.y);
    ASSERT_EQ(x, vecYXXX.z);
    ASSERT_EQ(x, vecYXXX.w);
    ASSERT_EQ(y, vecYXXY.x);
    ASSERT_EQ(x, vecYXXY.y);
    ASSERT_EQ(x, vecYXXY.z);
    ASSERT_EQ(y, vecYXXY.w);
    ASSERT_EQ(y, vecYXXZ.x);
    ASSERT_EQ(x, vecYXXZ.y);
    ASSERT_EQ(x, vecYXXZ.z);
    ASSERT_EQ(z, vecYXXZ.w);
    ASSERT_EQ(y, vecYXXW.x);
    ASSERT_EQ(x, vecYXXW.y);
    ASSERT_EQ(x, vecYXXW.z);
    ASSERT_EQ(w, vecYXXW.w);
    ASSERT_EQ(y, vecYXYX.x);
    ASSERT_EQ(x, vecYXYX.y);
    ASSERT_EQ(y, vecYXYX.z);
    ASSERT_EQ(x, vecYXYX.w);
    ASSERT_EQ(y, vecYXYY.x);
    ASSERT_EQ(x, vecYXYY.y);
    ASSERT_EQ(y, vecYXYY.z);
    ASSERT_EQ(y, vecYXYY.w);
    ASSERT_EQ(y, vecYXYZ.x);
    ASSERT_EQ(x, vecYXYZ.y);
    ASSERT_EQ(y, vecYXYZ.z);
    ASSERT_EQ(z, vecYXYZ.w);
    ASSERT_EQ(y, vecYXYW.x);
    ASSERT_EQ(x, vecYXYW.y);
    ASSERT_EQ(y, vecYXYW.z);
    ASSERT_EQ(w, vecYXYW.w);
    ASSERT_EQ(y, vecYXZX.x);
    ASSERT_EQ(x, vecYXZX.y);
    ASSERT_EQ(z, vecYXZX.z);
    ASSERT_EQ(x, vecYXZX.w);
    ASSERT_EQ(y, vecYXZY.x);
    ASSERT_EQ(x, vecYXZY.y);
    ASSERT_EQ(z, vecYXZY.z);
    ASSERT_EQ(y, vecYXZY.w);
    ASSERT_EQ(y, vecYXZZ.x);
    ASSERT_EQ(x, vecYXZZ.y);
    ASSERT_EQ(z, vecYXZZ.z);
    ASSERT_EQ(z, vecYXZZ.w);
    ASSERT_EQ(y, vecYXZW.x);
    ASSERT_EQ(x, vecYXZW.y);
    ASSERT_EQ(z, vecYXZW.z);
    ASSERT_EQ(w, vecYXZW.w);
    ASSERT_EQ(y, vecYXWX.x);
    ASSERT_EQ(x, vecYXWX.y);
    ASSERT_EQ(w, vecYXWX.z);
    ASSERT_EQ(x, vecYXWX.w);
    ASSERT_EQ(y, vecYXWY.x);
    ASSERT_EQ(x, vecYXWY.y);
    ASSERT_EQ(w, vecYXWY.z);
    ASSERT_EQ(y, vecYXWY.w);
    ASSERT_EQ(y, vecYXWZ.x);
    ASSERT_EQ(x, vecYXWZ.y);
    ASSERT_EQ(w, vecYXWZ.z);
    ASSERT_EQ(z, vecYXWZ.w);
    ASSERT_EQ(y, vecYXWW.x);
    ASSERT_EQ(x, vecYXWW.y);
    ASSERT_EQ(w, vecYXWW.z);
    ASSERT_EQ(w, vecYXWW.w);
    ASSERT_EQ(y, vecYYXX.x);
    ASSERT_EQ(y, vecYYXX.y);
    ASSERT_EQ(x, vecYYXX.z);
    ASSERT_EQ(x, vecYYXX.w);
    ASSERT_EQ(y, vecYYXY.x);
    ASSERT_EQ(y, vecYYXY.y);
    ASSERT_EQ(x, vecYYXY.z);
    ASSERT_EQ(y, vecYYXY.w);
    ASSERT_EQ(y, vecYYXZ.x);
    ASSERT_EQ(y, vecYYXZ.y);
    ASSERT_EQ(x, vecYYXZ.z);
    ASSERT_EQ(z, vecYYXZ.w);
    ASSERT_EQ(y, vecYYXW.x);
    ASSERT_EQ(y, vecYYXW.y);
    ASSERT_EQ(x, vecYYXW.z);
    ASSERT_EQ(w, vecYYXW.w);
    ASSERT_EQ(y, vecYYYX.x);
    ASSERT_EQ(y, vecYYYX.y);
    ASSERT_EQ(y, vecYYYX.z);
    ASSERT_EQ(x, vecYYYX.w);
    ASSERT_EQ(y, vecYYYY.x);
    ASSERT_EQ(y, vecYYYY.y);
    ASSERT_EQ(y, vecYYYY.z);
    ASSERT_EQ(y, vecYYYY.w);
    ASSERT_EQ(y, vecYYYZ.x);
    ASSERT_EQ(y, vecYYYZ.y);
    ASSERT_EQ(y, vecYYYZ.z);
    ASSERT_EQ(z, vecYYYZ.w);
    ASSERT_EQ(y, vecYYYW.x);
    ASSERT_EQ(y, vecYYYW.y);
    ASSERT_EQ(y, vecYYYW.z);
    ASSERT_EQ(w, vecYYYW.w);
    ASSERT_EQ(y, vecYYZX.x);
    ASSERT_EQ(y, vecYYZX.y);
    ASSERT_EQ(z, vecYYZX.z);
    ASSERT_EQ(x, vecYYZX.w);
    ASSERT_EQ(y, vecYYZY.x);
    ASSERT_EQ(y, vecYYZY.y);
    ASSERT_EQ(z, vecYYZY.z);
    ASSERT_EQ(y, vecYYZY.w);
    ASSERT_EQ(y, vecYYZZ.x);
    ASSERT_EQ(y, vecYYZZ.y);
    ASSERT_EQ(z, vecYYZZ.z);
    ASSERT_EQ(z, vecYYZZ.w);
    ASSERT_EQ(y, vecYYZW.x);
    ASSERT_EQ(y, vecYYZW.y);
    ASSERT_EQ(z, vecYYZW.z);
    ASSERT_EQ(w, vecYYZW.w);
    ASSERT_EQ(y, vecYYWX.x);
    ASSERT_EQ(y, vecYYWX.y);
    ASSERT_EQ(w, vecYYWX.z);
    ASSERT_EQ(x, vecYYWX.w);
    ASSERT_EQ(y, vecYYWY.x);
    ASSERT_EQ(y, vecYYWY.y);
    ASSERT_EQ(w, vecYYWY.z);
    ASSERT_EQ(y, vecYYWY.w);
    ASSERT_EQ(y, vecYYWZ.x);
    ASSERT_EQ(y, vecYYWZ.y);
    ASSERT_EQ(w, vecYYWZ.z);
    ASSERT_EQ(z, vecYYWZ.w);
    ASSERT_EQ(y, vecYYWW.x);
    ASSERT_EQ(y, vecYYWW.y);
    ASSERT_EQ(w, vecYYWW.z);
    ASSERT_EQ(w, vecYYWW.w);
    ASSERT_EQ(y, vecYZXX.x);
    ASSERT_EQ(z, vecYZXX.y);
    ASSERT_EQ(x, vecYZXX.z);
    ASSERT_EQ(x, vecYZXX.w);
    ASSERT_EQ(y, vecYZXY.x);
    ASSERT_EQ(z, vecYZXY.y);
    ASSERT_EQ(x, vecYZXY.z);
    ASSERT_EQ(y, vecYZXY.w);
    ASSERT_EQ(y, vecYZXZ.x);
    ASSERT_EQ(z, vecYZXZ.y);
    ASSERT_EQ(x, vecYZXZ.z);
    ASSERT_EQ(z, vecYZXZ.w);
    ASSERT_EQ(y, vecYZXW.x);
    ASSERT_EQ(z, vecYZXW.y);
    ASSERT_EQ(x, vecYZXW.z);
    ASSERT_EQ(w, vecYZXW.w);
    ASSERT_EQ(y, vecYZYX.x);
    ASSERT_EQ(z, vecYZYX.y);
    ASSERT_EQ(y, vecYZYX.z);
    ASSERT_EQ(x, vecYZYX.w);
    ASSERT_EQ(y, vecYZYY.x);
    ASSERT_EQ(z, vecYZYY.y);
    ASSERT_EQ(y, vecYZYY.z);
    ASSERT_EQ(y, vecYZYY.w);
    ASSERT_EQ(y, vecYZYZ.x);
    ASSERT_EQ(z, vecYZYZ.y);
    ASSERT_EQ(y, vecYZYZ.z);
    ASSERT_EQ(z, vecYZYZ.w);
    ASSERT_EQ(y, vecYZYW.x);
    ASSERT_EQ(z, vecYZYW.y);
    ASSERT_EQ(y, vecYZYW.z);
    ASSERT_EQ(w, vecYZYW.w);
    ASSERT_EQ(y, vecYZZX.x);
    ASSERT_EQ(z, vecYZZX.y);
    ASSERT_EQ(z, vecYZZX.z);
    ASSERT_EQ(x, vecYZZX.w);
    ASSERT_EQ(y, vecYZZY.x);
    ASSERT_EQ(z, vecYZZY.y);
    ASSERT_EQ(z, vecYZZY.z);
    ASSERT_EQ(y, vecYZZY.w);
    ASSERT_EQ(y, vecYZZZ.x);
    ASSERT_EQ(z, vecYZZZ.y);
    ASSERT_EQ(z, vecYZZZ.z);
    ASSERT_EQ(z, vecYZZZ.w);
    ASSERT_EQ(y, vecYZZW.x);
    ASSERT_EQ(z, vecYZZW.y);
    ASSERT_EQ(z, vecYZZW.z);
    ASSERT_EQ(w, vecYZZW.w);
    ASSERT_EQ(y, vecYZWX.x);
    ASSERT_EQ(z, vecYZWX.y);
    ASSERT_EQ(w, vecYZWX.z);
    ASSERT_EQ(x, vecYZWX.w);
    ASSERT_EQ(y, vecYZWY.x);
    ASSERT_EQ(z, vecYZWY.y);
    ASSERT_EQ(w, vecYZWY.z);
    ASSERT_EQ(y, vecYZWY.w);
    ASSERT_EQ(y, vecYZWZ.x);
    ASSERT_EQ(z, vecYZWZ.y);
    ASSERT_EQ(w, vecYZWZ.z);
    ASSERT_EQ(z, vecYZWZ.w);
    ASSERT_EQ(y, vecYZWW.x);
    ASSERT_EQ(z, vecYZWW.y);
    ASSERT_EQ(w, vecYZWW.z);
    ASSERT_EQ(w, vecYZWW.w);
    ASSERT_EQ(y, vecYWXX.x);
    ASSERT_EQ(w, vecYWXX.y);
    ASSERT_EQ(x, vecYWXX.z);
    ASSERT_EQ(x, vecYWXX.w);
    ASSERT_EQ(y, vecYWXY.x);
    ASSERT_EQ(w, vecYWXY.y);
    ASSERT_EQ(x, vecYWXY.z);
    ASSERT_EQ(y, vecYWXY.w);
    ASSERT_EQ(y, vecYWXZ.x);
    ASSERT_EQ(w, vecYWXZ.y);
    ASSERT_EQ(x, vecYWXZ.z);
    ASSERT_EQ(z, vecYWXZ.w);
    ASSERT_EQ(y, vecYWXW.x);
    ASSERT_EQ(w, vecYWXW.y);
    ASSERT_EQ(x, vecYWXW.z);
    ASSERT_EQ(w, vecYWXW.w);
    ASSERT_EQ(y, vecYWYX.x);
    ASSERT_EQ(w, vecYWYX.y);
    ASSERT_EQ(y, vecYWYX.z);
    ASSERT_EQ(x, vecYWYX.w);
    ASSERT_EQ(y, vecYWYY.x);
    ASSERT_EQ(w, vecYWYY.y);
    ASSERT_EQ(y, vecYWYY.z);
    ASSERT_EQ(y, vecYWYY.w);
    ASSERT_EQ(y, vecYWYZ.x);
    ASSERT_EQ(w, vecYWYZ.y);
    ASSERT_EQ(y, vecYWYZ.z);
    ASSERT_EQ(z, vecYWYZ.w);
    ASSERT_EQ(y, vecYWYW.x);
    ASSERT_EQ(w, vecYWYW.y);
    ASSERT_EQ(y, vecYWYW.z);
    ASSERT_EQ(w, vecYWYW.w);
    ASSERT_EQ(y, vecYWZX.x);
    ASSERT_EQ(w, vecYWZX.y);
    ASSERT_EQ(z, vecYWZX.z);
    ASSERT_EQ(x, vecYWZX.w);
    ASSERT_EQ(y, vecYWZY.x);
    ASSERT_EQ(w, vecYWZY.y);
    ASSERT_EQ(z, vecYWZY.z);
    ASSERT_EQ(y, vecYWZY.w);
    ASSERT_EQ(y, vecYWZZ.x);
    ASSERT_EQ(w, vecYWZZ.y);
    ASSERT_EQ(z, vecYWZZ.z);
    ASSERT_EQ(z, vecYWZZ.w);
    ASSERT_EQ(y, vecYWZW.x);
    ASSERT_EQ(w, vecYWZW.y);
    ASSERT_EQ(z, vecYWZW.z);
    ASSERT_EQ(w, vecYWZW.w);
    ASSERT_EQ(y, vecYWWX.x);
    ASSERT_EQ(w, vecYWWX.y);
    ASSERT_EQ(w, vecYWWX.z);
    ASSERT_EQ(x, vecYWWX.w);
    ASSERT_EQ(y, vecYWWY.x);
    ASSERT_EQ(w, vecYWWY.y);
    ASSERT_EQ(w, vecYWWY.z);
    ASSERT_EQ(y, vecYWWY.w);
    ASSERT_EQ(y, vecYWWZ.x);
    ASSERT_EQ(w, vecYWWZ.y);
    ASSERT_EQ(w, vecYWWZ.z);
    ASSERT_EQ(z, vecYWWZ.w);
    ASSERT_EQ(y, vecYWWW.x);
    ASSERT_EQ(w, vecYWWW.y);
    ASSERT_EQ(w, vecYWWW.z);
    ASSERT_EQ(w, vecYWWW.w);
    ASSERT_EQ(z, vecZXXX.x);
    ASSERT_EQ(x, vecZXXX.y);
    ASSERT_EQ(x, vecZXXX.z);
    ASSERT_EQ(x, vecZXXX.w);
    ASSERT_EQ(z, vecZXXY.x);
    ASSERT_EQ(x, vecZXXY.y);
    ASSERT_EQ(x, vecZXXY.z);
    ASSERT_EQ(y, vecZXXY.w);
    ASSERT_EQ(z, vecZXXZ.x);
    ASSERT_EQ(x, vecZXXZ.y);
    ASSERT_EQ(x, vecZXXZ.z);
    ASSERT_EQ(z, vecZXXZ.w);
    ASSERT_EQ(z, vecZXXW.x);
    ASSERT_EQ(x, vecZXXW.y);
    ASSERT_EQ(x, vecZXXW.z);
    ASSERT_EQ(w, vecZXXW.w);
    ASSERT_EQ(z, vecZXYX.x);
    ASSERT_EQ(x, vecZXYX.y);
    ASSERT_EQ(y, vecZXYX.z);
    ASSERT_EQ(x, vecZXYX.w);
    ASSERT_EQ(z, vecZXYY.x);
    ASSERT_EQ(x, vecZXYY.y);
    ASSERT_EQ(y, vecZXYY.z);
    ASSERT_EQ(y, vecZXYY.w);
    ASSERT_EQ(z, vecZXYZ.x);
    ASSERT_EQ(x, vecZXYZ.y);
    ASSERT_EQ(y, vecZXYZ.z);
    ASSERT_EQ(z, vecZXYZ.w);
    ASSERT_EQ(z, vecZXYW.x);
    ASSERT_EQ(x, vecZXYW.y);
    ASSERT_EQ(y, vecZXYW.z);
    ASSERT_EQ(w, vecZXYW.w);
    ASSERT_EQ(z, vecZXZX.x);
    ASSERT_EQ(x, vecZXZX.y);
    ASSERT_EQ(z, vecZXZX.z);
    ASSERT_EQ(x, vecZXZX.w);
    ASSERT_EQ(z, vecZXZY.x);
    ASSERT_EQ(x, vecZXZY.y);
    ASSERT_EQ(z, vecZXZY.z);
    ASSERT_EQ(y, vecZXZY.w);
    ASSERT_EQ(z, vecZXZZ.x);
    ASSERT_EQ(x, vecZXZZ.y);
    ASSERT_EQ(z, vecZXZZ.z);
    ASSERT_EQ(z, vecZXZZ.w);
    ASSERT_EQ(z, vecZXZW.x);
    ASSERT_EQ(x, vecZXZW.y);
    ASSERT_EQ(z, vecZXZW.z);
    ASSERT_EQ(w, vecZXZW.w);
    ASSERT_EQ(z, vecZXWX.x);
    ASSERT_EQ(x, vecZXWX.y);
    ASSERT_EQ(w, vecZXWX.z);
    ASSERT_EQ(x, vecZXWX.w);
    ASSERT_EQ(z, vecZXWY.x);
    ASSERT_EQ(x, vecZXWY.y);
    ASSERT_EQ(w, vecZXWY.z);
    ASSERT_EQ(y, vecZXWY.w);
    ASSERT_EQ(z, vecZXWZ.x);
    ASSERT_EQ(x, vecZXWZ.y);
    ASSERT_EQ(w, vecZXWZ.z);
    ASSERT_EQ(z, vecZXWZ.w);
    ASSERT_EQ(z, vecZXWW.x);
    ASSERT_EQ(x, vecZXWW.y);
    ASSERT_EQ(w, vecZXWW.z);
    ASSERT_EQ(w, vecZXWW.w);
    ASSERT_EQ(z, vecZYXX.x);
    ASSERT_EQ(y, vecZYXX.y);
    ASSERT_EQ(x, vecZYXX.z);
    ASSERT_EQ(x, vecZYXX.w);
    ASSERT_EQ(z, vecZYXY.x);
    ASSERT_EQ(y, vecZYXY.y);
    ASSERT_EQ(x, vecZYXY.z);
    ASSERT_EQ(y, vecZYXY.w);
    ASSERT_EQ(z, vecZYXZ.x);
    ASSERT_EQ(y, vecZYXZ.y);
    ASSERT_EQ(x, vecZYXZ.z);
    ASSERT_EQ(z, vecZYXZ.w);
    ASSERT_EQ(z, vecZYXW.x);
    ASSERT_EQ(y, vecZYXW.y);
    ASSERT_EQ(x, vecZYXW.z);
    ASSERT_EQ(w, vecZYXW.w);
    ASSERT_EQ(z, vecZYYX.x);
    ASSERT_EQ(y, vecZYYX.y);
    ASSERT_EQ(y, vecZYYX.z);
    ASSERT_EQ(x, vecZYYX.w);
    ASSERT_EQ(z, vecZYYY.x);
    ASSERT_EQ(y, vecZYYY.y);
    ASSERT_EQ(y, vecZYYY.z);
    ASSERT_EQ(y, vecZYYY.w);
    ASSERT_EQ(z, vecZYYZ.x);
    ASSERT_EQ(y, vecZYYZ.y);
    ASSERT_EQ(y, vecZYYZ.z);
    ASSERT_EQ(z, vecZYYZ.w);
    ASSERT_EQ(z, vecZYYW.x);
    ASSERT_EQ(y, vecZYYW.y);
    ASSERT_EQ(y, vecZYYW.z);
    ASSERT_EQ(w, vecZYYW.w);
    ASSERT_EQ(z, vecZYZX.x);
    ASSERT_EQ(y, vecZYZX.y);
    ASSERT_EQ(z, vecZYZX.z);
    ASSERT_EQ(x, vecZYZX.w);
    ASSERT_EQ(z, vecZYZY.x);
    ASSERT_EQ(y, vecZYZY.y);
    ASSERT_EQ(z, vecZYZY.z);
    ASSERT_EQ(y, vecZYZY.w);
    ASSERT_EQ(z, vecZYZZ.x);
    ASSERT_EQ(y, vecZYZZ.y);
    ASSERT_EQ(z, vecZYZZ.z);
    ASSERT_EQ(z, vecZYZZ.w);
    ASSERT_EQ(z, vecZYZW.x);
    ASSERT_EQ(y, vecZYZW.y);
    ASSERT_EQ(z, vecZYZW.z);
    ASSERT_EQ(w, vecZYZW.w);
    ASSERT_EQ(z, vecZYWX.x);
    ASSERT_EQ(y, vecZYWX.y);
    ASSERT_EQ(w, vecZYWX.z);
    ASSERT_EQ(x, vecZYWX.w);
    ASSERT_EQ(z, vecZYWY.x);
    ASSERT_EQ(y, vecZYWY.y);
    ASSERT_EQ(w, vecZYWY.z);
    ASSERT_EQ(y, vecZYWY.w);
    ASSERT_EQ(z, vecZYWZ.x);
    ASSERT_EQ(y, vecZYWZ.y);
    ASSERT_EQ(w, vecZYWZ.z);
    ASSERT_EQ(z, vecZYWZ.w);
    ASSERT_EQ(z, vecZYWW.x);
    ASSERT_EQ(y, vecZYWW.y);
    ASSERT_EQ(w, vecZYWW.z);
    ASSERT_EQ(w, vecZYWW.w);
    ASSERT_EQ(z, vecZZXX.x);
    ASSERT_EQ(z, vecZZXX.y);
    ASSERT_EQ(x, vecZZXX.z);
    ASSERT_EQ(x, vecZZXX.w);
    ASSERT_EQ(z, vecZZXY.x);
    ASSERT_EQ(z, vecZZXY.y);
    ASSERT_EQ(x, vecZZXY.z);
    ASSERT_EQ(y, vecZZXY.w);
    ASSERT_EQ(z, vecZZXZ.x);
    ASSERT_EQ(z, vecZZXZ.y);
    ASSERT_EQ(x, vecZZXZ.z);
    ASSERT_EQ(z, vecZZXZ.w);
    ASSERT_EQ(z, vecZZXW.x);
    ASSERT_EQ(z, vecZZXW.y);
    ASSERT_EQ(x, vecZZXW.z);
    ASSERT_EQ(w, vecZZXW.w);
    ASSERT_EQ(z, vecZZYX.x);
    ASSERT_EQ(z, vecZZYX.y);
    ASSERT_EQ(y, vecZZYX.z);
    ASSERT_EQ(x, vecZZYX.w);
    ASSERT_EQ(z, vecZZYY.x);
    ASSERT_EQ(z, vecZZYY.y);
    ASSERT_EQ(y, vecZZYY.z);
    ASSERT_EQ(y, vecZZYY.w);
    ASSERT_EQ(z, vecZZYZ.x);
    ASSERT_EQ(z, vecZZYZ.y);
    ASSERT_EQ(y, vecZZYZ.z);
    ASSERT_EQ(z, vecZZYZ.w);
    ASSERT_EQ(z, vecZZYW.x);
    ASSERT_EQ(z, vecZZYW.y);
    ASSERT_EQ(y, vecZZYW.z);
    ASSERT_EQ(w, vecZZYW.w);
    ASSERT_EQ(z, vecZZZX.x);
    ASSERT_EQ(z, vecZZZX.y);
    ASSERT_EQ(z, vecZZZX.z);
    ASSERT_EQ(x, vecZZZX.w);
    ASSERT_EQ(z, vecZZZY.x);
    ASSERT_EQ(z, vecZZZY.y);
    ASSERT_EQ(z, vecZZZY.z);
    ASSERT_EQ(y, vecZZZY.w);
    ASSERT_EQ(z, vecZZZZ.x);
    ASSERT_EQ(z, vecZZZZ.y);
    ASSERT_EQ(z, vecZZZZ.z);
    ASSERT_EQ(z, vecZZZZ.w);
    ASSERT_EQ(z, vecZZZW.x);
    ASSERT_EQ(z, vecZZZW.y);
    ASSERT_EQ(z, vecZZZW.z);
    ASSERT_EQ(w, vecZZZW.w);
    ASSERT_EQ(z, vecZZWX.x);
    ASSERT_EQ(z, vecZZWX.y);
    ASSERT_EQ(w, vecZZWX.z);
    ASSERT_EQ(x, vecZZWX.w);
    ASSERT_EQ(z, vecZZWY.x);
    ASSERT_EQ(z, vecZZWY.y);
    ASSERT_EQ(w, vecZZWY.z);
    ASSERT_EQ(y, vecZZWY.w);
    ASSERT_EQ(z, vecZZWZ.x);
    ASSERT_EQ(z, vecZZWZ.y);
    ASSERT_EQ(w, vecZZWZ.z);
    ASSERT_EQ(z, vecZZWZ.w);
    ASSERT_EQ(z, vecZZWW.x);
    ASSERT_EQ(z, vecZZWW.y);
    ASSERT_EQ(w, vecZZWW.z);
    ASSERT_EQ(w, vecZZWW.w);
    ASSERT_EQ(z, vecZWXX.x);
    ASSERT_EQ(w, vecZWXX.y);
    ASSERT_EQ(x, vecZWXX.z);
    ASSERT_EQ(x, vecZWXX.w);
    ASSERT_EQ(z, vecZWXY.x);
    ASSERT_EQ(w, vecZWXY.y);
    ASSERT_EQ(x, vecZWXY.z);
    ASSERT_EQ(y, vecZWXY.w);
    ASSERT_EQ(z, vecZWXZ.x);
    ASSERT_EQ(w, vecZWXZ.y);
    ASSERT_EQ(x, vecZWXZ.z);
    ASSERT_EQ(z, vecZWXZ.w);
    ASSERT_EQ(z, vecZWXW.x);
    ASSERT_EQ(w, vecZWXW.y);
    ASSERT_EQ(x, vecZWXW.z);
    ASSERT_EQ(w, vecZWXW.w);
    ASSERT_EQ(z, vecZWYX.x);
    ASSERT_EQ(w, vecZWYX.y);
    ASSERT_EQ(y, vecZWYX.z);
    ASSERT_EQ(x, vecZWYX.w);
    ASSERT_EQ(z, vecZWYY.x);
    ASSERT_EQ(w, vecZWYY.y);
    ASSERT_EQ(y, vecZWYY.z);
    ASSERT_EQ(y, vecZWYY.w);
    ASSERT_EQ(z, vecZWYZ.x);
    ASSERT_EQ(w, vecZWYZ.y);
    ASSERT_EQ(y, vecZWYZ.z);
    ASSERT_EQ(z, vecZWYZ.w);
    ASSERT_EQ(z, vecZWYW.x);
    ASSERT_EQ(w, vecZWYW.y);
    ASSERT_EQ(y, vecZWYW.z);
    ASSERT_EQ(w, vecZWYW.w);
    ASSERT_EQ(z, vecZWZX.x);
    ASSERT_EQ(w, vecZWZX.y);
    ASSERT_EQ(z, vecZWZX.z);
    ASSERT_EQ(x, vecZWZX.w);
    ASSERT_EQ(z, vecZWZY.x);
    ASSERT_EQ(w, vecZWZY.y);
    ASSERT_EQ(z, vecZWZY.z);
    ASSERT_EQ(y, vecZWZY.w);
    ASSERT_EQ(z, vecZWZZ.x);
    ASSERT_EQ(w, vecZWZZ.y);
    ASSERT_EQ(z, vecZWZZ.z);
    ASSERT_EQ(z, vecZWZZ.w);
    ASSERT_EQ(z, vecZWZW.x);
    ASSERT_EQ(w, vecZWZW.y);
    ASSERT_EQ(z, vecZWZW.z);
    ASSERT_EQ(w, vecZWZW.w);
    ASSERT_EQ(z, vecZWWX.x);
    ASSERT_EQ(w, vecZWWX.y);
    ASSERT_EQ(w, vecZWWX.z);
    ASSERT_EQ(x, vecZWWX.w);
    ASSERT_EQ(z, vecZWWY.x);
    ASSERT_EQ(w, vecZWWY.y);
    ASSERT_EQ(w, vecZWWY.z);
    ASSERT_EQ(y, vecZWWY.w);
    ASSERT_EQ(z, vecZWWZ.x);
    ASSERT_EQ(w, vecZWWZ.y);
    ASSERT_EQ(w, vecZWWZ.z);
    ASSERT_EQ(z, vecZWWZ.w);
    ASSERT_EQ(z, vecZWWW.x);
    ASSERT_EQ(w, vecZWWW.y);
    ASSERT_EQ(w, vecZWWW.z);
    ASSERT_EQ(w, vecZWWW.w);
    ASSERT_EQ(w, vecWXXX.x);
    ASSERT_EQ(x, vecWXXX.y);
    ASSERT_EQ(x, vecWXXX.z);
    ASSERT_EQ(x, vecWXXX.w);
    ASSERT_EQ(w, vecWXXY.x);
    ASSERT_EQ(x, vecWXXY.y);
    ASSERT_EQ(x, vecWXXY.z);
    ASSERT_EQ(y, vecWXXY.w);
    ASSERT_EQ(w, vecWXXZ.x);
    ASSERT_EQ(x, vecWXXZ.y);
    ASSERT_EQ(x, vecWXXZ.z);
    ASSERT_EQ(z, vecWXXZ.w);
    ASSERT_EQ(w, vecWXXW.x);
    ASSERT_EQ(x, vecWXXW.y);
    ASSERT_EQ(x, vecWXXW.z);
    ASSERT_EQ(w, vecWXXW.w);
    ASSERT_EQ(w, vecWXYX.x);
    ASSERT_EQ(x, vecWXYX.y);
    ASSERT_EQ(y, vecWXYX.z);
    ASSERT_EQ(x, vecWXYX.w);
    ASSERT_EQ(w, vecWXYY.x);
    ASSERT_EQ(x, vecWXYY.y);
    ASSERT_EQ(y, vecWXYY.z);
    ASSERT_EQ(y, vecWXYY.w);
    ASSERT_EQ(w, vecWXYZ.x);
    ASSERT_EQ(x, vecWXYZ.y);
    ASSERT_EQ(y, vecWXYZ.z);
    ASSERT_EQ(z, vecWXYZ.w);
    ASSERT_EQ(w, vecWXYW.x);
    ASSERT_EQ(x, vecWXYW.y);
    ASSERT_EQ(y, vecWXYW.z);
    ASSERT_EQ(w, vecWXYW.w);
    ASSERT_EQ(w, vecWXZX.x);
    ASSERT_EQ(x, vecWXZX.y);
    ASSERT_EQ(z, vecWXZX.z);
    ASSERT_EQ(x, vecWXZX.w);
    ASSERT_EQ(w, vecWXZY.x);
    ASSERT_EQ(x, vecWXZY.y);
    ASSERT_EQ(z, vecWXZY.z);
    ASSERT_EQ(y, vecWXZY.w);
    ASSERT_EQ(w, vecWXZZ.x);
    ASSERT_EQ(x, vecWXZZ.y);
    ASSERT_EQ(z, vecWXZZ.z);
    ASSERT_EQ(z, vecWXZZ.w);
    ASSERT_EQ(w, vecWXZW.x);
    ASSERT_EQ(x, vecWXZW.y);
    ASSERT_EQ(z, vecWXZW.z);
    ASSERT_EQ(w, vecWXZW.w);
    ASSERT_EQ(w, vecWXWX.x);
    ASSERT_EQ(x, vecWXWX.y);
    ASSERT_EQ(w, vecWXWX.z);
    ASSERT_EQ(x, vecWXWX.w);
    ASSERT_EQ(w, vecWXWY.x);
    ASSERT_EQ(x, vecWXWY.y);
    ASSERT_EQ(w, vecWXWY.z);
    ASSERT_EQ(y, vecWXWY.w);
    ASSERT_EQ(w, vecWXWZ.x);
    ASSERT_EQ(x, vecWXWZ.y);
    ASSERT_EQ(w, vecWXWZ.z);
    ASSERT_EQ(z, vecWXWZ.w);
    ASSERT_EQ(w, vecWXWW.x);
    ASSERT_EQ(x, vecWXWW.y);
    ASSERT_EQ(w, vecWXWW.z);
    ASSERT_EQ(w, vecWXWW.w);
    ASSERT_EQ(w, vecWYXX.x);
    ASSERT_EQ(y, vecWYXX.y);
    ASSERT_EQ(x, vecWYXX.z);
    ASSERT_EQ(x, vecWYXX.w);
    ASSERT_EQ(w, vecWYXY.x);
    ASSERT_EQ(y, vecWYXY.y);
    ASSERT_EQ(x, vecWYXY.z);
    ASSERT_EQ(y, vecWYXY.w);
    ASSERT_EQ(w, vecWYXZ.x);
    ASSERT_EQ(y, vecWYXZ.y);
    ASSERT_EQ(x, vecWYXZ.z);
    ASSERT_EQ(z, vecWYXZ.w);
    ASSERT_EQ(w, vecWYXW.x);
    ASSERT_EQ(y, vecWYXW.y);
    ASSERT_EQ(x, vecWYXW.z);
    ASSERT_EQ(w, vecWYXW.w);
    ASSERT_EQ(w, vecWYYX.x);
    ASSERT_EQ(y, vecWYYX.y);
    ASSERT_EQ(y, vecWYYX.z);
    ASSERT_EQ(x, vecWYYX.w);
    ASSERT_EQ(w, vecWYYY.x);
    ASSERT_EQ(y, vecWYYY.y);
    ASSERT_EQ(y, vecWYYY.z);
    ASSERT_EQ(y, vecWYYY.w);
    ASSERT_EQ(w, vecWYYZ.x);
    ASSERT_EQ(y, vecWYYZ.y);
    ASSERT_EQ(y, vecWYYZ.z);
    ASSERT_EQ(z, vecWYYZ.w);
    ASSERT_EQ(w, vecWYYW.x);
    ASSERT_EQ(y, vecWYYW.y);
    ASSERT_EQ(y, vecWYYW.z);
    ASSERT_EQ(w, vecWYYW.w);
    ASSERT_EQ(w, vecWYZX.x);
    ASSERT_EQ(y, vecWYZX.y);
    ASSERT_EQ(z, vecWYZX.z);
    ASSERT_EQ(x, vecWYZX.w);
    ASSERT_EQ(w, vecWYZY.x);
    ASSERT_EQ(y, vecWYZY.y);
    ASSERT_EQ(z, vecWYZY.z);
    ASSERT_EQ(y, vecWYZY.w);
    ASSERT_EQ(w, vecWYZZ.x);
    ASSERT_EQ(y, vecWYZZ.y);
    ASSERT_EQ(z, vecWYZZ.z);
    ASSERT_EQ(z, vecWYZZ.w);
    ASSERT_EQ(w, vecWYZW.x);
    ASSERT_EQ(y, vecWYZW.y);
    ASSERT_EQ(z, vecWYZW.z);
    ASSERT_EQ(w, vecWYZW.w);
    ASSERT_EQ(w, vecWYWX.x);
    ASSERT_EQ(y, vecWYWX.y);
    ASSERT_EQ(w, vecWYWX.z);
    ASSERT_EQ(x, vecWYWX.w);
    ASSERT_EQ(w, vecWYWY.x);
    ASSERT_EQ(y, vecWYWY.y);
    ASSERT_EQ(w, vecWYWY.z);
    ASSERT_EQ(y, vecWYWY.w);
    ASSERT_EQ(w, vecWYWZ.x);
    ASSERT_EQ(y, vecWYWZ.y);
    ASSERT_EQ(w, vecWYWZ.z);
    ASSERT_EQ(z, vecWYWZ.w);
    ASSERT_EQ(w, vecWYWW.x);
    ASSERT_EQ(y, vecWYWW.y);
    ASSERT_EQ(w, vecWYWW.z);
    ASSERT_EQ(w, vecWYWW.w);
    ASSERT_EQ(w, vecWZXX.x);
    ASSERT_EQ(z, vecWZXX.y);
    ASSERT_EQ(x, vecWZXX.z);
    ASSERT_EQ(x, vecWZXX.w);
    ASSERT_EQ(w, vecWZXY.x);
    ASSERT_EQ(z, vecWZXY.y);
    ASSERT_EQ(x, vecWZXY.z);
    ASSERT_EQ(y, vecWZXY.w);
    ASSERT_EQ(w, vecWZXZ.x);
    ASSERT_EQ(z, vecWZXZ.y);
    ASSERT_EQ(x, vecWZXZ.z);
    ASSERT_EQ(z, vecWZXZ.w);
    ASSERT_EQ(w, vecWZXW.x);
    ASSERT_EQ(z, vecWZXW.y);
    ASSERT_EQ(x, vecWZXW.z);
    ASSERT_EQ(w, vecWZXW.w);
    ASSERT_EQ(w, vecWZYX.x);
    ASSERT_EQ(z, vecWZYX.y);
    ASSERT_EQ(y, vecWZYX.z);
    ASSERT_EQ(x, vecWZYX.w);
    ASSERT_EQ(w, vecWZYY.x);
    ASSERT_EQ(z, vecWZYY.y);
    ASSERT_EQ(y, vecWZYY.z);
    ASSERT_EQ(y, vecWZYY.w);
    ASSERT_EQ(w, vecWZYZ.x);
    ASSERT_EQ(z, vecWZYZ.y);
    ASSERT_EQ(y, vecWZYZ.z);
    ASSERT_EQ(z, vecWZYZ.w);
    ASSERT_EQ(w, vecWZYW.x);
    ASSERT_EQ(z, vecWZYW.y);
    ASSERT_EQ(y, vecWZYW.z);
    ASSERT_EQ(w, vecWZYW.w);
    ASSERT_EQ(w, vecWZZX.x);
    ASSERT_EQ(z, vecWZZX.y);
    ASSERT_EQ(z, vecWZZX.z);
    ASSERT_EQ(x, vecWZZX.w);
    ASSERT_EQ(w, vecWZZY.x);
    ASSERT_EQ(z, vecWZZY.y);
    ASSERT_EQ(z, vecWZZY.z);
    ASSERT_EQ(y, vecWZZY.w);
    ASSERT_EQ(w, vecWZZZ.x);
    ASSERT_EQ(z, vecWZZZ.y);
    ASSERT_EQ(z, vecWZZZ.z);
    ASSERT_EQ(z, vecWZZZ.w);
    ASSERT_EQ(w, vecWZZW.x);
    ASSERT_EQ(z, vecWZZW.y);
    ASSERT_EQ(z, vecWZZW.z);
    ASSERT_EQ(w, vecWZZW.w);
    ASSERT_EQ(w, vecWZWX.x);
    ASSERT_EQ(z, vecWZWX.y);
    ASSERT_EQ(w, vecWZWX.z);
    ASSERT_EQ(x, vecWZWX.w);
    ASSERT_EQ(w, vecWZWY.x);
    ASSERT_EQ(z, vecWZWY.y);
    ASSERT_EQ(w, vecWZWY.z);
    ASSERT_EQ(y, vecWZWY.w);
    ASSERT_EQ(w, vecWZWZ.x);
    ASSERT_EQ(z, vecWZWZ.y);
    ASSERT_EQ(w, vecWZWZ.z);
    ASSERT_EQ(z, vecWZWZ.w);
    ASSERT_EQ(w, vecWZWW.x);
    ASSERT_EQ(z, vecWZWW.y);
    ASSERT_EQ(w, vecWZWW.z);
    ASSERT_EQ(w, vecWZWW.w);
    ASSERT_EQ(w, vecWWXX.x);
    ASSERT_EQ(w, vecWWXX.y);
    ASSERT_EQ(x, vecWWXX.z);
    ASSERT_EQ(x, vecWWXX.w);
    ASSERT_EQ(w, vecWWXY.x);
    ASSERT_EQ(w, vecWWXY.y);
    ASSERT_EQ(x, vecWWXY.z);
    ASSERT_EQ(y, vecWWXY.w);
    ASSERT_EQ(w, vecWWXZ.x);
    ASSERT_EQ(w, vecWWXZ.y);
    ASSERT_EQ(x, vecWWXZ.z);
    ASSERT_EQ(z, vecWWXZ.w);
    ASSERT_EQ(w, vecWWXW.x);
    ASSERT_EQ(w, vecWWXW.y);
    ASSERT_EQ(x, vecWWXW.z);
    ASSERT_EQ(w, vecWWXW.w);
    ASSERT_EQ(w, vecWWYX.x);
    ASSERT_EQ(w, vecWWYX.y);
    ASSERT_EQ(y, vecWWYX.z);
    ASSERT_EQ(x, vecWWYX.w);
    ASSERT_EQ(w, vecWWYY.x);
    ASSERT_EQ(w, vecWWYY.y);
    ASSERT_EQ(y, vecWWYY.z);
    ASSERT_EQ(y, vecWWYY.w);
    ASSERT_EQ(w, vecWWYZ.x);
    ASSERT_EQ(w, vecWWYZ.y);
    ASSERT_EQ(y, vecWWYZ.z);
    ASSERT_EQ(z, vecWWYZ.w);
    ASSERT_EQ(w, vecWWYW.x);
    ASSERT_EQ(w, vecWWYW.y);
    ASSERT_EQ(y, vecWWYW.z);
    ASSERT_EQ(w, vecWWYW.w);
    ASSERT_EQ(w, vecWWZX.x);
    ASSERT_EQ(w, vecWWZX.y);
    ASSERT_EQ(z, vecWWZX.z);
    ASSERT_EQ(x, vecWWZX.w);
    ASSERT_EQ(w, vecWWZY.x);
    ASSERT_EQ(w, vecWWZY.y);
    ASSERT_EQ(z, vecWWZY.z);
    ASSERT_EQ(y, vecWWZY.w);
    ASSERT_EQ(w, vecWWZZ.x);
    ASSERT_EQ(w, vecWWZZ.y);
    ASSERT_EQ(z, vecWWZZ.z);
    ASSERT_EQ(z, vecWWZZ.w);
    ASSERT_EQ(w, vecWWZW.x);
    ASSERT_EQ(w, vecWWZW.y);
    ASSERT_EQ(z, vecWWZW.z);
    ASSERT_EQ(w, vecWWZW.w);
    ASSERT_EQ(w, vecWWWX.x);
    ASSERT_EQ(w, vecWWWX.y);
    ASSERT_EQ(w, vecWWWX.z);
    ASSERT_EQ(x, vecWWWX.w);
    ASSERT_EQ(w, vecWWWY.x);
    ASSERT_EQ(w, vecWWWY.y);
    ASSERT_EQ(w, vecWWWY.z);
    ASSERT_EQ(y, vecWWWY.w);
    ASSERT_EQ(w, vecWWWZ.x);
    ASSERT_EQ(w, vecWWWZ.y);
    ASSERT_EQ(w, vecWWWZ.z);
    ASSERT_EQ(z, vecWWWZ.w);
    ASSERT_EQ(w, vecWWWW.x);
    ASSERT_EQ(w, vecWWWW.y);
    ASSERT_EQ(w, vecWWWW.z);
    ASSERT_EQ(w, vecWWWW.w);
}

UTEST(test_Vec4, TestToString)
{
    String expected = "1.00,2.00,3.00,4.00";

    Vec4 vec = {1.f, 2.f, 3.f, 4.f};

    String result = ToString(vec);

    ASSERT_TRUE(result == expected);
}

UTEST(test_Vec4, TestFromString)
{
    Vec4 expected = {1.f, 2.f, 3.f, 4.f};

    String inputString = "1.00,2.00,3.00,4.00";

    Vec4 result{};

    ASSERT_TRUE(FromString(result, inputString));

    ASSERT_TRUE(result == expected);
}

UTEST(test_Vec4, TestFuzzyEquals)
{
    Vec4 vecA = {1.f, 2.f, 3.f, 4.f};

    Vec4 vecB = {0.9999999999999912, 2.0000000000000023f, 3.f, 4.000000000000000042};

    ASSERT_TRUE(FEquals(vecA, vecB));
}

UTEST(test_Vec4, LerpVector)
{
    Vec4 expected = {1.5f, 1.5f, 1.5f, 1.5f};
    Vec4 vec = Vec4::One();

    ASSERT_TRUE(expected == Vec4::Lerp(vec, {2.f, 2.f, 2.f, 2.f}, .5f));
}