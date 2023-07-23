//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include <utest.h>
#include <utils/math/Projection.h>
#include <utils/math/Transform.h>
#include <utils/math/vec/Equality.h>

using namespace Siege;

static constexpr float aspectRatio = 1.33333337f;
static constexpr float degrees = 1.04719758f;

UTEST(test_Projection, CreatePerspectiveMatrix)
{
    float radians = 1.39626336f; // 80 degrees as radians
    Mat4 expected {{0.893815219, 0, 0, 0},
                   {0, -1.19175363, 0, 0},
                   {0, 0, -1.00001001, -1},
                   {0, 0, -0.0100001004, 0}};

    Mat4 perspective = Perspective(radians, aspectRatio, 0.01f, 1000.f);

    ASSERT_TRUE(perspective == expected);
}

UTEST(test_Projection, CreateOrthographicMatrix)
{
    Mat4 expected {{0.00249999994f, 0, 0, 0},
                   {0, -0.00333333341, 0, 0},
                   {0, 0, -0.00100009993, 0},
                   {-1, -1, -0.000100010002, 1}};

    Mat4 orthographic = Orthographic(0.f, 800.f, 600.f, 0.f, 0.1f, 1000.f);

    ASSERT_TRUE(orthographic == expected);
}

UTEST(test_Projection, InvertProjectionMatrix)
{
    auto perspective = Perspective(degrees, aspectRatio, 0.1f, 1000.f);

    Mat4 expected = {{0.769800425f, -0.f, 0.f, -0.f},
                     {-0.f, -0.577350318f, -0.f, 0.f},
                     {0.f, -0.f, 0.f, -9.99900054},
                     {-0.f, 0.f, -1.f, 10.000001f}};

    auto result = Mat4::Inverse(perspective);

    ASSERT_TRUE(result == expected);
}

UTEST(test_Projection, UnProject2DPointTo3D)
{
    Vec3 expected = {-0.000915590383f, -0.993325829f, -2.40004373f};

    const Vec3 camPos = {0.f, -1.f, -2.5f};
    const Vec3 camFront = {-0.00897132326f, 0.00620366121f, 0.999940514f};

    Vec2 ndcCoords = {-0.000244140625f, 0.10485673f};

    Mat4 projection = Perspective(degrees, aspectRatio, 0.1f, 100.f);
    Mat4 view = LookAt(camPos, camPos + camFront);

    Vec4 coords = {ndcCoords, 0.f, 1.f};

    Vec3 unProjected = UnProjectPoint3D(view, projection, coords);

    ASSERT_LE(expected.x - unProjected.x, ScaledEpsilon(expected.x, unProjected.x));
    ASSERT_LE(expected.y - unProjected.y, ScaledEpsilon(expected.y, unProjected.y));
    ASSERT_LE(expected.z - unProjected.z, ScaledEpsilon(expected.z, unProjected.z));
}

UTEST(test_Projection, ConvertWorldPointToScreen)
{
    Vec3 expected = {372.660217f, 307.925781f, 0.f};

    const Vec3 camPos = {0.f, -1.f, -2.5f};
    const Vec3 camFront = {0.0180248432f, 0.00381749892f, 0.999830245f};
    const Vec3 direction = {-0.0345313251f, 0.0190420188, 0.999222159f};

    Mat4 projection = Perspective(degrees, aspectRatio, 0.1f, 100.f);
    Mat4 view = LookAt(camPos, camPos + camFront);

    Vec3 screenCoords = WorldToScreen(camPos + 2.f * direction, projection * view, 800, 600);

    ASSERT_TRUE(screenCoords == expected);
}
