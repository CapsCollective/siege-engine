//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
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

UTEST(test_Mat4, CreateEmptyMatrix)
{
    Siege::Mat4 mat {};

    ASSERT_TRUE(mat == Siege::Mat4::Zero());
}

UTEST(test_Mat4, CreateConstexprEmptyMatrix)
{
    constexpr Siege::Mat4 mat {};

    ASSERT_TRUE(mat == Siege::Mat4::Zero());
}

UTEST(test_Mat4, CreateIdentityMatrix)
{
    Siege::Mat4 mat = Siege::Mat4::Identity();

    ASSERT_EQ(1, mat[0][0]);
    ASSERT_EQ(1, mat[1][1]);
    ASSERT_EQ(1, mat[2][2]);
    ASSERT_EQ(1, mat[3][3]);

    ASSERT_FALSE(mat == Siege::Mat4::Zero());
}

UTEST(test_Mat4, CreateIdentityMatrixFromScalarInitialiser)
{
    Siege::Mat4 mat {1};

    ASSERT_TRUE(mat == Siege::Mat4::Identity());

    Siege::Mat4 mat2 {2};

    ASSERT_EQ(2, mat2[0][0]);
    ASSERT_EQ(2, mat2[1][1]);
    ASSERT_EQ(2, mat2[2][2]);
    ASSERT_EQ(2, mat2[3][3]);

    ASSERT_FALSE(mat2 == Siege::Mat4::Identity());
}

UTEST(test_Mat4, CreateMat4FromMat2)
{
    Mat4 expected = {{1.f, 2.f, 0.f, 0.f},
                     {3.f, 4.f, 0.f, 0.f},
                     {0.f, 0.f, 1.f, 0.f},
                     {0.f, 0.f, 0.f, 1.f}};

    Mat2 initialMat = {{1, 2}, {3, 4}};

    Mat4 result = initialMat;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat4, CreateMat4FromMat3)
{
    Mat4 expected = {{1.f, 2.f, 3.f, 0.f},
                     {4.f, 5.f, 6.f, 0.f},
                     {7.f, 8.f, 9.f, 0.f},
                     {0.f, 0.f, 0.f, 1.f}};

    Mat3 initialMat = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    Mat4 result = initialMat;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat4, AddMatrices)
{
    Siege::Mat4 expectedMatrix = {{2, 2, 2, 2}, {2, 2, 2, 2}, {2, 2, 2, 2}, {2, 2, 2, 2}};

    Siege::Mat4 matrixA = {{1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f}};

    Siege::Mat4 matrixB = {{1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f}};

    Siege::Mat4 result = matrixA + matrixB;

    ASSERT_TRUE(result == expectedMatrix);
}

UTEST(test_Mat4, AddMatricesWithPlusEqualsOperator)
{
    Siege::Mat4 expectedMatrix = {{2, 2, 2, 2}, {2, 2, 2, 2}, {2, 2, 2, 2}, {2, 2, 2, 2}};

    Siege::Mat4 matrixA = {{1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f}};

    Siege::Mat4 matrixB = {{1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f}};

    matrixA += matrixB;

    ASSERT_TRUE(matrixA == expectedMatrix);
}

UTEST(test_Mat4, SubtractMatrices)
{
    Siege::Mat4 expected = Siege::Mat4::Zero();

    Siege::Mat4 matrixA = {{1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f}};

    Siege::Mat4 matrixB = {{1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f}};

    Siege::Mat4 result = matrixA - matrixB;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat4, SubtractMatricesWithMinusEqualsOperator)
{
    Siege::Mat4 expected = Siege::Mat4::Zero();

    Siege::Mat4 matrixA = {{1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f}};

    Siege::Mat4 matrixB = {{1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f},
                           {1.f, 1.f, 1.f, 1.f}};

    matrixA -= matrixB;

    ASSERT_TRUE(matrixA == expected);
}

UTEST(test_Mat4, MultiplyTwoMatrices)
{
    Siege::Mat4 expected = {{34.f, 36.f, 38.f, 40.f},
                            {56.f, 54.f, 52.f, 50.f},
                            {88.f, 90.f, 92.f, 94.f},
                            {110.f, 108.f, 106.f, 104.f}};

    Siege::Mat4 matrixA = {{1.f, 2.f, 3.f, 4.f},
                           {4.f, 3.f, 2.f, 1.f},
                           {5.f, 6.f, 7.f, 8.f},
                           {8.f, 7.f, 6.f, 5.f}};

    Siege::Mat4 matrixB = {{4.f, 3.f, 2.f, 1.f},
                           {1.f, 2.f, 3.f, 4.f},
                           {7.f, 6.f, 5.f, 4.f},
                           {4.f, 5.f, 6.f, 7.f}};

    Siege::Mat4 result = matrixA * matrixB;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat4, MultiplyTwoMatricesWithTimesEqualsOperator)
{
    Siege::Mat4 expected = {{34.f, 36.f, 38.f, 40.f},
                            {56.f, 54.f, 52.f, 50.f},
                            {88.f, 90.f, 92.f, 94.f},
                            {110.f, 108.f, 106.f, 104.f}};

    Siege::Mat4 matrixA = {{1.f, 2.f, 3.f, 4.f},
                           {4.f, 3.f, 2.f, 1.f},
                           {5.f, 6.f, 7.f, 8.f},
                           {8.f, 7.f, 6.f, 5.f}};

    Siege::Mat4 matrixB = {{4.f, 3.f, 2.f, 1.f},
                           {1.f, 2.f, 3.f, 4.f},
                           {7.f, 6.f, 5.f, 4.f},
                           {4.f, 5.f, 6.f, 7.f}};

    matrixA *= matrixB;

    ASSERT_TRUE(matrixA == expected);
}

UTEST(test_Mat4, MultiplyByScalar)
{
    Siege::Mat4 expected = {{3.f, 3.f, 3.f, 3.f},
                            {3.f, 3.f, 3.f, 3.f},
                            {3.f, 3.f, 3.f, 3.f},
                            {3.f, 3.f, 3.f, 3.f}};

    Siege::Mat4 matrix = {{1.f, 1.f, 1.f, 1.f},
                          {1.f, 1.f, 1.f, 1.f},
                          {1.f, 1.f, 1.f, 1.f},
                          {1.f, 1.f, 1.f, 1.f}};

    Siege::Mat4 result = matrix * 3.f;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat4, MultiplyByScalarWithTimesEqualsOperator)
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

    ASSERT_TRUE(matrix == expected);
}

UTEST(test_Mat4, MultiplyVectorByMatrix)
{
    Siege::Vec4 expected {24, 26, 28, 30};

    Siege::Vec4 vector = {2, 1, 2, 1};
    Siege::Mat4 matrix = {{1.f, 2.f, 3.f, 4.f},
                          {4.f, 3.f, 2.f, 1.f},
                          {5.f, 6.f, 7.f, 8.f},
                          {8.f, 7.f, 6.f, 5.f}};

    Siege::Vec4 result = matrix * vector;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat4, CalculateDeterminant)
{
    float expectedDeterminant = 0.f;

    Siege::Mat4 matrix = {{1.f, 2.f, 3.f, 4.f},
                          {4.f, 3.f, 2.f, 1.f},
                          {5.f, 6.f, 7.f, 8.f},
                          {8.f, 7.f, 6.f, 5.f}};

    float determinant = Siege::Mat4::Determinant(matrix);
    ASSERT_EQ(expectedDeterminant, determinant);

    Siege::Mat4 matrix0 = {{5, 2, 3, 6}, {8, 4, 2, 8}, {4, 6, 9, 2}, {3, 6, 9, 2}};
    expectedDeterminant = 128.f;
    determinant = Siege::Mat4::Determinant(matrix0);
    ASSERT_EQ(expectedDeterminant, determinant);

    determinant = Siege::Mat4::Determinant(matrix0);
    ASSERT_EQ(expectedDeterminant, determinant);
}

UTEST(test_Mat4, CalculateInverse)
{
    Siege::Mat4 expected = {{0.f, 0.f, 1.f, -1.f},
                            {-0.53125f, 0.375f, -0.625f, 0.71875f},
                            {0.3125f, -0.25f, 0.25f, -0.1875f},
                            {0.1875f, 0.f, -0.75f, 0.6875f}};

    Siege::Mat4 matrix = {{5, 2, 3, 6}, {8, 4, 2, 8}, {4, 6, 9, 2}, {3, 6, 9, 2}};

    Siege::Mat4 result = Siege::Mat4::Inverse(matrix);

    ASSERT_TRUE(result == expected);

    result = Siege::Mat4::Inverse(matrix);

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat4, DivideMatrix)
{
    Siege::Mat4 expected = Siege::Mat4::Identity();

    Siege::Mat4 matrixA = {{5.f, 2.f, 3.f, 6.f},
                           {8.f, 4.f, 2.f, 8.f},
                           {4.f, 6.f, 9.f, 2.f},
                           {3.f, 6.f, 9.f, 2.f}};

    Siege::Mat4 matrixB = {{5.f, 2.f, 3.f, 6.f},
                           {8.f, 4.f, 2.f, 8.f},
                           {4.f, 6.f, 9.f, 2.f},
                           {3.f, 6.f, 9.f, 2.f}};

    Siege::Mat4 result = matrixA / matrixB;

    ASSERT_TRUE(result == expected);
}

UTEST(test_Mat4, DivideMatrixWithSlashEqualsOperator)
{
    Siege::Mat4 expected = Siege::Mat4::Identity();

    Siege::Mat4 matrixA = {{5.f, 2.f, 3.f, 6.f},
                           {8.f, 4.f, 2.f, 8.f},
                           {4.f, 6.f, 9.f, 2.f},
                           {3.f, 6.f, 9.f, 2.f}};

    Siege::Mat4 matrixB = {{5.f, 2.f, 3.f, 6.f},
                           {8.f, 4.f, 2.f, 8.f},
                           {4.f, 6.f, 9.f, 2.f},
                           {3.f, 6.f, 9.f, 2.f}};

    matrixA /= matrixB;

    ASSERT_TRUE(matrixA == expected);
}

UTEST(test_Mat4, TransposeMatrix)
{
    Siege::Mat4 expected = {{1.f, 5.f, 9.f, 13},
                            {2.f, 6.f, 10.f, 14.f},
                            {3.f, 7.f, 11.f, 15.f},
                            {4.f, 8.f, 12.f, 16.f}};

    Siege::Mat4 matrix = {{1.f, 2.f, 3.f, 4.f},
                          {5.f, 6.f, 7.f, 8.f},
                          {9.f, 10.f, 11.f, 12.f},
                          {13.f, 14.f, 15.f, 16.f}};

    Siege::Mat4 transposed = Siege::Mat4::Transpose(matrix);

    ASSERT_TRUE(transposed == expected);
}

UTEST(test_Mat4, NegateMatrix)
{
    Siege::Mat4 expected = {{1.f, -1.f, 1.f, -1.f},
                            {-1.f, 1.f, -1.f, 1.f},
                            {1.f, -1.f, 1.f, -1.f},
                            {-1.f, 1.f, -1.f, 1.f}};

    Siege::Mat4 matrix = {{-1.f, 1.f, -1.f, 1.f},
                          {1.f, -1.f, 1.f, -1.f},
                          {-1.f, 1.f, -1.f, 1.f},
                          {1.f, -1.f, 1.f, -1.f}};

    ASSERT_TRUE(-matrix == expected);
}

UTEST(test_Mat4, ConvertMatrixToString)
{
    String str = "{1.00,2.00,3.00,4.00}\n{5.00,6.00,7.00,8.00}\n{9.00,10.00,11.00,12.00}\n{13.00,"
                 "14.00,15.00,16.00}";

    Mat4 matrix = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};

    String result = ToString(matrix);

    ASSERT_TRUE(str == result);
}

UTEST(test_Mat4, CheckMatrixEquality)
{
    Mat4 matrixA = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};

    Mat4 matrixB = {{0.99999999999912, 2, 3, 4},
                    {5, 6, 7, 8},
                    {8.99999999999999999, 10, 11, 12},
                    {13, 14, 15, 16}};

    ASSERT_TRUE(Equals(matrixA, matrixB, Epsilon<float>()));
}