//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include <utest.h>
#include <utils/mat/Mat3.h>

UTEST(test_Matrix3x3, CreateEmptyMatrix)
{
    Siege::Mat3 matrix;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(0.f, matrix[i]);
    }
}

UTEST(test_Matrix3x3, CreateMatrixWithValues)
{
    float expectedValues[] = {1.f, 2.f, 3.f, 2.f, 4.f, 5.f, 6.f, 7.f, 9.f};

    Siege::Mat3 matrix = {{1.f, 2.f, 3.f}, {2.f, 4.f, 5.f}, {6.f, 7.f, 9.f}};

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expectedValues[i], matrix[i]);
    }
}

UTEST(test_Matrix3x3, CreateIdentityMatrix)
{
    float expectedValues[] = {1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f};

    Siege::Mat3 matrix = Siege::Mat3::Identity;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expectedValues[i], matrix[i]);
    }
}

UTEST(test_Matrix3x3, CopyMatrixUsingConstructor)
{
    Siege::Mat3 matrixA = Siege::Mat3::Identity;
    Siege::Mat3 matrixB = {matrixA};

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(matrixA[i], matrixB[i]);
    }
}

UTEST(test_Matrix3x3, TestMatrixCopyInDifferentScopes)
{
    float expectedValues[] = {1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f};

    Siege::Mat3 matrixA;

    { // Open a new scope. MatrixB should not be in memory once we leave the brackets
        Siege::Mat3 matrixB = Siege::Mat3::Identity;
        matrixA = matrixB;
    }

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixA[i]);
    }
}

UTEST(test_Matrix3x3, GetElementInMatrix)
{
    float expected = 1;

    Siege::Mat3 matrixA = Siege::Mat3::Identity;

    float result0 = matrixA.Get(0, 0);
    float result1 = matrixA.Get(1, 1);

    ASSERT_EQ(expected, result0);
    ASSERT_EQ(expected, result1);
}

UTEST(test_Matrix3x3, CheckMatrixEquality)
{
    Siege::Mat3 matrixA = Siege::Mat3::Identity;
    Siege::Mat3 matrixB = {};
    Siege::Mat3 matrixC = matrixA;

    ASSERT_TRUE(matrixA == matrixC);
    ASSERT_FALSE(matrixA == matrixB);
    ASSERT_TRUE(matrixB == Siege::Mat3::Zero);
    ASSERT_FALSE(matrixB == matrixC);
}

UTEST(test_Matrix3x3, AddMatrices)
{
    float expectedValues[] = {2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f};

    Siege::Mat3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};
    Siege::Mat3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrixA.Add(matrixB);

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixA[i]);
    }
}

UTEST(test_Matrix3x3, AddMatricesWithPlusEqualsOperator)
{
    float expectedValues[] = {2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f};

    Siege::Mat3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};
    Siege::Mat3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrixA += matrixB;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixA[i]);
    }
}

UTEST(test_Matrix3x3, AddMatricesAndReturnNewMatrix)
{
    float expectedValues[] = {2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f};

    Siege::Mat3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};
    Siege::Mat3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    auto matrixC = matrixA + matrixB;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixC[i]);
    }
}

UTEST(test_Matrix3x3, SubtractMatrices)
{
    Siege::Mat3 expected = Siege::Mat3::Zero;

    Siege::Mat3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};
    Siege::Mat3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrixA.Subtract(matrixB);

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expected[i], matrixA[i]);
    }
}

UTEST(test_Matrix3x3, SubtractMatricesWithMinusEqualsOperator)
{
    Siege::Mat3 expected = Siege::Mat3::Zero;

    Siege::Mat3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};
    Siege::Mat3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrixA -= matrixB;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expected[i], matrixA[i]);
    }
}

UTEST(test_Matrix3x3, SubtractMatricesAndReturnNewMatrix)
{
    Siege::Mat3 expected = Siege::Mat3::Zero;

    Siege::Mat3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};
    Siege::Mat3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    Siege::Mat3 matrixC = matrixA - matrixB;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expected[0], matrixC[i]);
    }
}

UTEST(test_Matrix3x3, MultiplyByScalar)
{
    Siege::Mat3 expected = {{3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}};
    Siege::Mat3 matrix = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrix.Multiply(3.f);

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expected[i], matrix[i]);
    }
}

UTEST(test_Matrix3x3, MultiplyByScalarUsingTimesEqualsOperator)
{
    Siege::Mat3 expected = {{3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}};
    Siege::Mat3 matrix = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrix *= 3.f;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expected[i], matrix[i]);
    }
}

UTEST(test_Matrix3x3, MultiplyByScalarUsingEqualsOperator)
{
    Siege::Mat3 expected = {{3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}};
    Siege::Mat3 matrix = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    Siege::Mat3 matrixB = matrix * 3.f;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expected[i], matrixB[i]);
    }
}

UTEST(test_Matrix3x3, MultiplyTwoMatrices)
{
    Siege::Mat3 expected = {{26.f, 24.f, 22.f}, {18.f, 16.f, 14.f}, {70.f, 64.f, 58.f}};
    Siege::Mat3 matrixA = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};
    Siege::Mat3 matrixB = {{3.f, 2.f, 1.f}, {1.f, 2.f, 3.f}, {7.f, 6.f, 5.f}};

    matrixA.Multiply(matrixB);

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expected[i], matrixA[i]);
    }
}

UTEST(test_Matrix3x3, MultiplyTwoMatricesUsingTimesEqualsOperator)
{
    Siege::Mat3 expected = {{26.f, 24.f, 22.f}, {18.f, 16.f, 14.f}, {70.f, 64.f, 58.f}};
    Siege::Mat3 matrixA = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};
    Siege::Mat3 matrixB = {{3.f, 2.f, 1.f}, {1.f, 2.f, 3.f}, {7.f, 6.f, 5.f}};

    matrixA *= matrixB;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expected[i], matrixA[i]);
    }
}

UTEST(test_Matrix3x3, MultiplyTwoMatricesUsingTimesOperator)
{
    Siege::Mat3 expected = {{26.f, 24.f, 22.f}, {18.f, 16.f, 14.f}, {70.f, 64.f, 58.f}};
    Siege::Mat3 matrixA = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};
    Siege::Mat3 matrixB = {{3.f, 2.f, 1.f}, {1.f, 2.f, 3.f}, {7.f, 6.f, 5.f}};

    Siege::Mat3 matrixC = matrixA * matrixB;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expected[i], matrixC[i]);
    }
}

UTEST(test_Matrix3x3, MultiplyVectorByMatrix)
{
    Siege::Vec3 expected {10, 10, 30};

    Siege::Vec3 vector = {2, 1, 2};
    Siege::Mat3 matrix = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};

    Siege::Vec3 result = matrix.Multiply(vector);

    ASSERT_TRUE(expected == result);
}

UTEST(test_Matrix3x3, MultiplyVectorWithTimesOperator)
{
    Siege::Vec3 expected {10, 10, 30};

    Siege::Vec3 vector = {2, 1, 2};
    Siege::Mat3 matrix = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};

    Siege::Vec3 result = matrix * vector;

    ASSERT_TRUE(expected == result);
}

UTEST(test_Matrix3x3, CalculateDeterminant)
{
    float expectedDeterminant = 0;
    Siege::Mat3 matrix = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};

    float determinant = matrix.Determinant();
    ASSERT_EQ(expectedDeterminant, determinant);

    Siege::Mat3 matrix0 = {{5.f, 2.f, 3.f}, {3.f, 6.f, 1.f}, {5.f, 6.f, 7.f}};
    expectedDeterminant = 112.f;
    determinant = matrix0.Determinant();
    ASSERT_EQ(expectedDeterminant, determinant);

    determinant = Siege::Mat3::Determinant(matrix0);
    ASSERT_EQ(expectedDeterminant, determinant);
}

UTEST(test_Matrix3x3, CalculateInverse)
{
    Siege::Mat3 expected = {{0.2142857f, -0.0357142f, -0.0357142f},
                            {-0.0357142f, 0.2142857f, -0.0357142f},
                            {-0.0357142f, -0.0357142f, 0.2142857f}};

    Siege::Mat3 matrix = {{5.f, 1.f, 1.f}, {1.f, 5.f, 1.f}, {1.f, 1.f, 5.f}};

    Siege::Mat3 result = matrix.Inverse();

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_TRUE((result[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }

    result = Siege::Mat3::Inverse(matrix);

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_TRUE((result[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix3x3, DivideMatrix)
{
    Siege::Mat3 expected = {{9.f, -3.f, -5.f},
                            {5.f, 1.f, -5.f},
                            {-2.33333333333, -0.333333333333, 3.f}};

    Siege::Mat3 matrixA = {{2.f, 4.f, 6.f}, {1.f, 5.f, 6.f}, {2.f, 4.f, 7.f}};

    Siege::Mat3 matrixB = {{5.f, 1.f, 1.f}, {1.f, 5.f, 1.f}, {1.f, 1.f, 5.f}};

    matrixA.Divide(matrixB);

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_TRUE((matrixA[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix3x3, DivideMatrixWithSlashEqualsOperator)
{
    Siege::Mat3 expected = {{9.f, -3.f, -5.f},
                            {5.f, 1.f, -5.f},
                            {-2.33333333333, -0.333333333333, 3.f}};

    Siege::Mat3 matrixA = {{2.f, 4.f, 6.f}, {1.f, 5.f, 6.f}, {2.f, 4.f, 7.f}};
    Siege::Mat3 matrixB = {{5.f, 1.f, 1.f}, {1.f, 5.f, 1.f}, {1.f, 1.f, 5.f}};

    matrixA /= matrixB;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_TRUE((matrixA[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix3x3, DivideMatrixWithSlashOperator)
{
    Siege::Mat3 expected = {{9.f, -3.f, -5.f},
                            {5.f, 1.f, -5.f},
                            {-2.33333333333, -0.333333333333, 3.f}};

    Siege::Mat3 matrixA = {{2.f, 4.f, 6.f}, {1.f, 5.f, 6.f}, {2.f, 4.f, 7.f}};
    Siege::Mat3 matrixB = {{5.f, 1.f, 1.f}, {1.f, 5.f, 1.f}, {1.f, 1.f, 5.f}};

    Siege::Mat3 result = matrixA / matrixB;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_TRUE((result[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix3x3, TestReverseOperator)
{
    Siege::Mat3 expected = {{1.f, -1.f, 1.f}, {-1.f, 1.f, -1.f}, {1.f, -1.f, 1.f}};

    Siege::Mat3 matrix = {{-1.f, 1.f, -1.f}, {1.f, -1.f, 1.f}, {-1.f, 1.f, -1.f}};

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_TRUE(expected == -matrix);
    }
}

UTEST(test_Matrix3x3, TestTranspose)
{
    Siege::Mat3 expected = {{1.f, 4.f, 7.f}, {2.f, 5.f, 8.f}, {3.f, 6.f, 9.f}};

    Siege::Mat3 matrix = {{1.f, 2.f, 3.f}, {4.f, 5.f, 6.f}, {7.f, 8.f, 9.f}};

    Siege::Mat3 transposed = matrix.Transpose();

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_TRUE(expected == transposed);
    }
}
