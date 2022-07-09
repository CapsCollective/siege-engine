//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include <utest.h>
#include <utils/Matrix.h>

#include <glm/glm.hpp>
#include <iostream>

// -------------------------------------- Matrix2x2 -----------------------------------------------

UTEST(test_Matrix2x2, CreateEmptyMatrix)
{
    Siege::Utils::Matrix2x2 matrix;

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(0.f, matrix[i]);
    }
}

UTEST(test_Matrix2x2, CreateMatrixWithValues)
{
    Siege::Utils::Matrix2x2 matrix = {0, 1, 2, 3};

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(i, matrix[i]);
    }
}

UTEST(test_Matrix2x2, CreateMatrixWithVectors)
{
    Siege::Utils::Matrix2x2 matrix = {{0, 1}, {2, 3}};

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(i, matrix[i]);
    }
}

UTEST(test_Matrix2x2, CreateIdentityMatrix)
{
    float expectedValues[] = {1.f, 0.f, 0.f, 1.f};

    Siege::Utils::Matrix2x2 matrix = Siege::Utils::Matrix2x2::Identity;

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(expectedValues[i], matrix[i]);
    }
}

UTEST(test_Matrix2x2, CopyMatrixUsingConstructor)
{
    Siege::Utils::Matrix2x2 matrixA = Siege::Utils::Matrix2x2::Identity;
    Siege::Utils::Matrix2x2 matrixB = {matrixA};

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(matrixA[i], matrixB[i]);
    }
}

UTEST(test_Matrix2x2, TestMatrixCopyInDifferentScopes)
{
    float expectedValues[] = {1.f, 0.f, 0.f, 1.f};

    Siege::Utils::Matrix2x2 matrixA;

    { // Open a new scope. MatrixB should not be in memory once we leave the brackets
        Siege::Utils::Matrix2x2 matrixB = Siege::Utils::Matrix2x2::Identity;
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

    Siege::Utils::Matrix2x2 matrixA = Siege::Utils::Matrix2x2::Identity;

    float result0 = matrixA.Get(0, 0);
    float result1 = matrixA.Get(1, 1);

    ASSERT_EQ(expected, result0);
    ASSERT_EQ(expected, result1);
}

UTEST(test_Matrix2x2, CheckMatrixEquality)
{
    Siege::Utils::Matrix2x2 matrixA = Siege::Utils::Matrix2x2::Identity;
    Siege::Utils::Matrix2x2 matrixB = {};
    Siege::Utils::Matrix2x2 matrixC = matrixA;

    ASSERT_TRUE(matrixA == matrixC);
    ASSERT_FALSE(matrixA == matrixB);
    ASSERT_TRUE(matrixB == Siege::Utils::Matrix2x2::Zero);
    ASSERT_FALSE(matrixB == matrixC);
}

UTEST(test_Matrix2x2, AddMatrices)
{
    float expectedValues[] = {2.f, 2.f, 2.f, 2.f};

    Siege::Utils::Matrix2x2 matrixA = {1.f, 1.f, 1.f, 1.f};
    Siege::Utils::Matrix2x2 matrixB = {1.f, 1.f, 1.f, 1.f};

    matrixA.Add(matrixB);

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixA[i]);
    }
}

UTEST(test_Matrix2x2, AddMatricesWithPlusEqualsOperator)
{
    float expectedValues[] = {2.f, 2.f, 2.f, 2.f};

    Siege::Utils::Matrix2x2 matrixA = {1.f, 1.f, 1.f, 1.f};
    Siege::Utils::Matrix2x2 matrixB = {1.f, 1.f, 1.f, 1.f};

    matrixA += matrixB;

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixA[i]);
    }
}

UTEST(test_Matrix2x2, AddMatricesAndReturnNewMatrix)
{
    float expectedValues[] = {2.f, 2.f, 2.f, 2.f};

    Siege::Utils::Matrix2x2 matrixA = {1.f, 1.f, 1.f, 1.f};
    Siege::Utils::Matrix2x2 matrixB = {1.f, 1.f, 1.f, 1.f};

    auto matrixC = matrixA + matrixB;

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixC[i]);
    }
}

UTEST(test_Matrix2x2, SubtractMatrices)
{
    Siege::Utils::Matrix2x2 expected = Siege::Utils::Matrix2x2::Zero;

    Siege::Utils::Matrix2x2 matrixA = {1.f, 1.f, 1.f, 1.f};
    Siege::Utils::Matrix2x2 matrixB = {1.f, 1.f, 1.f, 1.f};

    matrixA.Subtract(matrixB);

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(matrixA[i], expected[i]);
    }
}

UTEST(test_Matrix2x2, SubtractMatricesWithMinusEqualsOperator)
{
    Siege::Utils::Matrix2x2 expected = Siege::Utils::Matrix2x2::Zero;

    Siege::Utils::Matrix2x2 matrixA = {1.f, 1.f, 1.f, 1.f};
    Siege::Utils::Matrix2x2 matrixB = {1.f, 1.f, 1.f, 1.f};

    matrixA -= matrixB;

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(matrixA[i], expected[i]);
    }
}

UTEST(test_Matrix2x2, SubtractMatricesAndReturnNewMatrix)
{
    Siege::Utils::Matrix2x2 expected = Siege::Utils::Matrix2x2::Zero;

    Siege::Utils::Matrix2x2 matrixA = {1.f, 1.f, 1.f, 1.f};
    Siege::Utils::Matrix2x2 matrixB = {1.f, 1.f, 1.f, 1.f};

    Siege::Utils::Matrix2x2 matrixC = matrixA - matrixB;

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(matrixC[i], expected[i]);
    }
}

UTEST(test_Matrix2x2, MultiplyByScalar)
{
    Siege::Utils::Matrix2x2 expected = {3.f, 3.f, 3.f, 3.f};
    Siege::Utils::Matrix2x2 matrix = {1.f, 1.f, 1.f, 1.f};

    matrix.MultiplyScalar(3.f);

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(matrix[i], expected[i]);
    }
}

UTEST(test_Matrix2x2, MultiplyByScalarUsingTimesEqualsOperator)
{
    Siege::Utils::Matrix2x2 expected = {3.f, 3.f, 3.f, 3.f};
    Siege::Utils::Matrix2x2 matrix = {1.f, 1.f, 1.f, 1.f};

    matrix *= 3.f;

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(matrix[i], expected[i]);
    }
}

UTEST(test_Matrix2x2, MultiplyByScalarUsingTimesOperator)
{
    Siege::Utils::Matrix2x2 expected = {3.f, 3.f, 3.f, 3.f};
    Siege::Utils::Matrix2x2 matrix = {1.f, 1.f, 1.f, 1.f};

    Siege::Utils::Matrix2x2 matrixB = matrix * 3.f;

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(expected[i], matrixB[i]);
    }
}

UTEST(test_Matrix2x2, MultiplyTwoMatrices)
{
    Siege::Utils::Matrix2x2 expected = {5.f, 4.f, 12.f, 9.f}; // 1 2  3 2  5 4
    Siege::Utils::Matrix2x2 matrixA = {1.f, 2.f, 3.f, 3.f}; // 3 3  1 1  12 6
    Siege::Utils::Matrix2x2 matrixB = {3.f, 2.f, 1.f, 1.f}; //

    matrixA.Multiply(matrixB);

    ASSERT_TRUE(expected == matrixA);
}

UTEST(test_Matrix2x2, MultiplyTwoMatricesUsingTimesEqualsOperator)
{
    Siege::Utils::Matrix2x2 expected = {5.f, 4.f, 12.f, 9.f};
    Siege::Utils::Matrix2x2 matrixA = {1.f, 2.f, 3.f, 3.f};
    Siege::Utils::Matrix2x2 matrixB = {3.f, 2.f, 1.f, 1.f};

    matrixA *= matrixB;

    ASSERT_TRUE(expected == matrixA);
}

UTEST(test_Matrix2x2, MultiplyTwoMatricesUsingTimesOperator)
{
    Siege::Utils::Matrix2x2 expected = {5.f, 4.f, 12.f, 9.f};
    Siege::Utils::Matrix2x2 matrixA = {1.f, 2.f, 3.f, 3.f};
    Siege::Utils::Matrix2x2 matrixB = {3.f, 2.f, 1.f, 1.f};

    Siege::Utils::Matrix2x2 matrixC = matrixA * matrixB;

    ASSERT_TRUE(expected == matrixC);
}

UTEST(test_Matrix2x2, MultiplyVectorByMatrix)
{
    Vec2 expected {4, 9};

    Vec2 vector = {2, 1};
    Siege::Utils::Matrix2x2 matrix = {1.f, 2.f, 3.f, 3.f};

    Vec2 result = matrix.Multiply(vector);

    ASSERT_TRUE(expected == result);
}

UTEST(test_Matrix2x2, MultiplyVectorByWithTimesOperator)
{
    Vec2 expected {4, 9};

    Vec2 vector = {2, 1};
    Siege::Utils::Matrix2x2 matrix = {1.f, 2.f, 3.f, 3.f};

    Vec2 result = matrix * vector;

    ASSERT_TRUE(expected == result);
}

UTEST(test_Matrix2x2, CalculateDeterminant)
{
    float expectedDeterminant = 0;
    Siege::Utils::Matrix2x2 matrix = {1.f, 2.f, 1.f, 2.f};

    float determinant = matrix.Determinant();
    ASSERT_EQ(expectedDeterminant, determinant);

    Siege::Utils::Matrix2x2 matrix0 = {5.f, 2.f, 3.f, 3.f};
    expectedDeterminant = 9.f;
    determinant = matrix0.Determinant();
    ASSERT_EQ(expectedDeterminant, determinant);

    determinant = Siege::Utils::Matrix2x2::Determinant(matrix0);
    ASSERT_EQ(expectedDeterminant, determinant);
}

UTEST(test_Matrix2x2, CalculateInverse)
{
    Siege::Utils::Matrix2x2 expected = {0.25f, -0.25f, -0.25, 1.25f};

    Siege::Utils::Matrix2x2 matrix = {5.f, 1.f, 1.f, 1.f};

    Siege::Utils::Matrix2x2 result = matrix.Inverse();

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_TRUE((result[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }

    result = Siege::Utils::Matrix2x2::Inverse(matrix);

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_TRUE((result[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix2x2, DivideMatrix)
{
    Siege::Utils::Matrix2x2 expected = {1.f, 0.f, 0.545454545455f, 3.2727272727f};

    Siege::Utils::Matrix2x2 matrixA = {2.f, 4.f, 6.f, 1.f};
    Siege::Utils::Matrix2x2 matrixB = {5.f, 1.f, 1.f, 1.f};

    matrixA.Divide(matrixB);

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_TRUE((matrixA[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix2x2, DivideMatrixWithSlashEqualsOperator)
{
    Siege::Utils::Matrix2x2 expected = {1.f, 0.f, 0.545454545455f, 3.2727272727f};

    Siege::Utils::Matrix2x2 matrixA = {2.f, 4.f, 6.f, 1.f};
    Siege::Utils::Matrix2x2 matrixB = {5.f, 1.f, 1.f, 1.f};

    matrixA /= matrixB;

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_TRUE((matrixA[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix2x2, DivideMatrixWithSlashOperator)
{
    Siege::Utils::Matrix2x2 expected = {1.f, 0.f, 0.545454545455f, 3.2727272727f};

    Siege::Utils::Matrix2x2 matrixA = {2.f, 4.f, 6.f, 1.f};
    Siege::Utils::Matrix2x2 matrixB = {5.f, 1.f, 1.f, 1.f};

    Siege::Utils::Matrix2x2 result = matrixA / matrixB;

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_TRUE((result[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix2x2, TestReverseOperator)
{
    Siege::Utils::Matrix2x2 expected = {1.f, -1.f, 1.f, -1.f};

    Siege::Utils::Matrix2x2 matrix = {-1.f, 1.f, -1.f, 1.f};

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_TRUE(expected == -matrix);
    }
}

UTEST(test_Matrix2x2, TestTranspose)
{
    Siege::Utils::Matrix2x2 expected = {1.f, 3.f, 2.f, 4.f};

    Siege::Utils::Matrix2x2 matrix = {1.f, 2.f, 3.f, 4.f};

    Siege::Utils::Matrix2x2 transposed = matrix.Transpose();


    ASSERT_TRUE(expected == transposed);
}

// -------------------------------------- Matrix3x3 -----------------------------------------------

UTEST(test_Matrix3x3, CreateEmptyMatrix)
{
    Siege::Utils::Matrix3x3 matrix;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(0.f, matrix[i]);
    }
}

UTEST(test_Matrix3x3, CreateMatrixWithValues)
{
    float expectedValues[] = {1.f, 2.f, 3.f, 2.f, 4.f, 5.f, 6.f, 7.f, 9.f};

    Siege::Utils::Matrix3x3 matrix = {{1.f, 2.f, 3.f}, {2.f, 4.f, 5.f}, {6.f, 7.f, 9.f}};

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expectedValues[i], matrix[i]);
    }
}

UTEST(test_Matrix3x3, CreateIdentityMatrix)
{
    float expectedValues[] = {1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f};

    Siege::Utils::Matrix3x3 matrix = Siege::Utils::Matrix3x3::Identity;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expectedValues[i], matrix[i]);
    }
}

UTEST(test_Matrix3x3, CopyMatrixUsingConstructor)
{
    Siege::Utils::Matrix3x3 matrixA = Siege::Utils::Matrix3x3::Identity;
    Siege::Utils::Matrix3x3 matrixB = {matrixA};

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(matrixA[i], matrixB[i]);
    }
}

UTEST(test_Matrix3x3, TestMatrixCopyInDifferentScopes)
{
    float expectedValues[] = {1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f};

    Siege::Utils::Matrix3x3 matrixA;

    { // Open a new scope. MatrixB should not be in memory once we leave the brackets
        Siege::Utils::Matrix3x3 matrixB = Siege::Utils::Matrix3x3::Identity;
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

    Siege::Utils::Matrix3x3 matrixA = Siege::Utils::Matrix3x3::Identity;

    float result0 = matrixA.Get(0, 0);
    float result1 = matrixA.Get(1, 1);

    ASSERT_EQ(expected, result0);
    ASSERT_EQ(expected, result1);
}

UTEST(test_Matrix3x3, CheckMatrixEquality)
{
    Siege::Utils::Matrix3x3 matrixA = Siege::Utils::Matrix3x3::Identity;
    Siege::Utils::Matrix3x3 matrixB = {};
    Siege::Utils::Matrix3x3 matrixC = matrixA;

    ASSERT_TRUE(matrixA == matrixC);
    ASSERT_FALSE(matrixA == matrixB);
    ASSERT_TRUE(matrixB == Siege::Utils::Matrix3x3::Zero);
    ASSERT_FALSE(matrixB == matrixC);
}

UTEST(test_Matrix3x3, AddMatrices)
{
    float expectedValues[] = {2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f};

    Siege::Utils::Matrix3x3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};
    Siege::Utils::Matrix3x3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrixA.Add(matrixB);

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixA[i]);
    }
}

UTEST(test_Matrix3x3, AddMatricesWithPlusEqualsOperator)
{
    float expectedValues[] = {2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f};

    Siege::Utils::Matrix3x3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};
    Siege::Utils::Matrix3x3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrixA += matrixB;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixA[i]);
    }
}

UTEST(test_Matrix3x3, AddMatricesAndReturnNewMatrix)
{
    float expectedValues[] = {2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f};

    Siege::Utils::Matrix3x3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};
    Siege::Utils::Matrix3x3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    auto matrixC = matrixA + matrixB;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixC[i]);
    }
}

UTEST(test_Matrix3x3, SubtractMatrices)
{
    Siege::Utils::Matrix3x3 expected = Siege::Utils::Matrix3x3::Zero;

    Siege::Utils::Matrix3x3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};
    Siege::Utils::Matrix3x3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrixA.Subtract(matrixB);

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expected, matrixA);
    }
}

UTEST(test_Matrix3x3, SubtractMatricesWithMinusEqualsOperator)
{
    Siege::Utils::Matrix3x3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};
    Siege::Utils::Matrix3x3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrixA -= matrixB;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(Siege::Utils::Matrix3x3::Zero, matrixA);
    }
}

UTEST(test_Matrix3x3, SubtractMatricesAndReturnNewMatrix)
{
    Siege::Utils::Matrix3x3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};
    Siege::Utils::Matrix3x3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    Siege::Utils::Matrix3x3 matrixC = matrixA - matrixB;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(Siege::Utils::Matrix3x3::Zero, matrixC);
    }
}

UTEST(test_Matrix3x3, MultiplyByScalar)
{
    Siege::Utils::Matrix3x3 expected = {{3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}};
    Siege::Utils::Matrix3x3 matrix = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrix.MultiplyScalar(3.f);

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expected, matrix);
    }
}

UTEST(test_Matrix3x3, MultiplyByScalarUsingTimesEqualsOperator)
{
    Siege::Utils::Matrix3x3 expected = {{3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}};
    Siege::Utils::Matrix3x3 matrix = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrix *= 3.f;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expected, matrix);
    }
}

UTEST(test_Matrix3x3, MultiplyByScalarUsingEqualsOperator)
{
    Siege::Utils::Matrix3x3 expected = {{3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}};
    Siege::Utils::Matrix3x3 matrix = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    Siege::Utils::Matrix3x3 matrixB = matrix * 3.f;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expected, matrixB);
    }
}

UTEST(test_Matrix3x3, MultiplyTwoMatrices)
{
    Siege::Utils::Matrix3x3 expected = {{26.f, 24.f, 22.f}, {18.f, 16.f, 14.f}, {70.f, 64.f, 58.f}};
    Siege::Utils::Matrix3x3 matrixA = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};
    Siege::Utils::Matrix3x3 matrixB = {{3.f, 2.f, 1.f}, {1.f, 2.f, 3.f}, {7.f, 6.f, 5.f}};

    matrixA.Multiply(matrixB);

    ASSERT_EQ(expected, matrixA);
}

UTEST(test_Matrix3x3, MultiplyTwoMatricesUsingTimesEqualsOperator)
{
    Siege::Utils::Matrix3x3 expected = {{26.f, 24.f, 22.f}, {18.f, 16.f, 14.f}, {70.f, 64.f, 58.f}};
    Siege::Utils::Matrix3x3 matrixA = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};
    Siege::Utils::Matrix3x3 matrixB = {{3.f, 2.f, 1.f}, {1.f, 2.f, 3.f}, {7.f, 6.f, 5.f}};

    matrixA *= matrixB;

    ASSERT_EQ(expected, matrixA);
}

UTEST(test_Matrix3x3, MultiplyTwoMatricesUsingTimesOperator)
{
    Siege::Utils::Matrix3x3 expected = {{26.f, 24.f, 22.f}, {18.f, 16.f, 14.f}, {70.f, 64.f, 58.f}};
    Siege::Utils::Matrix3x3 matrixA = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};
    Siege::Utils::Matrix3x3 matrixB = {{3.f, 2.f, 1.f}, {1.f, 2.f, 3.f}, {7.f, 6.f, 5.f}};

    Siege::Utils::Matrix3x3 matrixC = matrixA * matrixB;

    ASSERT_EQ(expected, matrixC);
}

UTEST(test_Matrix3x3, MultiplyVectorByMatrix)
{
    Vec3 expected {10, 10, 30};

    Vec3 vector = {2, 1, 2};
    Siege::Utils::Matrix3x3 matrix = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};

    Vec3 result = matrix.Multiply(vector);

    ASSERT_TRUE(expected == result);
}

UTEST(test_Matrix3x3, MultiplyVectorByWithTimesOperator)
{
    Vec3 expected {10, 10, 30};

    Vec3 vector = {2, 1, 2};
    Siege::Utils::Matrix3x3 matrix = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};

    Vec3 result = matrix * vector;

    ASSERT_TRUE(expected == result);
}

UTEST(test_Matrix3x3, CalculateDeterminant)
{
    float expectedDeterminant = 0;
    Siege::Utils::Matrix3x3 matrix = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};

    float determinant = matrix.Determinant();
    ASSERT_EQ(expectedDeterminant, determinant);

    Siege::Utils::Matrix3x3 matrix0 = {{5.f, 2.f, 3.f}, {3.f, 6.f, 1.f}, {5.f, 6.f, 7.f}};
    expectedDeterminant = 112.f;
    determinant = matrix0.Determinant();
    ASSERT_EQ(expectedDeterminant, determinant);

    determinant = Siege::Utils::Matrix3x3::Determinant(matrix0);
    ASSERT_EQ(expectedDeterminant, determinant);
}

UTEST(test_Matrix3x3, CalculateInverse)
{
    Siege::Utils::Matrix3x3 expected = {{0.2142857f, -0.0357142f, -0.0357142f},
                                        {-0.0357142f, 0.2142857f, -0.0357142f},
                                        {-0.0357142f, -0.0357142f, 0.2142857f}};

    Siege::Utils::Matrix3x3 matrix = {{5.f, 1.f, 1.f}, {1.f, 5.f, 1.f}, {1.f, 1.f, 5.f}};

    Siege::Utils::Matrix3x3 result = matrix.Inverse();

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_TRUE((result[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }

    result = Siege::Utils::Matrix3x3::Inverse(matrix);

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_TRUE((result[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix3x3, DivideMatrix)
{
    Siege::Utils::Matrix3x3 expected = {{9.f, -3.f, -5.f},
                                        {5.f, 1.f, -5.f},
                                        {-2.33333333333, -0.333333333333, 3.f}};

    Siege::Utils::Matrix3x3 matrixA = {{2.f, 4.f, 6.f}, {1.f, 5.f, 6.f}, {2.f, 4.f, 7.f}};

    Siege::Utils::Matrix3x3 matrixB = {{5.f, 1.f, 1.f}, {1.f, 5.f, 1.f}, {1.f, 1.f, 5.f}};

    matrixA.Divide(matrixB);

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_TRUE((matrixA[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix3x3, DivideMatrixWithSlashEqualsOperator)
{
    Siege::Utils::Matrix3x3 expected = {{9.f, -3.f, -5.f},
                                        {5.f, 1.f, -5.f},
                                        {-2.33333333333, -0.333333333333, 3.f}};

    Siege::Utils::Matrix3x3 matrixA = {{2.f, 4.f, 6.f}, {1.f, 5.f, 6.f}, {2.f, 4.f, 7.f}};
    Siege::Utils::Matrix3x3 matrixB = {{5.f, 1.f, 1.f}, {1.f, 5.f, 1.f}, {1.f, 1.f, 5.f}};

    matrixA /= matrixB;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_TRUE((matrixA[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix3x3, DivideMatrixWithSlashOperator)
{
    Siege::Utils::Matrix3x3 expected = {{9.f, -3.f, -5.f},
                                        {5.f, 1.f, -5.f},
                                        {-2.33333333333, -0.333333333333, 3.f}};

    Siege::Utils::Matrix3x3 matrixA = {{2.f, 4.f, 6.f}, {1.f, 5.f, 6.f}, {2.f, 4.f, 7.f}};
    Siege::Utils::Matrix3x3 matrixB = {{5.f, 1.f, 1.f}, {1.f, 5.f, 1.f}, {1.f, 1.f, 5.f}};

    Siege::Utils::Matrix3x3 result = matrixA / matrixB;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_TRUE((result[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix3x3, TestReverseOperator)
{
    Siege::Utils::Matrix3x3 expected = {{1.f, -1.f, 1.f}, {-1.f, 1.f, -1.f}, {1.f, -1.f, 1.f}};

    Siege::Utils::Matrix3x3 matrix = {{-1.f, 1.f, -1.f}, {1.f, -1.f, 1.f}, {-1.f, 1.f, -1.f}};

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_TRUE(expected == -matrix);
    }
}

UTEST(test_Matrix3x3, TestTranspose)
{
    Siege::Utils::Matrix3x3 expected = {{1.f, 4.f, 7.f}, {2.f, 5.f, 8.f}, {3.f, 6.f, 9.f}};

    Siege::Utils::Matrix3x3 matrix = {{1.f, 2.f, 3.f}, {4.f, 5.f, 6.f}, {7.f, 8.f, 9.f}};

    Siege::Utils::Matrix3x3 transposed = matrix.Transpose();

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_TRUE(expected == transposed);
    }
}

// -------------------------------------- Matrix4x4 -----------------------------------------------



// Define test fixture
struct test_Matrix_Benchmark
{
    test_Matrix_Benchmark() {}

    static float* RandomValues()
    {
        static bool initialised = false;
        static float randomValues[180000000];

        if (!initialised)
        {
            std::cout << "INITIALISED" << std::endl;
            int seed = rand() % 1000000;

            for (size_t i = 0; i < 180000000; i++) randomValues[i] = seed + i % 1000000;
            initialised = true;
        }

        return randomValues;
    }

    static size_t& CurrentVal()
    {
        static size_t currentVal = 0;
        return currentVal;
    }
};

UTEST_F_SETUP(test_Matrix_Benchmark)
{
    std::cout << "STARTED UP" << std::endl;
}

UTEST_F_TEARDOWN(test_Matrix_Benchmark)
{
    std::cout << "TORN DOWN" << std::endl;
    test_Matrix_Benchmark::CurrentVal() = 0;
}

UTEST_F(test_Matrix_Benchmark, TestMatrix2x2ExecutionTimes)
{
    float* testVals = test_Matrix_Benchmark::RandomValues();
    size_t currentIndex = test_Matrix_Benchmark::CurrentVal();

    float additionTime = 0.f;
    float createTime = 0.f;
    size_t size = sizeof(Siege::Utils::Matrix2x2);

    for (size_t i = 0; i < 1000000; i++)
    {
        float x0 = testVals[currentIndex++], y0 = testVals[currentIndex++],
              x1 = testVals[currentIndex++], y1 = testVals[currentIndex++];

        auto startInit = std::chrono::high_resolution_clock::now();

        Siege::Utils::Matrix2x2 mat0 = {x0, y0, x1, y1};

        createTime += std::chrono::duration<float, std::chrono::microseconds::period>(
                          std::chrono::high_resolution_clock::now() - startInit)
                          .count();

        Siege::Utils::Matrix2x2 mat1 = {testVals[currentIndex++], testVals[currentIndex++],
                                        testVals[currentIndex++], testVals[currentIndex++]};

        auto start = std::chrono::high_resolution_clock::now();
        Siege::Utils::Matrix2x2 mat2 = mat0 * mat1;
        additionTime += std::chrono::duration<float, std::chrono::microseconds::period>(
                            std::chrono::high_resolution_clock::now() - start)
                            .count();
    }

    test_Matrix_Benchmark::CurrentVal() = currentIndex;

    std::cout << "SIZE: " << size << std::endl;
    std::cout << "CREATION TIME: " << createTime * 0.001 << "ms" << std::endl;
    std::cout << "EXECUTION TIME: " << additionTime * 0.001 << "ms" << std::endl;
    std::cout << "AVERAGE EXECUTION TIME PER ITERATION: " << (additionTime * 0.001) / 1000000
              << "ms" << std::endl;
}

UTEST_F(test_Matrix_Benchmark, TestGlm2x2ExecutionTimes)
{
    float* testVals = test_Matrix_Benchmark::RandomValues();
    size_t currentIndex = test_Matrix_Benchmark::CurrentVal();

    float additionTime = 0.f;
    float createTime = 0.f;
    size_t size = sizeof(glm::mat2);

    for (size_t i = 0; i < 1000000; i++)
    {
        float x0 = testVals[currentIndex++], y0 = testVals[currentIndex++],
              x1 = testVals[currentIndex++], y1 = testVals[currentIndex++];

        auto startInit = std::chrono::high_resolution_clock::now();
        glm::mat2 mat0 = { x0, y0, x1, y1 };
        createTime += std::chrono::duration<float, std::chrono::microseconds::period>(
                          std::chrono::high_resolution_clock::now() - startInit)
                          .count();

        glm::mat2 mat1 = {testVals[currentIndex++], testVals[currentIndex++],
                          testVals[currentIndex++], testVals[currentIndex++]};

        auto start = std::chrono::high_resolution_clock::now();
        glm::mat2 mat2 = mat0 * mat1;
        additionTime += std::chrono::duration<float, std::chrono::microseconds::period>(
                            std::chrono::high_resolution_clock::now() - start)
                            .count();
    }

    test_Matrix_Benchmark::CurrentVal() = currentIndex;

    std::cout << "SIZE: " << size << std::endl;
    std::cout << "CREATION TIME: " << createTime * 0.001 << "ms" << std::endl;
    std::cout << "EXECUTION TIME: " << additionTime * 0.001 << "ms" << std::endl;
    std::cout << "AVERAGE EXECUTION TIME PER ITERATION: " << (additionTime * 0.001) / 1000000
              << "ms" << std::endl;
}

UTEST_F(test_Matrix_Benchmark, TestMatrixExecutionTimes)
{
    float* testVals = test_Matrix_Benchmark::RandomValues();
    size_t currentIndex = test_Matrix_Benchmark::CurrentVal();

    float additionTime = 0.f;
    float createTime = 0.f;
    size_t size = sizeof(Siege::Utils::Matrix3x3);

    for (size_t i = 0; i < 1000000; i++)
    {
        float x0 = testVals[currentIndex++], y0 = testVals[currentIndex++],
              z0 = testVals[currentIndex++], x1 = testVals[currentIndex++],
              y1 = testVals[currentIndex++], z1 = testVals[currentIndex++],
              x2 = testVals[currentIndex++], y2 = testVals[currentIndex++],
              z2 = testVals[currentIndex++];

        auto startInit = std::chrono::high_resolution_clock::now();

        Siege::Utils::Matrix3x3 mat0 = {x0, y0, z0, x1, y1, z1, x2, y2, z2};

        createTime += std::chrono::duration<float, std::chrono::microseconds::period>(
                          std::chrono::high_resolution_clock::now() - startInit)
                          .count();

        Siege::Utils::Matrix3x3 mat1 = {
            {testVals[currentIndex++], testVals[currentIndex++], testVals[currentIndex++]},
            {testVals[currentIndex++], testVals[currentIndex++], testVals[currentIndex++]},
            {testVals[currentIndex++], testVals[currentIndex++], testVals[currentIndex++]}};

        auto start = std::chrono::high_resolution_clock::now();
        Siege::Utils::Matrix3x3 mat2 = mat0 * mat1;
        additionTime += std::chrono::duration<float, std::chrono::microseconds::period>(
                            std::chrono::high_resolution_clock::now() - start)
                            .count();
    }

    test_Matrix_Benchmark::CurrentVal() = currentIndex;

    std::cout << "SIZE: " << size << std::endl;
    std::cout << "CREATION TIME: " << createTime * 0.001 << "ms" << std::endl;
    std::cout << "EXECUTION TIME: " << additionTime * 0.001 << "ms" << std::endl;
    std::cout << "AVERAGE EXECUTION TIME PER ITERATION: " << (additionTime * 0.001) / 1000000
              << "ms" << std::endl;
}

UTEST_F(test_Matrix_Benchmark, TestGlmExecutionTimes)
{
    float* testVals = test_Matrix_Benchmark::RandomValues();
    size_t currentIndex = test_Matrix_Benchmark::CurrentVal();

    float additionTime = 0.f;
    float createTime = 0.f;
    size_t size = sizeof(glm::mat3);

    for (size_t i = 0; i < 1000000; i++)
    {
        float x0 = testVals[currentIndex++], y0 = testVals[currentIndex++],
              z0 = testVals[currentIndex++], x1 = testVals[currentIndex++],
              y1 = testVals[currentIndex++], z1 = testVals[currentIndex++],
              x2 = testVals[currentIndex++], y2 = testVals[currentIndex++],
              z2 = testVals[currentIndex++];

        auto startInit = std::chrono::high_resolution_clock::now();
        glm::mat3 mat0 = {x0, y0, z0, x1, y1, z1, x2, y2, z2};
        createTime += std::chrono::duration<float, std::chrono::microseconds::period>(
                          std::chrono::high_resolution_clock::now() - startInit)
                          .count();

        glm::mat3 mat1 = {
            {testVals[currentIndex++], testVals[currentIndex++], testVals[currentIndex++]},
            {testVals[currentIndex++], testVals[currentIndex++], testVals[currentIndex++]},
            {testVals[currentIndex++], testVals[currentIndex++], testVals[currentIndex++]}};

        auto start = std::chrono::high_resolution_clock::now();
        glm::mat3 mat2 = mat0 * mat1;
        additionTime += std::chrono::duration<float, std::chrono::microseconds::period>(
                            std::chrono::high_resolution_clock::now() - start)
                            .count();
    }

    test_Matrix_Benchmark::CurrentVal() = currentIndex;

    std::cout << "SIZE: " << size << std::endl;
    std::cout << "CREATION TIME: " << createTime * 0.001 << "ms" << std::endl;
    std::cout << "EXECUTION TIME: " << additionTime * 0.001 << "ms" << std::endl;
    std::cout << "AVERAGE EXECUTION TIME PER ITERATION: " << (additionTime * 0.001) / 1000000
              << "ms" << std::endl;
}

// -------------------------------------- Matrix4x4 -----------------------------------------------