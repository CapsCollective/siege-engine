//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include <utest.h>
#include <utils/math/mat/Equality.h>
#include <utils/math/mat/Format.h>
#include <utils/math/mat/Mat.h>
#include <utils/math/mat/Mat2.h>
#include <utils/math/mat/Mat4.h>
#include <utils/math/vec/Vec2.h>

using namespace Siege;

UTEST(test_Mat2, CreateEmptyMatrix)
{
    Mat2 mat {};

    ASSERT_TRUE(mat == Mat2::Zero());
}

UTEST(test_Mat2, CreateConstexprEmptyMatrix)
{
    constexpr Mat2 mat {};

    ASSERT_TRUE(mat == Mat2::Zero());
}

UTEST(test_Mat2, CreateIdentityMatrix)
{
    Mat2 mat = Mat2::Identity();

    ASSERT_EQ(1, mat[0][0]);
    ASSERT_EQ(1, mat[1][1]);

    ASSERT_FALSE(mat == Mat2::Zero());
}

UTEST(test_Mat2, CreateIdentityMatrixFromScalarInitialiser)
{
    Mat2 mat {1};

    ASSERT_TRUE(mat == Mat2::Identity());

    Mat2 mat2 {2};

    ASSERT_EQ(2, mat2[0][0]);
    ASSERT_EQ(2, mat2[1][1]);

    ASSERT_FALSE(mat2 == Mat2::Identity());
}

UTEST(test_Mat2, CreateMat2FromMat3)
{
    Mat2 expected = {{1.f, 2.f}, {4.f, 5.f}};

    Mat4 initialMat = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};

    Mat2 result = initialMat;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat2, CreateMat2FromMat4)
{
    Mat2 expected = {{1.f, 2.f}, {5.f, 6.f}};

    Mat4 initialMat = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};

    Mat2 result = initialMat;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat2, AddMatrices)
{
    Siege::Mat2 expectedMatrix = {{2, 2}, {2, 2}};

    Siege::Mat2 matrixA = {{1.f, 1.f}, {1.f, 1.f}};

    Siege::Mat2 matrixB = {{1.f, 1.f}, {1.f, 1.f}};

    Siege::Mat2 result = matrixA + matrixB;

    ASSERT_TRUE(result == expectedMatrix);
}

UTEST(test_Mat2, AddMatricesWithPlusEqualsOperator)
{
    Siege::Mat2 expectedMatrix = {{2, 2}, {2, 2}};

    Siege::Mat2 matrixA = {{1.f, 1.f}, {1.f, 1.f}};

    Siege::Mat2 matrixB = {{1.f, 1.f}, {1.f, 1.f}};

    matrixA += matrixB;

    ASSERT_TRUE(matrixA == expectedMatrix);
}

UTEST(test_Mat2, SubtractMatrices)
{
    Siege::Mat2 expected = Siege::Mat2::Zero();

    Siege::Mat2 matrixA = {{1.f, 1.f}, {1.f, 1.f}};

    Siege::Mat2 matrixB = {{1.f, 1.f}, {1.f, 1.f}};

    Siege::Mat2 result = matrixA - matrixB;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat2, SubtractMatricesWithMinusEqualsOperator)
{
    Siege::Mat2 expected = Siege::Mat2::Zero();

    Siege::Mat2 matrixA = {{1.f, 1.f}, {1.f, 1.f}};

    Siege::Mat2 matrixB = {{1.f, 1.f}, {1.f, 1.f}};

    matrixA -= matrixB;

    ASSERT_TRUE(matrixA == expected);
}

UTEST(test_Mat2, MultiplyTwoMatrices)
{
    Siege::Mat2 expected = {{18.f, 10.f}, {10.f, 6.f}};

    Siege::Mat2 matrixA = {{4.f, 2.f}, {3.f, 2.f}};

    Siege::Mat2 matrixB = {{3.f, 2.f}, {1.f, 2.f}};

    Siege::Mat2 result = matrixA * matrixB;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat2, MultiplyTwoMatricesWithTimesEqualsOperator)
{
    Siege::Mat2 expected = {{18.f, 10.f}, {10.f, 6.f}};

    Siege::Mat2 matrixA = {{4.f, 2.f}, {3.f, 2.f}};

    Siege::Mat2 matrixB = {{3.f, 2.f}, {1.f, 2.f}};

    matrixA *= matrixB;

    ASSERT_TRUE(matrixA == expected);
}

UTEST(test_Mat2, MultiplyByScalar)
{
    Siege::Mat2 expected = {{3.f, 3.f}, {3.f, 3.f}};

    Siege::Mat2 matrix = {{1.f, 1.f}, {1.f, 1.f}};

    Siege::Mat2 result = matrix * 3.f;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat2, MultiplyByScalarWithTimesEqualsOperator)
{
    Siege::Mat2 expected = {{3.f, 3.f}, {3.f, 3.f}};
    Siege::Mat2 matrix = {{1.f, 1.f}, {1.f, 1.f}};

    matrix *= 3.f;

    ASSERT_TRUE(matrix == expected);
}

UTEST(test_Mat2, MultiplyVectorByMatrix)
{
    Siege::Vec2 expected {5, 6};

    Siege::Vec2 vector = {2, 1};
    Siege::Mat2 matrix = {{1.f, 2.f}, {3.f, 2.f}};

    Siege::Vec2 result = matrix * vector;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat2, CalculateDeterminant)
{
    float expectedDeterminant = 0;
    Siege::Mat2 matrix = {{1.f, 2.f}, {1.f, 2.f}};

    float determinant = Siege::Mat2::Determinant(matrix);
    ASSERT_EQ(expectedDeterminant, determinant);

    Siege::Mat2 matrix0 = {{5.f, 2.f}, {3.f, 3.f}};
    expectedDeterminant = 9.f;
    determinant = Siege::Mat2::Determinant(matrix0);
    ASSERT_EQ(expectedDeterminant, determinant);

    determinant = Siege::Mat2::Determinant(matrix0);
    ASSERT_EQ(expectedDeterminant, determinant);
}

UTEST(test_Mat2, CalculateInverse)
{
    Siege::Mat2 expected = {{0.25f, -0.25f}, {-0.25, 1.25f}};

    Siege::Mat2 matrix = {{5.f, 1.f}, {1.f, 1.f}};

    Siege::Mat2 result = Siege::Mat2::Inverse(matrix);

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat2, DivideMatrix)
{
    Siege::Mat2 expected = {{-1.f, 0.75f}, {7.f, 0.25f}};

    Siege::Mat2 matrixA = {{2.f, 4.f}, {6.f, 1.f}};
    Siege::Mat2 matrixB = {{5.f, 1.f}, {1.f, 1.f}};

    Siege::Mat2 result = matrixA / matrixB;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat2, DivideMatrixWithSlashEqualsOperator)
{
    Siege::Mat2 expected = {{-1.f, 0.75f}, {7.f, 0.25f}};

    Siege::Mat2 matrixA = {{2.f, 4.f}, {6.f, 1.f}};
    Siege::Mat2 matrixB = {{5.f, 1.f}, {1.f, 1.f}};

    matrixA /= matrixB;

    ASSERT_TRUE(matrixA == expected);
}

UTEST(test_Mat2, TestTranspose)
{
    Siege::Mat2 expected = {{1.f, 5.f}, {2.f, 6.f}};

    Siege::Mat2 matrix = {
        {1.f, 2.f},
        {5.f, 6.f},
    };

    Siege::Mat2 transposed = Siege::Mat2::Transpose(matrix);

    ASSERT_TRUE(transposed == expected);
}

UTEST(test_Mat2, TestReverseOperator)
{
    Siege::Mat2 expected = {{1.f, -1.f}, {-1.f, 1.f}};

    Siege::Mat2 matrix = {{-1.f, 1.f}, {1.f, -1.f}};

    ASSERT_TRUE(-matrix == expected);
}

UTEST(test_Mat2, TestToString)
{
    String str = "{1.00,2.00}\n{3.00,4.00}";

    Mat2 matrix = {{1, 2}, {3, 4}};

    String result = ToString(matrix);

    ASSERT_TRUE(str == result);
}

UTEST(test_Mat2, TestFuzzyEquals)
{
    Mat2 matrixA = {{1, 2}, {3, 4}};

    Mat2 matrixB = {{0.99999999999912, 2}, {3, 4}};

    ASSERT_TRUE(FEquals(matrixA, matrixB));
}