//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
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

#define ASSERT_EQ_VEC3(a, b)                                               \
    ASSERT_LE(std::abs(a.x - b.x), std::numeric_limits<float>::epsilon()); \
    ASSERT_LE(std::abs(a.y - b.y), std::numeric_limits<float>::epsilon()); \
    ASSERT_LE(std::abs(a.z - b.z), std::numeric_limits<float>::epsilon());

using namespace Siege;

UTEST(test_Vec3, CreateVec3)
{
    Vec3 vec {};

    ASSERT_EQ(0.f, vec.x);
    ASSERT_EQ(0.f, vec.y);
    ASSERT_EQ(0.f, vec.z);
}

UTEST(test_Vec3, CreateVec3WithValues)
{
    Vec3 vec {1.f, 2.f, 3.f};

    ASSERT_EQ(1.f, vec.x);
    ASSERT_EQ(2.f, vec.y);
    ASSERT_EQ(3.f, vec.z);
}

UTEST(test_Vec3, CreateVec3One)
{
    Vec3 vec = Vec3::One();

    ASSERT_EQ(1, vec.x);
    ASSERT_EQ(1, vec.y);
    ASSERT_EQ(1, vec.z);
}

UTEST(test_Vec3, CreateVec3UpVector)
{
    Vec3 vec = Vec3::Up();

    ASSERT_EQ(0, vec.x);
    ASSERT_EQ(-1, vec.y);
    ASSERT_EQ(0, vec.z);
}

UTEST(test_Vec3, CreateVec3RightVector)
{
    Vec3 vec = Vec3::Right();

    ASSERT_EQ(1, vec.x);
    ASSERT_EQ(0, vec.y);
    ASSERT_EQ(0, vec.z);
}

UTEST(test_Vec3, CreateVec3WithVec2)
{
    Vec3 expected = {1.f, 2.f, 0.f};

    Vec2 vec = {1.f, 2.f};

    Vec3 result = vec;

    ASSERT_EQ_VEC3(result, expected);
}

UTEST(test_Vec3, CreateVec3WithVec4)
{
    Vec3 expected = {1.f, 2.f, 3.f};

    Vec4 vec = {1.f, 2.f, 3.f, 4.f};

    Vec3 result = vec;

    ASSERT_EQ_VEC3(result, expected);
}

UTEST(test_Vec3, SubscriptOperator)
{
    Vec3 vec = {1.f, 2.f, 3.f};

    ASSERT_EQ(1.f, vec[0]);
    ASSERT_EQ(2.f, vec[1]);
    ASSERT_EQ(3.f, vec[2]);
}

UTEST(test_Vec3, SubscriptAssignmentOperator)
{
    Vec3 vec {};

    vec[0] = 2.f;
    vec[1] = 1.f;
    vec[2] = 3.f;

    ASSERT_EQ(2.f, vec[0]);
    ASSERT_EQ(1.f, vec[1]);
    ASSERT_EQ(3.f, vec[2]);
}

UTEST(test_Vec3, AddVectors)
{
    Vec3 expected = {4.f, 2.f, 6};

    Vec3 vecA = {2.f, 1.f, 3.f};

    Vec3 result = vecA + vecA;

    ASSERT_EQ_VEC3(result, expected);
}

UTEST(test_Vec3, AddScalar)
{
    Vec3 expected = {4.f, 2.f, 6.f};

    Vec3 vecA = {2.f, 0.f, 4.f};

    Vec3 result = vecA + 2.f;

    ASSERT_EQ_VEC3(result, expected);
}

UTEST(test_Vec3, SubtractVectors)
{
    Vec3 expected = {4.f, 2.f, 6.f};

    Vec3 vecA = {8.f, 4.f, 12.f};

    Vec3 vecB = {4.f, 2.f, 6.f};

    Vec3 result = vecA - vecB;

    ASSERT_EQ_VEC3(result, expected);
}

UTEST(test_Vec3, SubtractScalar)
{
    Vec3 expected = {4.f, 2.f, 6.f};

    Vec3 vecA = {6.f, 4.f, 8.f};

    Vec3 result = vecA - 2.f;

    ASSERT_EQ_VEC3(result, expected);
}

UTEST(test_Vec3, MultiplyVectors)
{
    Vec3 expected = {4.f, 2.f, 6.f};

    Vec3 vecA = {2.f, 1.f, 2.f};

    Vec3 vecB = {2.f, 2.f, 3.f};

    Vec3 result = vecA * vecB;

    ASSERT_EQ_VEC3(result, expected);
}

UTEST(test_Vec3, MultiplyScalar)
{
    Vec3 expected = {4.f, 2.f, 6.f};

    Vec3 vecA = {2.f, 1.f, 3.f};

    Vec3 result = vecA * 2.f;

    ASSERT_EQ_VEC3(result, expected);
}

UTEST(test_Vec3, DivideVectors)
{
    Vec3 expected = {4.f, 2.f, 6.f};

    Vec3 vecA = {8.f, 6.f, 12.f};

    Vec3 vecB = {2.f, 3.f, 2.f};

    Vec3 result = vecA / vecB;

    ASSERT_EQ_VEC3(result, expected);
}

UTEST(test_Vec3, DivideScalar)
{
    Vec3 expected = {4.f, 2.f, 6.f};

    Vec3 vecA = {8.f, 4.f, 12.f};

    Vec3 result = vecA / 2.f;

    ASSERT_EQ_VEC3(result, expected);
}

UTEST(test_Vec3, CaculateDotProduct)
{
    Vec3 vecA = {2, 3, 4};
    Vec3 vecB = {5, 6, 7};

    auto dot = Vec3::Dot(vecA, vecB);

    ASSERT_EQ(dot, 56.f);
}

UTEST(test_Vec3, CaculateCrossProduct)
{
    Vec3 vecA = {2, 3, 4};
    Vec3 vecB = {5, 6, 7};

    auto cross = Vec3::Cross(vecA, vecB);

    ASSERT_EQ(cross.x, -3.f);
    ASSERT_EQ(cross.y, 6.f);
    ASSERT_EQ(cross.z, -3.f);
}

UTEST(test_Vec3, GetVectorLength)
{
    Vec3 vec = {2, 3, 4};

    float length = Vec3::Length(vec);

    ASSERT_LE((length - 5.38516474f), std::numeric_limits<float>::epsilon());
}

UTEST(test_Vec3, NormaliseVector)
{
    Vec3 vec = {2, 3, 4};

    Vec3 norm = Vec3::Normalise(vec);

    ASSERT_LE((norm.x - 0.37181139f), std::numeric_limits<float>::epsilon());
    ASSERT_LE((norm.y - 0.557717085f), std::numeric_limits<float>::epsilon());
    ASSERT_LE((norm.z - 0.743622779f), std::numeric_limits<float>::epsilon());
}

UTEST(test_Vec3, Vec3CosOperation)
{
    Vec3 expected = {0.540302336f, -0.416146845, -0.989992499};

    Vec3 vec = {1.f, 2.f, 3.f};

    auto result = Vec3::Cos(vec);

    ASSERT_EQ_VEC3(result, expected);
}

UTEST(test_Vec3, Vec3SinOperation)
{
    Vec3 expected = {0.841471016f, 0.909297406, 0.141120002};

    Vec3 vec = {1.f, 2.f, 3.f};

    auto result = Vec3::Sin(vec);

    ASSERT_EQ_VEC3(result, expected);
}

UTEST(test_Vec3, Vec3TanOperation)
{
    Vec3 expected = {1.55740774f, -2.18503976, -0.142546549};

    Vec3 vec = {1.f, 2.f, 3.f};

    auto result = Vec3::Tan(vec);

    ASSERT_EQ_VEC3(result, expected);
}

UTEST(test_Vec3, SwizzleOperators)
{
    srand(time(NULL));
    float x = rand() % 1000;
    float y = rand() % 1000;
    float z = rand() % 1000;
    Vec3 vec = {x, y, z};

    auto vecXX = vec.XX();
    auto vecXY = vec.XY();
    auto vecXZ = vec.XZ();
    auto vecYX = vec.YX();
    auto vecYY = vec.YY();
    auto vecYZ = vec.YZ();
    auto vecZX = vec.ZX();
    auto vecZY = vec.ZY();
    auto vecZZ = vec.ZZ();

    ASSERT_EQ(x, vecXX.x);
    ASSERT_EQ(x, vecXX.y);
    ASSERT_EQ(x, vecXY.x);
    ASSERT_EQ(y, vecXY.y);
    ASSERT_EQ(x, vecXZ.x);
    ASSERT_EQ(z, vecXZ.y);
    ASSERT_EQ(y, vecYX.x);
    ASSERT_EQ(x, vecYX.y);
    ASSERT_EQ(y, vecYY.x);
    ASSERT_EQ(y, vecYY.y);
    ASSERT_EQ(y, vecYZ.x);
    ASSERT_EQ(z, vecYZ.y);
    ASSERT_EQ(z, vecZX.x);
    ASSERT_EQ(x, vecZX.y);
    ASSERT_EQ(z, vecZY.x);
    ASSERT_EQ(y, vecZY.y);
    ASSERT_EQ(z, vecZZ.x);
    ASSERT_EQ(z, vecZZ.y);

    auto vecXXX = vec.XXX();
    auto vecXXY = vec.XXY();
    auto vecXXZ = vec.XXZ();
    auto vecXYX = vec.XYX();
    auto vecXYY = vec.XYY();
    auto vecXYZ = vec.XYZ();
    auto vecXZX = vec.XZX();
    auto vecXZY = vec.XZY();
    auto vecXZZ = vec.XZZ();
    auto vecYXX = vec.YXX();
    auto vecYXY = vec.YXY();
    auto vecYXZ = vec.YXZ();
    auto vecYYX = vec.YYX();
    auto vecYYY = vec.YYY();
    auto vecYYZ = vec.YYZ();
    auto vecYZX = vec.YZX();
    auto vecYZY = vec.YZY();
    auto vecYZZ = vec.YZZ();
    auto vecZXX = vec.ZXX();
    auto vecZXY = vec.ZXY();
    auto vecZXZ = vec.ZXZ();
    auto vecZYX = vec.ZYX();
    auto vecZYY = vec.ZYY();
    auto vecZYZ = vec.ZYZ();
    auto vecZZX = vec.ZZX();
    auto vecZZY = vec.ZZY();
    auto vecZZZ = vec.ZZZ();

    ASSERT_EQ(x, vecXXX.x);
    ASSERT_EQ(x, vecXXX.y);
    ASSERT_EQ(x, vecXXX.z);
    ASSERT_EQ(x, vecXXY.x);
    ASSERT_EQ(x, vecXXY.y);
    ASSERT_EQ(y, vecXXY.z);
    ASSERT_EQ(x, vecXXZ.x);
    ASSERT_EQ(x, vecXXZ.y);
    ASSERT_EQ(z, vecXXZ.z);
    ASSERT_EQ(x, vecXYX.x);
    ASSERT_EQ(y, vecXYX.y);
    ASSERT_EQ(x, vecXYX.z);
    ASSERT_EQ(x, vecXYY.x);
    ASSERT_EQ(y, vecXYY.y);
    ASSERT_EQ(y, vecXYY.z);
    ASSERT_EQ(x, vecXYZ.x);
    ASSERT_EQ(y, vecXYZ.y);
    ASSERT_EQ(z, vecXYZ.z);
    ASSERT_EQ(x, vecXZX.x);
    ASSERT_EQ(z, vecXZX.y);
    ASSERT_EQ(x, vecXZX.z);
    ASSERT_EQ(x, vecXZY.x);
    ASSERT_EQ(z, vecXZY.y);
    ASSERT_EQ(y, vecXZY.z);
    ASSERT_EQ(x, vecXZZ.x);
    ASSERT_EQ(z, vecXZZ.y);
    ASSERT_EQ(z, vecXZZ.z);
    ASSERT_EQ(y, vecYXX.x);
    ASSERT_EQ(x, vecYXX.y);
    ASSERT_EQ(x, vecYXX.z);
    ASSERT_EQ(y, vecYXY.x);
    ASSERT_EQ(x, vecYXY.y);
    ASSERT_EQ(y, vecYXY.z);
    ASSERT_EQ(y, vecYXZ.x);
    ASSERT_EQ(x, vecYXZ.y);
    ASSERT_EQ(z, vecYXZ.z);
    ASSERT_EQ(y, vecYYX.x);
    ASSERT_EQ(y, vecYYX.y);
    ASSERT_EQ(x, vecYYX.z);
    ASSERT_EQ(y, vecYYY.x);
    ASSERT_EQ(y, vecYYY.y);
    ASSERT_EQ(y, vecYYY.z);
    ASSERT_EQ(y, vecYYZ.x);
    ASSERT_EQ(y, vecYYZ.y);
    ASSERT_EQ(z, vecYYZ.z);
    ASSERT_EQ(y, vecYZX.x);
    ASSERT_EQ(z, vecYZX.y);
    ASSERT_EQ(x, vecYZX.z);
    ASSERT_EQ(y, vecYZY.x);
    ASSERT_EQ(z, vecYZY.y);
    ASSERT_EQ(y, vecYZY.z);
    ASSERT_EQ(y, vecYZZ.x);
    ASSERT_EQ(z, vecYZZ.y);
    ASSERT_EQ(z, vecYZZ.z);
    ASSERT_EQ(z, vecZXX.x);
    ASSERT_EQ(x, vecZXX.y);
    ASSERT_EQ(x, vecZXX.z);
    ASSERT_EQ(z, vecZXY.x);
    ASSERT_EQ(x, vecZXY.y);
    ASSERT_EQ(y, vecZXY.z);
    ASSERT_EQ(z, vecZXZ.x);
    ASSERT_EQ(x, vecZXZ.y);
    ASSERT_EQ(z, vecZXZ.z);
    ASSERT_EQ(z, vecZYX.x);
    ASSERT_EQ(y, vecZYX.y);
    ASSERT_EQ(x, vecZYX.z);
    ASSERT_EQ(z, vecZYY.x);
    ASSERT_EQ(y, vecZYY.y);
    ASSERT_EQ(y, vecZYY.z);
    ASSERT_EQ(z, vecZYZ.x);
    ASSERT_EQ(y, vecZYZ.y);
    ASSERT_EQ(z, vecZYZ.z);
    ASSERT_EQ(z, vecZZX.x);
    ASSERT_EQ(z, vecZZX.y);
    ASSERT_EQ(x, vecZZX.z);
    ASSERT_EQ(z, vecZZY.x);
    ASSERT_EQ(z, vecZZY.y);
    ASSERT_EQ(y, vecZZY.z);
    ASSERT_EQ(z, vecZZZ.x);
    ASSERT_EQ(z, vecZZZ.y);
    ASSERT_EQ(z, vecZZZ.z);

    auto vecXXXX = vec.XXXX();
    auto vecXXXY = vec.XXXY();
    auto vecXXXZ = vec.XXXZ();
    auto vecXXYX = vec.XXYX();
    auto vecXXYY = vec.XXYY();
    auto vecXXYZ = vec.XXYZ();
    auto vecXXZX = vec.XXZX();
    auto vecXXZY = vec.XXZY();
    auto vecXXZZ = vec.XXZZ();
    auto vecXYXX = vec.XYXX();
    auto vecXYXY = vec.XYXY();
    auto vecXYXZ = vec.XYXZ();
    auto vecXYYX = vec.XYYX();
    auto vecXYYY = vec.XYYY();
    auto vecXYYZ = vec.XYYZ();
    auto vecXYZX = vec.XYZX();
    auto vecXYZY = vec.XYZY();
    auto vecXYZZ = vec.XYZZ();
    auto vecXZXX = vec.XZXX();
    auto vecXZXY = vec.XZXY();
    auto vecXZXZ = vec.XZXZ();
    auto vecXZYX = vec.XZYX();
    auto vecXZYY = vec.XZYY();
    auto vecXZYZ = vec.XZYZ();
    auto vecXZZX = vec.XZZX();
    auto vecXZZY = vec.XZZY();
    auto vecXZZZ = vec.XZZZ();
    auto vecYXXX = vec.YXXX();
    auto vecYXXY = vec.YXXY();
    auto vecYXXZ = vec.YXXZ();
    auto vecYXYX = vec.YXYX();
    auto vecYXYY = vec.YXYY();
    auto vecYXYZ = vec.YXYZ();
    auto vecYXZX = vec.YXZX();
    auto vecYXZY = vec.YXZY();
    auto vecYXZZ = vec.YXZZ();
    auto vecYYXX = vec.YYXX();
    auto vecYYXY = vec.YYXY();
    auto vecYYXZ = vec.YYXZ();
    auto vecYYYX = vec.YYYX();
    auto vecYYYY = vec.YYYY();
    auto vecYYYZ = vec.YYYZ();
    auto vecYYZX = vec.YYZX();
    auto vecYYZY = vec.YYZY();
    auto vecYYZZ = vec.YYZZ();
    auto vecYZXX = vec.YZXX();
    auto vecYZXY = vec.YZXY();
    auto vecYZXZ = vec.YZXZ();
    auto vecYZYX = vec.YZYX();
    auto vecYZYY = vec.YZYY();
    auto vecYZYZ = vec.YZYZ();
    auto vecYZZX = vec.YZZX();
    auto vecYZZY = vec.YZZY();
    auto vecYZZZ = vec.YZZZ();
    auto vecZXXX = vec.ZXXX();
    auto vecZXXY = vec.ZXXY();
    auto vecZXXZ = vec.ZXXZ();
    auto vecZXYX = vec.ZXYX();
    auto vecZXYY = vec.ZXYY();
    auto vecZXYZ = vec.ZXYZ();
    auto vecZXZX = vec.ZXZX();
    auto vecZXZY = vec.ZXZY();
    auto vecZXZZ = vec.ZXZZ();
    auto vecZYXX = vec.ZYXX();
    auto vecZYXY = vec.ZYXY();
    auto vecZYXZ = vec.ZYXZ();
    auto vecZYYX = vec.ZYYX();
    auto vecZYYY = vec.ZYYY();
    auto vecZYYZ = vec.ZYYZ();
    auto vecZYZX = vec.ZYZX();
    auto vecZYZY = vec.ZYZY();
    auto vecZYZZ = vec.ZYZZ();
    auto vecZZXX = vec.ZZXX();
    auto vecZZXY = vec.ZZXY();
    auto vecZZXZ = vec.ZZXZ();
    auto vecZZYX = vec.ZZYX();
    auto vecZZYY = vec.ZZYY();
    auto vecZZYZ = vec.ZZYZ();
    auto vecZZZX = vec.ZZZX();
    auto vecZZZY = vec.ZZZY();
    auto vecZZZZ = vec.ZZZZ();

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
}

UTEST(test_Vec3, ToString)
{
    String expected = "1.00,2.00,3.00";

    Vec3 vec = {1.f, 2.f, 3.f};

    String result = ToString(vec);

    ASSERT_STREQ(result.Str(), expected.Str());
}

UTEST(test_Vec3, FromString)
{
    Vec3 expected = {1.f, 2.f, 3.f};

    String inputString = "1.00,2.00,3.00";

    Vec3 result {};

    ASSERT_TRUE(FromString(result, inputString));

    ASSERT_EQ_VEC3(result, expected);
}

UTEST(test_Vec3, CheckVectorEquality)
{
    Vec3 vecA = {1.f, 2.f, 3.f};

    Vec3 vecB = {0.9999999999999912, 2.0000000000000023f, 3.f};

    ASSERT_TRUE(Equals(vecA, vecB, Epsilon<float>()));

    vecA = {-1.f, -2.f, -3.f};

    vecB = {-0.9999999999999912, -2.0000000000000023f, -3.f};

    ASSERT_TRUE(Equals(vecA, vecB, Epsilon<float>()));
}

UTEST(test_Vec3, LerpVector)
{
    Vec3 expected = {1.5f, 1.5f, 1.5f};
    Vec3 vec = Vec3::One();

    ASSERT_EQ_VEC3(expected, Vec3::Lerp(vec, {2.f, 2.f, 2.f}, .5f));
}