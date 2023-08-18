//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <utest.h>
#include <utils/math/Maths.h>
#include <utils/math/mat/Equality.h>
#include <utils/math/mat/Format.h>
#include <utils/math/mat/Mat2.h>
#include <utils/math/mat/Mat3.h>
#include <utils/math/mat/Mat4.h>

using namespace Siege;

UTEST(test_Mat3, CreateEmptyMatrix)
{
    Mat3 mat {};

    ASSERT_TRUE(mat == Mat3::Zero());
}

UTEST(test_Mat3, CreateConstexprEmptyMatrix)
{
    constexpr Mat3 mat {};

    ASSERT_TRUE(mat == Mat3::Zero());
}

UTEST(test_Mat3, CreateIdentityMatrix)
{
    Mat3 mat = Mat3::Identity();

    ASSERT_EQ(1, mat[0][0]);
    ASSERT_EQ(1, mat[1][1]);
    ASSERT_EQ(1, mat[2][2]);

    ASSERT_FALSE(mat == Mat3::Zero());
}

UTEST(test_Mat3, CreateIdentityMatrixFromScalarInitialiser)
{
    Mat3 mat {1};

    ASSERT_TRUE(mat == Mat3::Identity());

    Mat3 mat2 {2};

    ASSERT_EQ(2, mat2[0][0]);
    ASSERT_EQ(2, mat2[1][1]);
    ASSERT_EQ(2, mat2[2][2]);

    ASSERT_FALSE(mat2 == Mat3::Identity());
}

UTEST(test_Mat3, CreateMat3FromMat2)
{
    Mat3 expected = {{1.f, 2.f, 0.f}, {3.f, 4.f, 0.f}, {0.f, 0.f, 1.f}};

    Mat2 initialMat = {{1, 2}, {3, 4}};

    Mat3 result = initialMat;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat3, CreateMat3FromMat4)
{
    Mat3 expected = {{1.f, 2.f, 3.f}, {5.f, 6.f, 7.f}, {9.f, 10.f, 11.f}};

    Mat4 initialMat = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};

    Mat3 result = initialMat;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat3, AddMatrices)
{
    Siege::Mat3 expectedMatrix = {{2, 2, 2}, {2, 2, 2}, {2, 2, 2}};

    Siege::Mat3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    Siege::Mat3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    Siege::Mat3 result = matrixA + matrixB;

    ASSERT_TRUE(result == expectedMatrix);
}

UTEST(test_Mat3, AddMatricesWithPlusEqualsOperator)
{
    Siege::Mat3 expectedMatrix = {{2, 2, 2}, {2, 2, 2}, {2, 2, 2}};

    Siege::Mat3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    Siege::Mat3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrixA += matrixB;

    ASSERT_TRUE(matrixA == expectedMatrix);
}

UTEST(test_Mat3, SubtractMatrices)
{
    Siege::Mat3 expected = Siege::Mat3::Zero();

    Siege::Mat3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    Siege::Mat3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    Siege::Mat3 result = matrixA - matrixB;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat3, SubtractMatricesWithMinusEqualsOperator)
{
    Siege::Mat3 expected = Siege::Mat3::Zero();

    Siege::Mat3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    Siege::Mat3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrixA -= matrixB;

    ASSERT_TRUE(matrixA == expected);
}

UTEST(test_Mat3, MultiplyTwoMatrices)
{
    Siege::Mat3 expected = {{14.f, 16.f, 18.f}, {22.f, 24.f, 26.f}, {50.f, 56.f, 62.f}};

    Siege::Mat3 matrixA = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};

    Siege::Mat3 matrixB = {{3.f, 2.f, 1.f}, {1.f, 2.f, 3.f}, {7.f, 6.f, 5.f}};

    Siege::Mat3 result = matrixA * matrixB;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat3, MultiplyTwoMatricesWithTimesEqualsOperator)
{
    Siege::Mat3 expected = {{14.f, 16.f, 18.f}, {22.f, 24.f, 26.f}, {50.f, 56.f, 62.f}};

    Siege::Mat3 matrixA = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};

    Siege::Mat3 matrixB = {{3.f, 2.f, 1.f}, {1.f, 2.f, 3.f}, {7.f, 6.f, 5.f}};

    matrixA *= matrixB;

    ASSERT_TRUE(matrixA == expected);
}

UTEST(test_Mat3, MultiplyByScalar)
{
    Siege::Mat3 expected = {{3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}};

    Siege::Mat3 matrix = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    Siege::Mat3 result = matrix * 3.f;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat3, MultiplyByScalarWithTimesEqualsOperator)
{
    Siege::Mat3 expected = {{3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}};
    Siege::Mat3 matrix = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrix *= 3.f;

    ASSERT_TRUE(matrix == expected);
}

UTEST(test_Mat3, MultiplyVectorByMatrix)
{
    Siege::Vec3 expected {15, 18, 21};

    Siege::Vec3 vector = {2, 1, 2};
    Siege::Mat3 matrix = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};

    Siege::Vec3 result = matrix * vector;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat3, CalculateDeterminant)
{
    float expectedDeterminant = 0;
    Mat3 matrix = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};

    float determinant = Mat3::Determinant(matrix);
    ASSERT_EQ(expectedDeterminant, determinant);

    Mat3 matrix0 = {{5.f, 2.f, 3.f}, {3.f, 6.f, 1.f}, {5.f, 6.f, 7.f}};
    expectedDeterminant = 112.f;
    determinant = Mat3::Determinant(matrix0);
    ASSERT_EQ(expectedDeterminant, determinant);
}

UTEST(test_Mat3, CalculateInverse)
{
    Mat3 expected = {{0.214285731f, -0.0357142873f, -0.0357142873f},
                     {-0.0357142873f, 0.214285731f, -0.0357142873},
                     {-0.0357142873f, -0.0357142873f, 0.214285731f}};

    Mat3 matrix = {{5.f, 1.f, 1.f}, {1.f, 5.f, 1.f}, {1.f, 1.f, 5.f}};

    Mat3 result = Mat3::Inverse(matrix);

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat3, DivideMatrix)
{
    Siege::Mat3 expected = {{0.321428597f, 0.535714328f, 0.821428656f},
                            {0.0714285895f, 0.785714388f, 0.821428656f},
                            {0.321428597f, 0.535714328f, 1.07142866f}};

    Siege::Mat3 matrixA = {{2.f, 4.f, 6.f}, {1.f, 5.f, 6.f}, {2.f, 4.f, 7.f}};

    Siege::Mat3 matrixB = {{5.f, 1.f, 1.f}, {1.f, 5.f, 1.f}, {1.f, 1.f, 5.f}};

    Siege::Mat3 result = matrixA / matrixB;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat3, DivideMatrixWithSlashEqualsOperator)
{
    Siege::Mat3 expected = {{0.321428597f, 0.535714328f, 0.821428656f},
                            {0.0714285895f, 0.785714388f, 0.821428656f},
                            {0.321428597f, 0.535714328f, 1.07142866f}};

    Siege::Mat3 matrixA = {{2.f, 4.f, 6.f}, {1.f, 5.f, 6.f}, {2.f, 4.f, 7.f}};

    Siege::Mat3 matrixB = {{5.f, 1.f, 1.f}, {1.f, 5.f, 1.f}, {1.f, 1.f, 5.f}};

    matrixA /= matrixB;

    ASSERT_TRUE(matrixA == expected);
}

UTEST(test_Mat3, TransposeMatrix)
{
    Siege::Mat3 expected = {{1.f, 5.f, 9.f}, {2.f, 6.f, 10.f}, {3.f, 7.f, 11.f}};

    Siege::Mat3 matrix = {{1.f, 2.f, 3.f}, {5.f, 6.f, 7.f}, {9.f, 10.f, 11.f}};

    Siege::Mat3 transposed = Siege::Mat3::Transpose(matrix);

    ASSERT_TRUE(transposed == expected);
}

UTEST(test_Mat3, NegateMatrix)
{
    Siege::Mat3 expected = {{1.f, -1.f, 1.f}, {-1.f, 1.f, -1.}, {1.f, -1.f, 1.f}};

    Siege::Mat3 matrix = {{-1.f, 1.f, -1.f},
                          {
                              1.f,
                              -1.f,
                              1.f,
                          },
                          {-1.f, 1.f, -1.f}};

    ASSERT_TRUE(-matrix == expected);
}

UTEST(test_Mat3, ConvertMatrixToString)
{
    String str = "{1.00,2.00,3.00}\n{4.00,5.00,6.00}\n{7.00,8.00,9.00}";

    Mat3 matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    String result = ToString(matrix);

    ASSERT_TRUE(str == result);
}

UTEST(test_Mat3, EvaluateEquallityWithEpsilon)
{
    Mat3 matrixA = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    Mat3 matrixB = {{0.99999999999912, 2, 3}, {4, 5.0000000000000000012, 6}, {7, 8, 9}};

    ASSERT_TRUE(Equals(matrixA, matrixB, Epsilon<float>()));
}