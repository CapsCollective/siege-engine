//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include <utest.h>
#include <utils/math/Float.h>
#include <utils/math/Transform.h>
#include <utils/math/mat/Equality.h>

UTEST(test_Transform, CreateViewMatrix)
{
    Siege::Mat4 expected {{1, 0, -0, 0},
                          {0, -0.928476631, 0.371390671, 0},
                          {0, -0.371390641, -0.928476691, 0},
                          {-0, -0, 0, 1}};

    Siege::Mat4 result = Siege::LookAt({0.f, 0.f, 0.f}, {0, -2.f, 5.f});

    ASSERT_TRUE(Siege::Equals(expected, result, std::numeric_limits<float>::epsilon()));
}

UTEST(test_Transform, TranslateMatrix)
{
    Siege::Mat4 expected {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, -1, -2.5, 1}};

    Siege::Mat4 model = Siege::Mat4::Identity();
    Siege::Mat4 result = Siege::Translate(model, {0.f, -1.f, -2.5f});

    ASSERT_TRUE(result == expected);
}

UTEST(test_Transform, TranslateViewMatrix)
{
    Siege::Mat4 expected {{1, 0, -0, 0},
                          {0, -0.928476631, 0.371390671, 0},
                          {0, -0.371390641, -0.928476691, 0},
                          {0, 1.85695314, 1.94980109, 1}};

    Siege::Mat4 view = Siege::LookAt({0.f, 0.f, 0.f}, {0, -2.f, 5.f});
    Siege::Mat4 result = Siege::Translate(view, {0.f, -1.f, -2.5f});

    ASSERT_TRUE(Siege::Equals(expected, result, 1e-6f));
}

UTEST(test_Transform, RotateMatrix)
{
    Siege::Mat4 expected {{0.939692616, 0, -0.342020124, 0},
                          {0, 1, 0, 0},
                          {0.342020124, 0, 0.939692616, 0},
                          {0, 0, 0, 1}};

    Siege::Mat4 model = Siege::Mat4::Identity();
    Siege::Mat4 result = Siege::Rotate(model, Siege::Float::Radians(20.f), {0, 1, 0});

    ASSERT_TRUE(result == expected);
}

UTEST(test_Transform, ScaleMatrix)
{
    Siege::Mat4 expected {{2, 0, 0, 0}, {0, 2, 0, 0}, {0, 0, 2, 0}, {0, -1, -2.5, 1}};

    Siege::Mat4 model = Siege::Mat4::Identity();
    Siege::Mat4 translated = Siege::Translate(model, {0.f, -1.f, -2.5f});
    Siege::Mat4 result = Siege::Scale(translated, {2.f, 2.f, 2.f});

    ASSERT_TRUE(result == expected);
}

UTEST(test_Transform, InvertViewMatrix)
{
    const Siege::Vec3 camPos = {0.f, -1.f, -2.5f};
    Siege::Vec3 camFront = {0.0175485481f, 0.0562163703f, 0.998264372f};
    Siege::Vec3 camUp = {0.000988078303f, -0.998418629f, 0.0562076867f};

    Siege::Mat4 view = Siege::LookAt(camPos, camPos + camFront, camUp);

    Siege::Mat4 expectedView = {{0.999845623f, 0.000988078303f, -0.0175485499f, 0.f},
                                {3.69745345E-11f, -0.998418748f, -0.0562163666f, 0.f},
                                {-0.0175763462f, 0.0562076867f, -0.998264431f, 0.f},
                                {-0.0439408645f, -0.857899546f, -2.5518775f, 1.f}};

    ASSERT_TRUE(Siege::Equals(expectedView, view, 1e-6f));
}