//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include <utest.h>
#include <utils/mat/Mat2.h>

UTEST(test_Matrix2x2, CreateEmptyMatrix)
{
    Siege::Mat2 matrix;

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(0.f, matrix[i]);
    }
}

UTEST(test_Matrix2x2, CreateMatrixWithValues)
{
    Siege::Mat2 matrix = {0, 1, 2, 3};

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(i, matrix[i]);
    }
}

UTEST(test_Matrix2x2, CreateMatrixWithVectors)
{
    Siege::Mat2 matrix = {{0, 1}, {2, 3}};

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(i, matrix[i]);
    }
}

UTEST(test_Matrix2x2, CreateIdentityMatrix)
{
    float expectedValues[] = {1.f, 0.f, 0.f, 1.f};

    Siege::Mat2 matrix = Siege::Mat2::Identity;

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(expectedValues[i], matrix[i]);
    }
}

UTEST(test_Matrix2x2, CopyMatrixUsingConstructor)
{
    Siege::Mat2 matrixA = Siege::Mat2::Identity;
    Siege::Mat2 matrixB = {matrixA};

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(matrixA[i], matrixB[i]);
    }
}

UTEST(test_Matrix2x2, TestMatrixCopyInDifferentScopes)
{
    float expectedValues[] = {1.f, 0.f, 0.f, 1.f};

    Siege::Mat2 matrixA;

    { // Open a new scope. MatrixB should not be in memory once we leave the brackets
        Siege::Mat2 matrixB = Siege::Mat2::Identity;
        matrixA = matrixB;
    }

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixA[i]);
    }
}

UTEST(test_Matrix2x2, GetElementInMatrix)
{
    float expected = 1;

    Siege::Mat2 matrixA = Siege::Mat2::Identity;

    float result0 = matrixA.Get(0, 0);
    float result1 = matrixA.Get(1, 1);

    ASSERT_EQ(expected, result0);
    ASSERT_EQ(expected, result1);
}

UTEST(test_Matrix2x2, CheckMatrixEquality)
{
    Siege::Mat2 matrixA = Siege::Mat2::Identity;
    Siege::Mat2 matrixB = {};
    Siege::Mat2 matrixC = matrixA;

    ASSERT_TRUE(matrixA == matrixC);
    ASSERT_FALSE(matrixA == matrixB);
    ASSERT_TRUE(matrixB == Siege::Mat2::Zero);
    ASSERT_FALSE(matrixB == matrixC);
}

UTEST(test_Matrix2x2, AddMatrices)
{
    float expectedValues[] = {2.f, 2.f, 2.f, 2.f};

    Siege::Mat2 matrixA = {1.f, 1.f, 1.f, 1.f};
    Siege::Mat2 matrixB = {1.f, 1.f, 1.f, 1.f};

    matrixA.Add(matrixB);

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixA[i]);
    }
}

UTEST(test_Matrix2x2, AddMatricesWithPlusEqualsOperator)
{
    float expectedValues[] = {2.f, 2.f, 2.f, 2.f};

    Siege::Mat2 matrixA = {1.f, 1.f, 1.f, 1.f};
    Siege::Mat2 matrixB = {1.f, 1.f, 1.f, 1.f};

    matrixA += matrixB;

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixA[i]);
    }
}

UTEST(test_Matrix2x2, AddMatricesAndReturnNewMatrix)
{
    float expectedValues[] = {2.f, 2.f, 2.f, 2.f};

    Siege::Mat2 matrixA = {1.f, 1.f, 1.f, 1.f};
    Siege::Mat2 matrixB = {1.f, 1.f, 1.f, 1.f};

    auto matrixC = matrixA + matrixB;

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixC[i]);
    }
}

UTEST(test_Matrix2x2, SubtractMatrices)
{
    Siege::Mat2 expected = Siege::Mat2::Zero;

    Siege::Mat2 matrixA = {1.f, 1.f, 1.f, 1.f};
    Siege::Mat2 matrixB = {1.f, 1.f, 1.f, 1.f};

    matrixA.Subtract(matrixB);

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(matrixA[i], expected[i]);
    }
}

UTEST(test_Matrix2x2, SubtractMatricesWithMinusEqualsOperator)
{
    Siege::Mat2 expected = Siege::Mat2::Zero;

    Siege::Mat2 matrixA = {1.f, 1.f, 1.f, 1.f};
    Siege::Mat2 matrixB = {1.f, 1.f, 1.f, 1.f};

    matrixA -= matrixB;

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(matrixA[i], expected[i]);
    }
}

UTEST(test_Matrix2x2, SubtractMatricesAndReturnNewMatrix)
{
    Siege::Mat2 expected = Siege::Mat2::Zero;

    Siege::Mat2 matrixA = {1.f, 1.f, 1.f, 1.f};
    Siege::Mat2 matrixB = {1.f, 1.f, 1.f, 1.f};

    Siege::Mat2 matrixC = matrixA - matrixB;

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(matrixC[i], expected[i]);
    }
}

UTEST(test_Matrix2x2, MultiplyByScalar)
{
    Siege::Mat2 expected = {3.f, 3.f, 3.f, 3.f};
    Siege::Mat2 matrix = {1.f, 1.f, 1.f, 1.f};

    matrix.Multiply(3.f);

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(matrix[i], expected[i]);
    }
}

UTEST(test_Matrix2x2, MultiplyByScalarUsingTimesEqualsOperator)
{
    Siege::Mat2 expected = {3.f, 3.f, 3.f, 3.f};
    Siege::Mat2 matrix = {1.f, 1.f, 1.f, 1.f};

    matrix *= 3.f;

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(matrix[i], expected[i]);
    }
}

UTEST(test_Matrix2x2, MultiplyByScalarUsingTimesOperator)
{
    Siege::Mat2 expected = {3.f, 3.f, 3.f, 3.f};
    Siege::Mat2 matrix = {1.f, 1.f, 1.f, 1.f};

    Siege::Mat2 matrixB = matrix * 3.f;

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(expected[i], matrixB[i]);
    }
}

UTEST(test_Matrix2x2, MultiplyTwoMatrices)
{
    Siege::Mat2 expected = {5.f, 4.f, 12.f, 9.f}; // 1 2  3 2  5 4
    Siege::Mat2 matrixA = {1.f, 2.f, 3.f, 3.f}; // 3 3  1 1  12 6
    Siege::Mat2 matrixB = {3.f, 2.f, 1.f, 1.f}; //

    matrixA.Multiply(matrixB);

    ASSERT_TRUE(expected == matrixA);
}

UTEST(test_Matrix2x2, MultiplyTwoMatricesUsingTimesEqualsOperator)
{
    Siege::Mat2 expected = {5.f, 4.f, 12.f, 9.f};
    Siege::Mat2 matrixA = {1.f, 2.f, 3.f, 3.f};
    Siege::Mat2 matrixB = {3.f, 2.f, 1.f, 1.f};

    matrixA *= matrixB;

    ASSERT_TRUE(expected == matrixA);
}

UTEST(test_Matrix2x2, MultiplyTwoMatricesUsingTimesOperator)
{
    Siege::Mat2 expected = {5.f, 4.f, 12.f, 9.f};
    Siege::Mat2 matrixA = {1.f, 2.f, 3.f, 3.f};
    Siege::Mat2 matrixB = {3.f, 2.f, 1.f, 1.f};

    Siege::Mat2 matrixC = matrixA * matrixB;

    ASSERT_TRUE(expected == matrixC);
}

UTEST(test_Matrix2x2, MultiplyVectorByMatrix)
{
    Siege::Vec2 expected {4, 9};

    Siege::Vec2 vector = {2, 1};
    Siege::Mat2 matrix = {1.f, 2.f, 3.f, 3.f};

    Siege::Vec2 result = matrix.Multiply(vector);

    ASSERT_TRUE(expected == result);
}

UTEST(test_Matrix2x2, MultiplyVectorByWithTimesOperator)
{
    Siege::Vec2 expected {4, 9};

    Siege::Vec2 vector = {2, 1};
    Siege::Mat2 matrix = {1.f, 2.f, 3.f, 3.f};

    Siege::Vec2 result = matrix * vector;

    ASSERT_TRUE(expected == result);
}

UTEST(test_Matrix2x2, CalculateDeterminant)
{
    float expectedDeterminant = 0;
    Siege::Mat2 matrix = {1.f, 2.f, 1.f, 2.f};

    float determinant = matrix.Determinant();
    ASSERT_EQ(expectedDeterminant, determinant);

    Siege::Mat2 matrix0 = {5.f, 2.f, 3.f, 3.f};
    expectedDeterminant = 9.f;
    determinant = matrix0.Determinant();
    ASSERT_EQ(expectedDeterminant, determinant);

    determinant = Siege::Mat2::Determinant(matrix0);
    ASSERT_EQ(expectedDeterminant, determinant);
}

UTEST(test_Matrix2x2, CalculateInverse)
{
    Siege::Mat2 expected = {0.25f, -0.25f, -0.25, 1.25f};

    Siege::Mat2 matrix = {5.f, 1.f, 1.f, 1.f};

    Siege::Mat2 result = matrix.Inverse();

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_TRUE((result[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }

    result = Siege::Mat2::Inverse(matrix);

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_TRUE((result[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix2x2, DivideMatrix)
{
    Siege::Mat2 expected = {1.f, 0.f, 0.545454545455f, 3.2727272727f};

    Siege::Mat2 matrixA = {2.f, 4.f, 6.f, 1.f};
    Siege::Mat2 matrixB = {5.f, 1.f, 1.f, 1.f};

    matrixA.Divide(matrixB);

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_TRUE((matrixA[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix2x2, DivideMatrixWithSlashEqualsOperator)
{
    Siege::Mat2 expected = {1.f, 0.f, 0.545454545455f, 3.2727272727f};

    Siege::Mat2 matrixA = {2.f, 4.f, 6.f, 1.f};
    Siege::Mat2 matrixB = {5.f, 1.f, 1.f, 1.f};

    matrixA /= matrixB;

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_TRUE((matrixA[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix2x2, DivideMatrixWithSlashOperator)
{
    Siege::Mat2 expected = {1.f, 0.f, 0.545454545455f, 3.2727272727f};

    Siege::Mat2 matrixA = {2.f, 4.f, 6.f, 1.f};
    Siege::Mat2 matrixB = {5.f, 1.f, 1.f, 1.f};

    Siege::Mat2 result = matrixA / matrixB;

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_TRUE((result[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix2x2, TestReverseOperator)
{
    Siege::Mat2 expected = {1.f, -1.f, 1.f, -1.f};

    Siege::Mat2 matrix = {-1.f, 1.f, -1.f, 1.f};

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_TRUE(expected == -matrix);
    }
}

UTEST(test_Matrix2x2, TestTranspose)
{
    Siege::Mat2 expected = {1.f, 3.f, 2.f, 4.f};

    Siege::Mat2 matrix = {1.f, 2.f, 3.f, 4.f};

    Siege::Mat2 transposed = matrix.Transpose();

    ASSERT_TRUE(expected == transposed);
}