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

UTEST(test_Matrix, CreateEmptyMatrix)
{
    Siege::Utils::Matrix3 matrix;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(0.f, matrix[i]);
    }
}

UTEST(test_Matrix, CreateMatrixWithValues)
{
    float expectedValues[] = {1.f, 2.f, 3.f, 2.f, 4.f, 5.f, 6.f, 7.f, 9.f};

    Siege::Utils::Matrix3 matrix = {{1.f, 2.f, 3.f}, {2.f, 4.f, 5.f}, {6.f, 7.f, 9.f}};

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expectedValues[i], matrix[i]);
    }
}

UTEST(test_Matrix, CreateIdentityMatrix)
{
    float expectedValues[] = {1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f};

    Siege::Utils::Matrix3 matrix = Siege::Utils::Matrix3::Identity;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expectedValues[i], matrix[i]);
    }
}

UTEST(test_Matrix, CopyMatrixUsingConstructor)
{
    Siege::Utils::Matrix3 matrixA = Siege::Utils::Matrix3::Identity;
    Siege::Utils::Matrix3 matrixB = {matrixA};

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(matrixA[i], matrixB[i]);
    }
}

UTEST(test_Matrix, TestMatrixCopyInDifferentScopes)
{
    float expectedValues[] = {1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f};

    Siege::Utils::Matrix3 matrixA;

    { // Open a new scope. MatrixB should not be in memory once we leave the brackets
        Siege::Utils::Matrix3 matrixB = Siege::Utils::Matrix3::Identity;
        matrixA = matrixB;
    }

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixA[i]);
    }
}

UTEST(test_Matrix, GetElementInMatrix)
{
    float expected = 1;

    Siege::Utils::Matrix3 matrixA = Siege::Utils::Matrix3::Identity;

    float result0 = matrixA.Get(0, 0);
    float result1 = matrixA.Get(1, 1);

    ASSERT_EQ(expected, result0);
    ASSERT_EQ(expected, result1);
}

UTEST(test_Matrix, CheckMatrixEquality)
{
    Siege::Utils::Matrix3 matrixA = Siege::Utils::Matrix3::Identity;
    Siege::Utils::Matrix3 matrixB = {};
    Siege::Utils::Matrix3 matrixC = matrixA;

    ASSERT_EQ(matrixA, matrixC);
    ASSERT_NE(matrixA, matrixB);
    ASSERT_EQ(matrixB, Siege::Utils::Matrix3::Zero);
    ASSERT_NE(matrixB, matrixC);
}

UTEST(test_Matrix, AddMatrices)
{
    float expectedValues[] = {2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f};

    Siege::Utils::Matrix3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};
    Siege::Utils::Matrix3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrixA.Add(matrixB);

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixA[i]);
    }
}

UTEST(test_Matrix, AddMatricesWithPlusEqualsOperator)
{
    float expectedValues[] = {2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f};

    Siege::Utils::Matrix3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};
    Siege::Utils::Matrix3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrixA += matrixB;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixA[i]);
    }
}

UTEST(test_Matrix, AddMatricesAndReturnNewMatrix)
{
    float expectedValues[] = {2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f};

    Siege::Utils::Matrix3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};
    Siege::Utils::Matrix3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    auto matrixC = matrixA + matrixB;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expectedValues[i], matrixC[i]);
    }
}

UTEST(test_Matrix, SubtractMatrices)
{
    Siege::Utils::Matrix3 expected = Siege::Utils::Matrix3::Zero;

    Siege::Utils::Matrix3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};
    Siege::Utils::Matrix3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrixA.Subtract(matrixB);

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expected, matrixA);
    }
}

UTEST(test_Matrix, SubtractMatricesWithMinusEqualsOperator)
{
    Siege::Utils::Matrix3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};
    Siege::Utils::Matrix3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrixA -= matrixB;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(Siege::Utils::Matrix3::Zero, matrixA);
    }
}

UTEST(test_Matrix, SubtractMatricesAndReturnNewMatrix)
{
    Siege::Utils::Matrix3 matrixA = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};
    Siege::Utils::Matrix3 matrixB = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    Siege::Utils::Matrix3 matrixC = matrixA - matrixB;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(Siege::Utils::Matrix3::Zero, matrixC);
    }
}

UTEST(test_Matrix, MultiplyByScalar)
{
    Siege::Utils::Matrix3 expected = {{3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}};
    Siege::Utils::Matrix3 matrix = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrix.MultiplyScalar(3.f);

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expected, matrix);
    }
}

UTEST(test_Matrix, MultiplyByScalarUsingTimesEqualsOperator)
{
    Siege::Utils::Matrix3 expected = {{3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}};
    Siege::Utils::Matrix3 matrix = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    matrix *= 3.f;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expected, matrix);
    }
}

UTEST(test_Matrix, MultiplyByScalarUsingEqualsOperator)
{
    Siege::Utils::Matrix3 expected = {{3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}, {3.f, 3.f, 3.f}};
    Siege::Utils::Matrix3 matrix = {{1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}};

    Siege::Utils::Matrix3 matrixB = matrix * 3.f;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_EQ(expected, matrixB);
    }
}

UTEST(test_Matrix, MultiplyTwoMatrices)
{
    Siege::Utils::Matrix3 expected = {{26.f, 24.f, 22.f}, {18.f, 16.f, 14.f}, {70.f, 64.f, 58.f}};
    Siege::Utils::Matrix3 matrixA = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};
    Siege::Utils::Matrix3 matrixB = {{3.f, 2.f, 1.f}, {1.f, 2.f, 3.f}, {7.f, 6.f, 5.f}};

    matrixA.Multiply(matrixB);

    ASSERT_EQ(expected, matrixA);
}

UTEST(test_Matrix, MultiplyTwoMatricesUsingTimesEqualsOperator)
{
    Siege::Utils::Matrix3 expected = {{26.f, 24.f, 22.f}, {18.f, 16.f, 14.f}, {70.f, 64.f, 58.f}};
    Siege::Utils::Matrix3 matrixA = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};
    Siege::Utils::Matrix3 matrixB = {{3.f, 2.f, 1.f}, {1.f, 2.f, 3.f}, {7.f, 6.f, 5.f}};

    matrixA *= matrixB;

    ASSERT_EQ(expected, matrixA);
}

UTEST(test_Matrix, MultiplyTwoMatricesUsingTimesOperator)
{
    Siege::Utils::Matrix3 expected = {{26.f, 24.f, 22.f}, {18.f, 16.f, 14.f}, {70.f, 64.f, 58.f}};
    Siege::Utils::Matrix3 matrixA = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};
    Siege::Utils::Matrix3 matrixB = {{3.f, 2.f, 1.f}, {1.f, 2.f, 3.f}, {7.f, 6.f, 5.f}};

    Siege::Utils::Matrix3 matrixC = matrixA * matrixB;

    ASSERT_EQ(expected, matrixC);
}

UTEST(test_Matrix, MultiplyVectorByMatrix)
{
    Vec3 expected {10, 10, 30};

    Vec3 vector = {2, 1, 2};
    Siege::Utils::Matrix3 matrix = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};

    Vec3 result = matrix.Multiply(vector);

    ASSERT_TRUE(expected == result);
}

UTEST(test_Matrix, MultiplyVectorByWithTimesOperator)
{
    Vec3 expected {10, 10, 30};

    Vec3 vector = {2, 1, 2};
    Siege::Utils::Matrix3 matrix = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};

    Vec3 result = matrix * vector;

    ASSERT_TRUE(expected == result);
}

UTEST(test_Matrix, CalculateDeterminant)
{
    float expectedDeterminant = 0;
    Siege::Utils::Matrix3 matrix = {{1.f, 2.f, 3.f}, {3.f, 2.f, 1.f}, {5.f, 6.f, 7.f}};

    float determinant = matrix.Determinant();
    ASSERT_EQ(expectedDeterminant, determinant);

    Siege::Utils::Matrix3 matrix0 = {{5.f, 2.f, 3.f}, {3.f, 6.f, 1.f}, {5.f, 6.f, 7.f}};
    expectedDeterminant = 112.f;
    determinant = matrix0.Determinant();
    ASSERT_EQ(expectedDeterminant, determinant);

    determinant = Siege::Utils::Matrix3::Determinant(matrix0);
    ASSERT_EQ(expectedDeterminant, determinant);
}

UTEST(test_Matrix, CalculateInverse)
{
    Siege::Utils::Matrix3 expected = {{0.2142857f, -0.0357142f, -0.0357142f},
                                      {-0.0357142f, 0.2142857f, -0.0357142f},
                                      {-0.0357142f, -0.0357142f, 0.2142857f}};

    Siege::Utils::Matrix3 matrix = {{5.f, 1.f, 1.f}, {1.f, 5.f, 1.f}, {1.f, 1.f, 5.f}};

    Siege::Utils::Matrix3 result = matrix.Inverse();

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_TRUE((result[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }

    result = Siege::Utils::Matrix3::Inverse(matrix);

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_TRUE((result[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix, DivideMatrix)
{
    Siege::Utils::Matrix3 expected = {{9.f, -3.f, -5.f},
                                      {5.f, 1.f, -5.f},
                                      {-2.33333333333, -0.333333333333, 3.f}};

    Siege::Utils::Matrix3 matrixA = {{2.f, 4.f, 6.f}, {1.f, 5.f, 6.f}, {2.f, 4.f, 7.f}};

    Siege::Utils::Matrix3 matrixB = {{5.f, 1.f, 1.f}, {1.f, 5.f, 1.f}, {1.f, 1.f, 5.f}};

    matrixA.Divide(matrixB);

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_TRUE((matrixA[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix, DivideMatrixWithSlashEqualsOperator)
{
    Siege::Utils::Matrix3 expected = {{9.f, -3.f, -5.f},
                                      {5.f, 1.f, -5.f},
                                      {-2.33333333333, -0.333333333333, 3.f}};

    Siege::Utils::Matrix3 matrixA = {{2.f, 4.f, 6.f}, {1.f, 5.f, 6.f}, {2.f, 4.f, 7.f}};
    Siege::Utils::Matrix3 matrixB = {{5.f, 1.f, 1.f}, {1.f, 5.f, 1.f}, {1.f, 1.f, 5.f}};

    matrixA /= matrixB;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_TRUE((matrixA[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix, DivideMatrixWithSlashOperator)
{
    Siege::Utils::Matrix3 expected = {{9.f, -3.f, -5.f},
                                      {5.f, 1.f, -5.f},
                                      {-2.33333333333, -0.333333333333, 3.f}};

    Siege::Utils::Matrix3 matrixA = {{2.f, 4.f, 6.f}, {1.f, 5.f, 6.f}, {2.f, 4.f, 7.f}};
    Siege::Utils::Matrix3 matrixB = {{5.f, 1.f, 1.f}, {1.f, 5.f, 1.f}, {1.f, 1.f, 5.f}};

    Siege::Utils::Matrix3 result = matrixA / matrixB;

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_TRUE((result[0] - expected[0]) < std::numeric_limits<float>::epsilon());
    }
}

UTEST(test_Matrix, TestReverseOperator)
{
    Siege::Utils::Matrix3 expected = {{1.f, -1.f, 1.f}, {-1.f, 1.f, -1.f}, {1.f, -1.f, 1.f}};

    Siege::Utils::Matrix3 matrix = {{-1.f, 1.f, -1.f}, {1.f, -1.f, 1.f}, {-1.f, 1.f, -1.f}};

    for (size_t i = 0; i < 9; i++)
    {
        ASSERT_TRUE(expected == -matrix);
    }
}

UTEST(test_Matrix, TestMatrixExecutionTimes)
{
    srand((unsigned) time(NULL));

    float additionTime = 0.f;
    float createTime = 0.f;
    size_t size = sizeof(Siege::Utils::Matrix3);

    for (size_t i = 0; i < 1000000; i++)
    {
        float x0 = (float) (rand() % 100), y0 = (float) (rand() % 100), z0 = (float) (rand() % 100),
              x1 = (float) (rand() % 100), y1 = (float) (rand() % 100), z1 = (float) (rand() % 100),
              x2 = (float) (rand() % 100), y2 = (float) (rand() % 100), z2 = (float) (rand() % 100);

        auto startInit = std::chrono::high_resolution_clock::now();
        Siege::Utils::Matrix3 mat0 = {x0, y0, z0, x1, y1, z1, x2, y2, z2};
        createTime += std::chrono::duration<float, std::chrono::microseconds::period>(
                          std::chrono::high_resolution_clock::now() - startInit)
                          .count();

        Siege::Utils::Matrix3 mat1 = {
            {(float) (rand() % 100), (float) (rand() % 100), (float) (rand() % 100)},
            {(float) (rand() % 100), (float) (rand() % 100), (float) (rand() % 100)},
            {(float) (rand() % 100), (float) (rand() % 100), (float) (rand() % 100)}};

        auto start = std::chrono::high_resolution_clock::now();
        Siege::Utils::Matrix3 mat2 = mat0 * mat1;
        additionTime += std::chrono::duration<float, std::chrono::microseconds::period>(
                            std::chrono::high_resolution_clock::now() - start)
                            .count();
    }

    std::cout << "SIZE: " << size << std::endl;
    std::cout << "CREATION TIME: " << createTime * 0.001 << "ms" << std::endl;
    std::cout << "EXECUTION TIME: " << additionTime * 0.001 << "ms" << std::endl;
    std::cout << "AVERAGE EXECUTION TIME PER ITERATION: " << (additionTime * 0.001) / 1000000
              << "ms" << std::endl;
}

UTEST(test_Matrix, TestGlmExecutionTimes)
{
    srand((unsigned) time(NULL));

    float additionTime = 0.f;
    float createTime = 0.f;
    size_t size = sizeof(glm::mat3);

    for (size_t i = 0; i < 1000000; i++)
    {
        float x0 = (float) (rand() % 100), y0 = (float) (rand() % 100), z0 = (float) (rand() % 100),
              x1 = (float) (rand() % 100), y1 = (float) (rand() % 100), z1 = (float) (rand() % 100),
              x2 = (float) (rand() % 100), y2 = (float) (rand() % 100), z2 = (float) (rand() % 100);

        auto startInit = std::chrono::high_resolution_clock::now();
        glm::mat3 mat0 = {x0, y0, z0, x1, y1, z1, x2, y2, z2};
        createTime += std::chrono::duration<float, std::chrono::microseconds::period>(
                          std::chrono::high_resolution_clock::now() - startInit)
                          .count();

        glm::mat3 mat1 = {{(float) (rand() % 100), (float) (rand() % 100), (float) (rand() % 100)},
                          {(float) (rand() % 100), (float) (rand() % 100), (float) (rand() % 100)},
                          {(float) (rand() % 100), (float) (rand() % 100), (float) (rand() % 100)}};

        auto start = std::chrono::high_resolution_clock::now();
        glm::mat3 mat2 = mat0 * mat1;
        additionTime += std::chrono::duration<float, std::chrono::microseconds::period>(
                            std::chrono::high_resolution_clock::now() - start)
                            .count();
    }
    std::cout << "SIZE: " << size << std::endl;
    std::cout << "CREATION TIME: " << createTime * 0.001 << "ms" << std::endl;
    std::cout << "EXECUTION TIME: " << additionTime * 0.001 << "ms" << std::endl;
    std::cout << "AVERAGE EXECUTION TIME PER ITERATION: " << (additionTime * 0.001) / 1000000
              << "ms" << std::endl;
}