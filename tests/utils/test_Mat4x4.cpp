//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include <utest.h>
#include <utils/math/mat/Mat4.h>

#include <limits>

UTEST(test_Matrix4x4, CreateEmptyMatrix)
{
    Siege::Mat4 matrix;

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(0.f, matrix[i]);
    }
}

UTEST(test_Matrix4x4, CreateMatrixWithValues)
{
    float expectedValues[] =
        {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f};

    Siege::Mat4 matrix = {{1.f, 2.f, 3.f, 4.f},
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
    float expectedValues[] =
        {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f};

    Siege::Mat4 matrix = Siege::Mat4::Identity;

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expectedValues[i], matrix[i]);
    }
}

UTEST(test_Matrix4x4, CopyMatrixUsingConstructor)
{
    Siege::Mat4 matrixA = Siege::Mat4::Identity;
    Siege::Mat4 matrixB = {matrixA};

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(matrixA[i], matrixB[i]);
    }
}

UTEST(test_Matrix4x4, TestMatrixCopyInDifferentScopes)
{
    float expectedValues[] =
        {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f};

    Siege::Mat4 matrixA;

    { // Open a new scope. MatrixB should not be in memory once we leave the brackets
        Siege::Mat4 matrixB = Siege::Mat4::Identity;
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

    Siege::Mat4 matrixA = Siege::Mat4::Identity;

    float result0 = matrixA.Get(0, 0);
    float result1 = matrixA.Get(1, 1);

    ASSERT_EQ(expected, result0);
    ASSERT_EQ(expected, result1);
}

UTEST(test_Matrix4x4, CheckMatrixEquality)
{
    Siege::Mat4 matrixA = Siege::Mat4::Identity;
    Siege::Mat4 matrixB = {};
    Siege::Mat4 matrixC = matrixA;

    ASSERT_TRUE(matrixA == matrixC);
    ASSERT_FALSE(matrixA == matrixB);
    ASSERT_TRUE(matrixB == Siege::Mat4::Zero);
    ASSERT_FALSE(matrixB == matrixC);
}

UTEST(test_Matrix4x4, AddMatrices)
{
    float expectedValues[] =
        {2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f};

    Siege::Mat4 matrixA = {{1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f}};

    Siege::Mat4 matrixB = {{1.f, 1.f, 1.f, 1.f},
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
    float expectedValues[] =
        {2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f};

    Siege::Mat4 matrixA = {{1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f}};

    Siege::Mat4 matrixB = {{1.f, 1.f, 1.f, 1.f},
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
    float expectedValues[] =
        {2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f};

    Siege::Mat4 matrixA = {{1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f}};

    Siege::Mat4 matrixB = {{1.f, 1.f, 1.f, 1.f},
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
    Siege::Mat4 expected = Siege::Mat4::Zero;

    Siege::Mat4 matrixA = {{1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f}};

    Siege::Mat4 matrixB = {{1.f, 1.f, 1.f, 1.f},
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
    Siege::Mat4 expected = Siege::Mat4::Zero;

    Siege::Mat4 matrixA = {{1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f}};

    Siege::Mat4 matrixB = {{1.f, 1.f, 1.f, 1.f},
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
    Siege::Mat4 expected = Siege::Mat4::Zero;

    Siege::Mat4 matrixA = {{1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f}};

    Siege::Mat4 matrixB = {{1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f}};

    Siege::Mat4 matrixC = matrixA - matrixB;

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expected[i], matrixC[i]);
    }
}

UTEST(test_Matrix4x4, MultiplyByScalar)
{
    Siege::Mat4 expected = {{3.f, 3.f, 3.f, 3.f},
                            {3.f, 3.f, 3.f, 3.f},
                            {3.f, 3.f, 3.f, 3.f},
                            {3.f, 3.f, 3.f, 3.f}};

    Siege::Mat4 matrix = {{1.f, 1.f, 1.f, 1.f},
                          {1.f, 1.f, 1.f, 1.f},
                          {1.f, 1.f, 1.f, 1.f},
                          {1.f, 1.f, 1.f, 1.f}};

    matrix.Multiply(3.f);

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expected[i], matrix[i]);
    }
}

UTEST(test_Matrix4x4, MultiplyByScalarUsingTimesEqualsOperator)
{
    Siege::Mat4 expected = {{3.f, 3.f, 3.f, 3.f},
                            {3.f, 3.f, 3.f, 3.f},
                            {3.f, 3.f, 3.f, 3.f},
                            {3.f, 3.f, 3.f, 3.f}};

    Siege::Mat4 matrix = {{1.f, 1.f, 1.f, 1.f},
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
    Siege::Mat4 expected = {{3.f, 3.f, 3.f, 3.f},
                            {3.f, 3.f, 3.f, 3.f},
                            {3.f, 3.f, 3.f, 3.f},
                            {3.f, 3.f, 3.f, 3.f}};

    Siege::Mat4 matrix = {{1.f, 1.f, 1.f, 1.f},
                          {1.f, 1.f, 1.f, 1.f},
                          {1.f, 1.f, 1.f, 1.f},
                          {1.f, 1.f, 1.f, 1.f}};

    Siege::Mat4 matrixB = matrix * 3.f;

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expected[i], matrixB[i]);
    }
}

UTEST(test_Matrix4x4, MultiplyTwoMatrices)
{
    Siege::Mat4 expected = {{43.f, 45.f, 47.f, 49.f},
                            {37.f, 35.f, 33.f, 31.f},
                            {107.f, 109.f, 111.f, 113.f},
                            {101.f, 99.f, 97.f, 95.f}};

    Siege::Mat4 matrixA = {{1.f, 2.f, 3.f, 4.f},
                           {4.f, 3.f, 2.f, 1.f},
                           {5.f, 6.f, 7.f, 8.f},
                           {8.f, 7.f, 6.f, 5.f}};

    Siege::Mat4 matrixB = {{4.f, 3.f, 2.f, 1.f},
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
    Siege::Mat4 expected = {{43.f, 45.f, 47.f, 49.f},
                            {37.f, 35.f, 33.f, 31.f},
                            {107.f, 109.f, 111.f, 113.f},
                            {101.f, 99.f, 97.f, 95.f}};

    Siege::Mat4 matrixA = {{1.f, 2.f, 3.f, 4.f},
                           {4.f, 3.f, 2.f, 1.f},
                           {5.f, 6.f, 7.f, 8.f},
                           {8.f, 7.f, 6.f, 5.f}};

    Siege::Mat4 matrixB = {{4.f, 3.f, 2.f, 1.f},
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
    Siege::Mat4 expected = {{43.f, 45.f, 47.f, 49.f},
                            {37.f, 35.f, 33.f, 31.f},
                            {107.f, 109.f, 111.f, 113.f},
                            {101.f, 99.f, 97.f, 95.f}};

    Siege::Mat4 matrixA = {{1.f, 2.f, 3.f, 4.f},
                           {4.f, 3.f, 2.f, 1.f},
                           {5.f, 6.f, 7.f, 8.f},
                           {8.f, 7.f, 6.f, 5.f}};

    Siege::Mat4 matrixB = {{4.f, 3.f, 2.f, 1.f},
                           {1.f, 2.f, 3.f, 4.f},
                           {7.f, 6.f, 5.f, 4.f},
                           {4.f, 5.f, 6.f, 7.f}};

    Siege::Mat4 matrixC = matrixA * matrixB;

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expected[i], matrixC[i]);
    }
}

UTEST(test_Matrix4x4, MultiplyVectorByMatrix)
{
    Siege::Vec4 expected {14, 16, 38, 40};

    Siege::Vec4 vector = {2, 1, 2, 1};
    Siege::Mat4 matrix = {{1.f, 2.f, 3.f, 4.f},
                          {4.f, 3.f, 2.f, 1.f},
                          {5.f, 6.f, 7.f, 8.f},
                          {8.f, 7.f, 6.f, 5.f}};

    Siege::Vec4 result = matrix.Multiply(vector);

    ASSERT_TRUE(expected == result);
}

UTEST(test_Matrix4x4, MultiplyVectorWithTimesOperator)
{
    Siege::Vec4 expected {14, 16, 38, 40};

    Siege::Vec4 vector = {2, 1, 2, 1};
    Siege::Mat4 matrix = {{1.f, 2.f, 3.f, 4.f},
                          {4.f, 3.f, 2.f, 1.f},
                          {5.f, 6.f, 7.f, 8.f},
                          {8.f, 7.f, 6.f, 5.f}};

    Siege::Vec4 result = matrix * vector;

    ASSERT_TRUE(expected == result);
}

UTEST(test_Matrix4x4, CalculateDeterminant)
{
    float expectedDeterminant = 0.f;

    Siege::Mat4 matrix = {{1.f, 2.f, 3.f, 4.f},
                          {4.f, 3.f, 2.f, 1.f},
                          {5.f, 6.f, 7.f, 8.f},
                          {8.f, 7.f, 6.f, 5.f}};

    float determinant = matrix.Determinant();
    ASSERT_EQ(expectedDeterminant, determinant);

    Siege::Mat4 matrix0 = {{5, 2, 3, 6}, {8, 4, 2, 8}, {4, 6, 9, 2}, {3, 6, 9, 2}};
    expectedDeterminant = 128.f;
    determinant = matrix0.Determinant();
    ASSERT_EQ(expectedDeterminant, determinant);

    determinant = Siege::Mat4::Determinant(matrix0);
    ASSERT_EQ(expectedDeterminant, determinant);
}

UTEST(test_Matrix4x4, CalculateInverse)
{
    Siege::Mat4 expected = {{0.f, 0.f, 1.f, -1.f},
                            {-0.53125f, 0.375f, -0.625f, 0.71875f},
                            {0.3125f, -0.25f, 0.25f, -0.1875f},
                            {0.1875f, 0.f, -0.75f, 0.6875f}};

    Siege::Mat4 matrix = {{5, 2, 3, 6}, {8, 4, 2, 8}, {4, 6, 9, 2}, {3, 6, 9, 2}};

    Siege::Mat4 result = matrix.Inverse();

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_TRUE((result[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }

    result = Siege::Mat4::Inverse(matrix);

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_TRUE((result[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix4x4, DivideMatrix)
{
    Siege::Mat4 expected = Siege::Mat4::Identity;

    Siege::Mat4 matrixA = {{5.f, 2.f, 3.f, 6.f},
                           {8.f, 4.f, 2.f, 8.f},
                           {4.f, 6.f, 9.f, 2.f},
                           {3.f, 6.f, 9.f, 2.f}};

    Siege::Mat4 matrixB = {{5.f, 2.f, 3.f, 6.f},
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
    Siege::Mat4 expected = Siege::Mat4::Identity;

    Siege::Mat4 matrixA = {{5.f, 2.f, 3.f, 6.f},
                           {8.f, 4.f, 2.f, 8.f},
                           {4.f, 6.f, 9.f, 2.f},
                           {3.f, 6.f, 9.f, 2.f}};

    Siege::Mat4 matrixB = {{5.f, 2.f, 3.f, 6.f},
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
    Siege::Mat4 expected = Siege::Mat4::Identity;

    Siege::Mat4 matrixA = {{5.f, 2.f, 3.f, 6.f},
                           {8.f, 4.f, 2.f, 8.f},
                           {4.f, 6.f, 9.f, 2.f},
                           {3.f, 6.f, 9.f, 2.f}};

    Siege::Mat4 matrixB = {{5.f, 2.f, 3.f, 6.f},
                           {8.f, 4.f, 2.f, 8.f},
                           {4.f, 6.f, 9.f, 2.f},
                           {3.f, 6.f, 9.f, 2.f}};

    Siege::Mat4 result = matrixA / matrixB;

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expected[i], result[i]);
    }
}

UTEST(test_Matrix4x4, TestTranspose)
{
    Siege::Mat4 expected = {{1.f, 5.f, 9.f, 13},
                            {2.f, 6.f, 10.f, 14.f},
                            {3.f, 7.f, 11.f, 15.f},
                            {4.f, 8.f, 12.f, 16.f}};

    Siege::Mat4 matrix = {{1.f, 2.f, 3.f, 4.f},
                          {5.f, 6.f, 7.f, 8.f},
                          {9.f, 10.f, 11.f, 12.f},
                          {13.f, 14.f, 15.f, 16.f}};

    Siege::Mat4 transposed = matrix.Transpose();
    Siege::Mat4 transposedS = Siege::Mat4::Transpose(matrix);

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_EQ(expected[i], transposed[i]);
        ASSERT_EQ(expected[i], transposedS[i]);
    }
}

UTEST(test_Matrix4x4, TestReverseOperator)
{
    Siege::Mat4 expected = {{1.f, -1.f, 1.f, -1.f},
                            {-1.f, 1.f, -1.f, 1.f},
                            {1.f, -1.f, 1.f, -1.f},
                            {-1.f, 1.f, -1.f, 1.f}};

    Siege::Mat4 matrix = {{-1.f, 1.f, -1.f, 1.f},
                          {1.f, -1.f, 1.f, -1.f},
                          {-1.f, 1.f, -1.f, 1.f},
                          {1.f, -1.f, 1.f, -1.f}};

    for (size_t i = 0; i < 16; i++)
    {
        ASSERT_TRUE(expected == -matrix);
    }
}