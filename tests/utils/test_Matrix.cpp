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

UTEST(test_Matrix3x3, MultiplyVectorWithTimesOperator)
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

UTEST(test_Matrix4x4, CreateEmptyMatrix)
{
    Siege::Utils::Matrix4x4 matrix;

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(0.f, matrix[i]);
    }
}

UTEST(test_Matrix4x4, CreateMatrixWithValues)
{
    float expectedValues[] = {1.f, 2.f, 3.f, 4.f,
                              5.f, 6.f, 7.f, 8.f,
                              9.f, 10.f, 11.f, 12.f,
                              13.f, 14.f, 15.f, 16.f};

    Siege::Utils::Matrix4x4 matrix = {{1.f, 2.f, 3.f, 4.f},
                                      {5.f, 6.f, 7.f, 8.f},
                                      {9.f, 10.f, 11.f, 12.f},
                                      {13.f, 14.f, 15.f, 16.f}};

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expectedValues[i], matrix[i]);
    }
}

UTEST(test_Matrix4x4, CreateIdentityMatrix)
{
    float expectedValues[] = {1.f, 0.f, 0.f, 0.f,
                              0.f, 1.f, 0.f, 0.f,
                              0.f, 0.f, 1.f, 0.f,
                              0.f, 0.f, 0.f, 1.f};

    Siege::Utils::Matrix4x4 matrix = Siege::Utils::Matrix4x4::Identity;

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expectedValues[i], matrix[i]);
    }
}

UTEST(test_Matrix4x4, CopyMatrixUsingConstructor)
{
    Siege::Utils::Matrix4x4 matrixA = Siege::Utils::Matrix4x4::Identity;
    Siege::Utils::Matrix4x4 matrixB = {matrixA};

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(matrixA[i], matrixB[i]);
    }
}

UTEST(test_Matrix4x4, TestMatrixCopyInDifferentScopes)
{
    float expectedValues[] = {1.f, 0.f, 0.f, 0.f,
                              0.f, 1.f, 0.f, 0.f,
                              0.f, 0.f, 1.f, 0.f,
                              0.f, 0.f, 0.f, 1.f};

    Siege::Utils::Matrix4x4 matrixA;

    { // Open a new scope. MatrixB should not be in memory once we leave the brackets
        Siege::Utils::Matrix4x4 matrixB = Siege::Utils::Matrix4x4::Identity;
        matrixA = matrixB;
    }

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixA[i]);
    }
}

UTEST(test_Matrix4x4, GetElementInMatrix)
{
    float expected = 1;

    Siege::Utils::Matrix4x4 matrixA = Siege::Utils::Matrix4x4::Identity;

    float result0 = matrixA.Get(0, 0);
    float result1 = matrixA.Get(1, 1);

    ASSERT_EQ(expected, result0);
    ASSERT_EQ(expected, result1);
}

UTEST(test_Matrix4x4, CheckMatrixEquality)
{
    Siege::Utils::Matrix4x4 matrixA = Siege::Utils::Matrix4x4::Identity;
    Siege::Utils::Matrix4x4 matrixB = {};
    Siege::Utils::Matrix4x4 matrixC = matrixA;

    ASSERT_TRUE(matrixA == matrixC);
    ASSERT_FALSE(matrixA == matrixB);
    ASSERT_TRUE(matrixB == Siege::Utils::Matrix4x4::Zero);
    ASSERT_FALSE(matrixB == matrixC);
}

UTEST(test_Matrix4x4, AddMatrices)
{
    float expectedValues[] = {2.f, 2.f, 2.f, 2.f,
                              2.f, 2.f, 2.f, 2.f,
                              2.f, 2.f, 2.f, 2.f,
                              2.f, 2.f, 2.f, 2.f};

    Siege::Utils::Matrix4x4 matrixA = {{1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f}};

    Siege::Utils::Matrix4x4 matrixB = {{1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f}};

    matrixA.Add(matrixB);

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixA[i]);
    }
}

UTEST(test_Matrix4x4, AddMatricesWithPlusEqualsOperator)
{
    float expectedValues[] = {2.f, 2.f, 2.f, 2.f,
                              2.f, 2.f, 2.f, 2.f,
                              2.f, 2.f, 2.f, 2.f,
                              2.f, 2.f, 2.f, 2.f};

    Siege::Utils::Matrix4x4 matrixA = {{1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f}};

    Siege::Utils::Matrix4x4 matrixB = {{1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f}};

    matrixA += matrixB;

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixA[i]);
    }
}

UTEST(test_Matrix4x4, AddMatricesAndReturnNewMatrix)
{
    float expectedValues[] = {2.f, 2.f, 2.f, 2.f,
                              2.f, 2.f, 2.f, 2.f,
                              2.f, 2.f, 2.f, 2.f,
                              2.f, 2.f, 2.f, 2.f};

    Siege::Utils::Matrix4x4 matrixA = {{1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f}};

    Siege::Utils::Matrix4x4 matrixB = {{1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f}};

    auto matrixC = matrixA + matrixB;

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixC[i]);
    }
}

UTEST(test_Matrix4x4, SubtractMatrices)
{
    Siege::Utils::Matrix4x4 expected = Siege::Utils::Matrix4x4::Zero;

    Siege::Utils::Matrix4x4 matrixA = {{1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f}};

    Siege::Utils::Matrix4x4 matrixB = {{1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f}};

    matrixA.Subtract(matrixB);

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expected[i], matrixA[i]);
    }
}

UTEST(test_Matrix4x4, SubtractMatricesWithMinusEqualsOperator)
{
    Siege::Utils::Matrix4x4 expected = Siege::Utils::Matrix4x4::Zero;

    Siege::Utils::Matrix4x4 matrixA = {{1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f}};

    Siege::Utils::Matrix4x4 matrixB = {{1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f}};

    matrixA -= matrixB;

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expected[i], matrixA[i]);
    }
}

UTEST(test_Matrix4x4, SubtractMatricesAndReturnNewMatrix)
{
    Siege::Utils::Matrix4x4 expected = Siege::Utils::Matrix4x4::Zero;

    Siege::Utils::Matrix4x4 matrixA = {{1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f}};

    Siege::Utils::Matrix4x4 matrixB = {{1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f},
                                       {1.f, 1.f, 1.f, 1.f}};

    Siege::Utils::Matrix4x4 matrixC = matrixA - matrixB;

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expected[i], matrixC[i]);
    }
}

UTEST(test_Matrix4x4, MultiplyByScalar)
{

    Siege::Utils::Matrix4x4 expected = {{3.f, 3.f, 3.f, 3.f},
                                        {3.f, 3.f, 3.f, 3.f},
                                        {3.f, 3.f, 3.f, 3.f},
                                        {3.f, 3.f, 3.f, 3.f}};

    Siege::Utils::Matrix4x4 matrix = {{1.f, 1.f, 1.f, 1.f},
                                      {1.f, 1.f, 1.f, 1.f},
                                      {1.f, 1.f, 1.f, 1.f},
                                      {1.f, 1.f, 1.f, 1.f}};

    matrix.MultiplyScalar(3.f);

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expected[i], matrix[i]);
    }
}

UTEST(test_Matrix4x4, MultiplyByScalarUsingTimesEqualsOperator)
{
    Siege::Utils::Matrix4x4 expected = {{3.f, 3.f, 3.f, 3.f},
                                        {3.f, 3.f, 3.f, 3.f},
                                        {3.f, 3.f, 3.f, 3.f},
                                        {3.f, 3.f, 3.f, 3.f}};

    Siege::Utils::Matrix4x4 matrix = {{1.f, 1.f, 1.f, 1.f},
                                      {1.f, 1.f, 1.f, 1.f},
                                      {1.f, 1.f, 1.f, 1.f},
                                      {1.f, 1.f, 1.f, 1.f}};

    matrix *= 3.f;

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expected[i], matrix[i]);
    }
}

UTEST(test_Matrix4x4, MultiplyByScalarUsingEqualsOperator)
{
    Siege::Utils::Matrix4x4 expected = {{3.f, 3.f, 3.f, 3.f},
                                        {3.f, 3.f, 3.f, 3.f},
                                        {3.f, 3.f, 3.f, 3.f},
                                        {3.f, 3.f, 3.f, 3.f}};

    Siege::Utils::Matrix4x4 matrix = {{1.f, 1.f, 1.f, 1.f},
                                      {1.f, 1.f, 1.f, 1.f},
                                      {1.f, 1.f, 1.f, 1.f},
                                      {1.f, 1.f, 1.f, 1.f}};

    Siege::Utils::Matrix4x4 matrixB = matrix * 3.f;

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expected[i], matrixB[i]);
    }
}

UTEST(test_Matrix4x4, MultiplyTwoMatrices)
{
    Siege::Utils::Matrix4x4 expected = {{43.f, 45.f, 47.f, 49.f},
                                        {37.f, 35.f, 33.f, 31.f},
                                        {107.f, 109.f, 111.f, 113.f},
                                        {101.f, 99.f, 97.f, 95.f}};

    Siege::Utils::Matrix4x4 matrixA = {{1.f, 2.f, 3.f, 4.f},
                                       {4.f, 3.f, 2.f, 1.f},
                                       {5.f, 6.f, 7.f, 8.f},
                                       {8.f, 7.f, 6.f, 5.f}};

    Siege::Utils::Matrix4x4 matrixB = {{4.f, 3.f, 2.f, 1.f},
                                       {1.f, 2.f, 3.f, 4.f},
                                       {7.f, 6.f, 5.f, 4.f},
                                       {4.f, 5.f, 6.f, 7.f}};

    matrixA.Multiply(matrixB);

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expected[i], matrixA[i]);
    }
}

UTEST(test_Matrix4x4, MultiplyTwoMatricesUsingTimesEqualsOperator)
{
    Siege::Utils::Matrix4x4 expected = {{43.f, 45.f, 47.f, 49.f},
                                        {37.f, 35.f, 33.f, 31.f},
                                        {107.f, 109.f, 111.f, 113.f},
                                        {101.f, 99.f, 97.f, 95.f}};

    Siege::Utils::Matrix4x4 matrixA = {{1.f, 2.f, 3.f, 4.f},
                                       {4.f, 3.f, 2.f, 1.f},
                                       {5.f, 6.f, 7.f, 8.f},
                                       {8.f, 7.f, 6.f, 5.f}};

    Siege::Utils::Matrix4x4 matrixB = {{4.f, 3.f, 2.f, 1.f},
                                       {1.f, 2.f, 3.f, 4.f},
                                       {7.f, 6.f, 5.f, 4.f},
                                       {4.f, 5.f, 6.f, 7.f}};

    matrixA *= matrixB;

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expected[i], matrixA[i]);
    }
}

UTEST(test_Matrix4x4, MultiplyTwoMatricesUsingTimesOperator)
{
    Siege::Utils::Matrix4x4 expected = {{43.f, 45.f, 47.f, 49.f},
                                        {37.f, 35.f, 33.f, 31.f},
                                        {107.f, 109.f, 111.f, 113.f},
                                        {101.f, 99.f, 97.f, 95.f}};

    Siege::Utils::Matrix4x4 matrixA = {{1.f, 2.f, 3.f, 4.f},
                                       {4.f, 3.f, 2.f, 1.f},
                                       {5.f, 6.f, 7.f, 8.f},
                                       {8.f, 7.f, 6.f, 5.f}};

    Siege::Utils::Matrix4x4 matrixB = {{4.f, 3.f, 2.f, 1.f},
                                       {1.f, 2.f, 3.f, 4.f},
                                       {7.f, 6.f, 5.f, 4.f},
                                       {4.f, 5.f, 6.f, 7.f}};

    Siege::Utils::Matrix4x4 matrixC = matrixA * matrixB;

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expected[i], matrixC[i]);
    }
}

UTEST(test_Matrix4x4, MultiplyVectorByMatrix)
{
    Vec4 expected {14, 16, 38, 40};

    Vec4 vector = {2, 1, 2, 1};
    Siege::Utils::Matrix4x4 matrix = {{1.f, 2.f, 3.f, 4.f},
                                      {4.f, 3.f, 2.f, 1.f},
                                      {5.f, 6.f, 7.f, 8.f},
                                      {8.f, 7.f, 6.f, 5.f}};

    Vec4 result = matrix.Multiply(vector);

    ASSERT_TRUE(expected == result);
}

UTEST(test_Matrix4x4, MultiplyVectorWithTimesOperator)
{
    Vec4 expected {14, 16, 38, 40};

    Vec4 vector = {2, 1, 2, 1};
    Siege::Utils::Matrix4x4 matrix = {{1.f, 2.f, 3.f, 4.f},
                                      {4.f, 3.f, 2.f, 1.f},
                                      {5.f, 6.f, 7.f, 8.f},
                                      {8.f, 7.f, 6.f, 5.f}};

    Vec4 result = matrix * vector;

    ASSERT_TRUE(expected == result);
}

UTEST(test_Matrix4x4, CalculateDeterminant)
{
    float expectedDeterminant = 0.f;

    Siege::Utils::Matrix4x4 matrix = {{1.f, 2.f, 3.f, 4.f},
                                      {4.f, 3.f, 2.f, 1.f},
                                      {5.f, 6.f, 7.f, 8.f},
                                      {8.f, 7.f, 6.f, 5.f}};

    float determinant = matrix.Determinant();
    ASSERT_EQ(expectedDeterminant, determinant);

    Siege::Utils::Matrix4x4 matrix0 = {{5, 2, 3, 6},
                                       {8, 4, 2, 8},
                                       {4, 6, 9, 2},
                                       {3, 6, 9, 2}};
    expectedDeterminant = 128.f;
    determinant = matrix0.Determinant();
    ASSERT_EQ(expectedDeterminant, determinant);

    determinant = Siege::Utils::Matrix4x4::Determinant(matrix0);
    ASSERT_EQ(expectedDeterminant, determinant);
}

UTEST(test_Matrix4x4, CalculateInverse)
{
    Siege::Utils::Matrix4x4 expected = {{0.f, 0.f, 1.f, -1.f},
                                        {-0.53125f, 0.375f, -0.625f, 0.71875f},
                                        {0.3125f, -0.25f, 0.25f, -0.1875f},
                                        {0.1875f, 0.f, -0.75f, 0.6875f}};

    Siege::Utils::Matrix4x4 matrix = {{5, 2, 3, 6},
                                      {8, 4, 2, 8},
                                      {4, 6, 9, 2},
                                      {3, 6, 9, 2}};

    Siege::Utils::Matrix4x4 result = matrix.Inverse();

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_TRUE((result[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }

    result = Siege::Utils::Matrix4x4::Inverse(matrix);

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_TRUE((result[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix4x4, DivideMatrix)
{
    Siege::Utils::Matrix4x4 expected = Siege::Utils::Matrix4x4::Identity;

    Siege::Utils::Matrix4x4 matrixA = {{5.f, 2.f, 3.f, 6.f},
                                       {8.f, 4.f, 2.f, 8.f},
                                       {4.f, 6.f, 9.f, 2.f},
                                       {3.f, 6.f, 9.f, 2.f}};

    Siege::Utils::Matrix4x4 matrixB = {{5.f, 2.f, 3.f, 6.f},
                                       {8.f, 4.f, 2.f, 8.f},
                                       {4.f, 6.f, 9.f, 2.f},
                                       {3.f, 6.f, 9.f, 2.f}};

    matrixA.Divide(matrixB);

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expected[i], matrixA[i]);
    }
}

UTEST(test_Matrix4x4, DivideMatrixWithSlashEqualsOperator)
{
    Siege::Utils::Matrix4x4 expected = Siege::Utils::Matrix4x4::Identity;

    Siege::Utils::Matrix4x4 matrixA = {{5.f, 2.f, 3.f, 6.f},
                                       {8.f, 4.f, 2.f, 8.f},
                                       {4.f, 6.f, 9.f, 2.f},
                                       {3.f, 6.f, 9.f, 2.f}};

    Siege::Utils::Matrix4x4 matrixB = {{5.f, 2.f, 3.f, 6.f},
                                       {8.f, 4.f, 2.f, 8.f},
                                       {4.f, 6.f, 9.f, 2.f},
                                       {3.f, 6.f, 9.f, 2.f}};

    matrixA /= matrixB;

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expected[i], matrixA[i]);
    }
}

UTEST(test_Matrix4x4, DivideMatrixWithSlashOperator)
{
    Siege::Utils::Matrix4x4 expected = Siege::Utils::Matrix4x4::Identity;

    Siege::Utils::Matrix4x4 matrixA = {{5.f, 2.f, 3.f, 6.f},
                                       {8.f, 4.f, 2.f, 8.f},
                                       {4.f, 6.f, 9.f, 2.f},
                                       {3.f, 6.f, 9.f, 2.f}};

    Siege::Utils::Matrix4x4 matrixB = {{5.f, 2.f, 3.f, 6.f},
                                       {8.f, 4.f, 2.f, 8.f},
                                       {4.f, 6.f, 9.f, 2.f},
                                       {3.f, 6.f, 9.f, 2.f}};

    Siege::Utils::Matrix4x4 result = matrixA / matrixB;

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expected[i], result[i]);
    }
}

UTEST(test_Matrix4x4, TestTranspose)
{
    Siege::Utils::Matrix4x4 expected = {{1.f, 5.f, 9.f, 13},
                                        {2.f, 6.f, 10.f, 14.f},
                                        {3.f, 7.f, 11.f, 15.f},
                                        {4.f, 8.f, 12.f, 16.f}};

    Siege::Utils::Matrix4x4 matrix = {{1.f, 2.f, 3.f, 4.f},
                                      {5.f, 6.f, 7.f, 8.f},
                                      {9.f, 10.f, 11.f, 12.f},
                                      {13.f, 14.f, 15.f, 16.f}};

    Siege::Utils::Matrix4x4 transposed = matrix.Transpose();
    Siege::Utils::Matrix4x4 transposedS = Siege::Utils::Matrix4x4::Transpose(matrix);

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expected[i], transposed[i]);
        ASSERT_EQ(expected[i], transposedS[i]);
    }
}

UTEST(test_Matrix4x4, TestReverseOperator)
{
    Siege::Utils::Matrix4x4 expected = {{1.f, -1.f, 1.f, -1.f},
                                        {-1.f, 1.f, -1.f, 1.f},
                                        {1.f, -1.f, 1.f, -1.f},
                                        {-1.f, 1.f, -1.f, 1.f}};

    Siege::Utils::Matrix4x4 matrix = {{-1.f, 1.f, -1.f, 1.f},
                                      {1.f, -1.f, 1.f, -1.f},
                                      {-1.f, 1.f, -1.f, 1.f},
                                      {1.f, -1.f, 1.f, -1.f}};

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_TRUE(expected == -matrix);
    }
}
