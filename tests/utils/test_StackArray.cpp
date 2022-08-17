//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include <utest.h>
#include <utils/StackArray.h>

#include <utility>
#include <cmath>

//---------------------------------------- SArray --------------------------------------------------

UTEST(test_SArray, CreateEmptySArray)
{
    Siege::Utils::SArray<uint32_t, 1> array = {};

    ASSERT_TRUE(array.Size() == 1);
    ASSERT_TRUE(array.Data() != nullptr);
}

UTEST(test_SArray, CreateSArrayWithInitialiserList)
{
    Siege::Utils::SArray<uint32_t, 3> array = {1, 2, 3};

    ASSERT_TRUE(array.Size() == 3);
    ASSERT_TRUE(array.Data() != nullptr);

    ASSERT_EQ(1, array[0]);
    ASSERT_EQ(2, array[1]);
    ASSERT_EQ(3, array[2]);
}

UTEST(test_SArray, CreateConstexprSArray)
{
    constexpr Siege::Utils::SArray<uint32_t, 3> array = {1, 2, 3};

    ASSERT_TRUE(array.Size() == 3);
    ASSERT_TRUE(array.Data() != nullptr);

    ASSERT_EQ(1, array[0]);
    ASSERT_EQ(2, array[1]);
    ASSERT_EQ(3, array[2]);
}

UTEST(test_SArray, CreateSArrayWithCopyConstructor)
{
    Siege::Utils::SArray<uint32_t, 3> arrayA = {2, 3, 4};

    Siege::Utils::SArray<uint32_t, 3> arrayB(arrayA);

    ASSERT_TRUE(arrayB.Size() == 3);
    ASSERT_TRUE(arrayB.Data() != nullptr);

    ASSERT_EQ(arrayA[0], arrayB[0]);
    ASSERT_EQ(arrayA[1], arrayB[1]);
    ASSERT_EQ(arrayA[2], arrayB[2]);
}

UTEST(test_SArray, CreateSArrayFromMoveConstructor)
{
    Siege::Utils::SArray<uint32_t, 4> arrayA = {1, 2, 3, 4};

    ASSERT_EQ(arrayA.Size(), 4);
    ASSERT_NE(arrayA.Data(), nullptr);

    ASSERT_EQ(arrayA[0], 1);
    ASSERT_EQ(arrayA[1], 2);
    ASSERT_EQ(arrayA[2], 3);
    ASSERT_EQ(arrayA[3], 4);

    Siege::Utils::SArray<uint32_t, 4> arrayB(std::move(arrayA));

    ASSERT_EQ(arrayB.Size(), 4);
    ASSERT_NE(arrayB.Data(), nullptr);

    ASSERT_EQ(arrayB[0], 1);
    ASSERT_EQ(arrayB[1], 2);
    ASSERT_EQ(arrayB[2], 3);
    ASSERT_EQ(arrayB[3], 4);
}

UTEST(test_SArray, CheckIndexInBounds)
{
    Siege::Utils::SArray<uint32_t, 1> array;

    ASSERT_TRUE(array.IsInBounds(0));
    ASSERT_FALSE(array.IsInBounds(1));
}

UTEST(test_SArray, InsertElementUsingFunction)
{
    Siege::Utils::SArray<uint32_t, 2> array;

    ASSERT_EQ(array.Size(), 2);

    array.Insert(0, 1);
    array.Insert(1, 2);

    ASSERT_EQ(1, array[0]);
    ASSERT_EQ(2, array[1]);
}

UTEST(test_SArray, InsertElementUsingBrackets)
{
    Siege::Utils::SArray<uint32_t, 2> array;

    ASSERT_EQ(array.Size(), 2);

    array[0] = 1;
    array[1] = 2;

    ASSERT_EQ(1, array[0]);
    ASSERT_EQ(2, array[1]);
}

UTEST(test_SArray, GetElementUsingFunction)
{
    Siege::Utils::SArray<uint32_t, 2> array;

    array[0] = 1;

    ASSERT_EQ(1, array.Get(0));

    array[1] = 2;

    ASSERT_EQ(2, array.Get(1));
}

UTEST(test_SArray, GetElementUsingBracket)
{
    Siege::Utils::SArray<uint32_t, 2> array;

    array[0] = 1;

    ASSERT_EQ(1, array[0]);

    array[1] = 2;

    ASSERT_EQ(2, array[1]);
}

UTEST(test_SArray, CopySArray)
{
    Siege::Utils::SArray<uint32_t, 2> arrayA = {0, 1};
    Siege::Utils::SArray<uint32_t, 2> arrayB = {3, 4};

    arrayA = arrayB;

    ASSERT_EQ(arrayA[0], arrayB[0]);
    ASSERT_EQ(arrayA[1], arrayB[1]);

    ASSERT_EQ(arrayA.Size(), arrayB.Size());
}

UTEST(test_SArray, CopyEmptySArray)
{
    Siege::Utils::SArray<uint32_t, 0> arrayA {};
    Siege::Utils::SArray<uint32_t, 0> arrayB(arrayA);

    arrayA = arrayB;

    ASSERT_EQ(arrayA.Size(), arrayB.Size());
}

UTEST(test_SArray, MoveSArray)
{
    Siege::Utils::SArray<uint32_t, 2> arrayA = {1, 2};

    Siege::Utils::SArray<uint32_t, 2> arrayB = {3, 4};

    ASSERT_EQ(arrayB.Size(), 2);

    ASSERT_NE(arrayB.Data(), nullptr);

    ASSERT_EQ(arrayB[0], 3);
    ASSERT_EQ(arrayB[1], 4);

    arrayA = std::move(arrayB);

    ASSERT_EQ(arrayA.Size(), 2);

    ASSERT_EQ(arrayA[0], 3);
    ASSERT_EQ(arrayA[1], 4);
}

UTEST(test_SArray, IterateUsingBrackets)
{
    Siege::Utils::SArray<uint32_t, 4> array({1, 2, 3, 4});

    uint32_t expectedResults[] = {1, 2, 3, 4};

    for (size_t i = 0; i < array.Size(); i++)
    {
        ASSERT_EQ(array[i], expectedResults[i]);
    }
}

UTEST(test_SArray, IterateUsingIterator)
{
    Siege::Utils::SArray<uint32_t, 4> array({1, 2, 3, 4});

    uint32_t expectedResults[] = {1, 2, 3, 4};

    size_t index = 0;
    for (auto& element : array)
    {
        ASSERT_EQ(expectedResults[index++], element);
    }
}

UTEST(test_SArray, IterateOverSingleElementSArray)
{
    Siege::Utils::SArray<uint32_t, 1> array = {1};

    uint32_t expectedResults[] = {1};

    size_t index = 0;
    for (auto& element : array)
    {
        ASSERT_EQ(expectedResults[index++], element);
    }
}

//---------------------------------------- MSArray -------------------------------------------------

UTEST(test_MSArray, CreateEmptyMSArray)
{
    Siege::Utils::MSArray<uint32_t, 0> array;

    ASSERT_EQ(0, array.Size());
    ASSERT_EQ(0, array.Count());
    ASSERT_TRUE(array.Data() != nullptr);
}

UTEST(test_MSArray, CreateMSArrayWithSize)
{
    Siege::Utils::MSArray<uint32_t, 2> array;

    ASSERT_EQ(2, array.Size());
    ASSERT_EQ(0, array.Count());
    ASSERT_FALSE(array.Data() == nullptr);
}

UTEST(test_MSArray, CreateMSArrayWithInitializerList)
{
    Siege::Utils::MSArray<uint32_t, 2> array = {0, 1};

    ASSERT_TRUE(array.Size() == 2);
    ASSERT_TRUE(array.Count() == 2);
    ASSERT_FALSE(array.Data() == nullptr);

    ASSERT_EQ(array[0], 0);
    ASSERT_EQ(array[1], 1);

    ASSERT_EQ(2, array.Count());
}

UTEST(test_MSArray, CreateMSArrayWithLargerInitializerList)
{
    Siege::Utils::MSArray<uint32_t, 2> array = {0, 1, 2, 3, 4, 5};

    ASSERT_TRUE(array.Size() == 2);
    ASSERT_TRUE(array.Count() == 2);
    ASSERT_FALSE(array.Data() == nullptr);

    ASSERT_EQ(array[0], 0);
    ASSERT_EQ(array[1], 1);

    ASSERT_EQ(2, array.Count());
}

UTEST(test_MSArray, CreateMSArrayWithSmallerInitializerList)
{
    Siege::Utils::MSArray<uint32_t, 4> array = {0, 1};

    ASSERT_TRUE(array.Size() == 4);
    ASSERT_TRUE(array.Count() == 2);
    ASSERT_FALSE(array.Data() == nullptr);

    ASSERT_EQ(array[0], 0);
    ASSERT_EQ(array[1], 1);

    ASSERT_EQ(2, array.Count());

    ASSERT_FALSE(array.Active(2));
    ASSERT_FALSE(array.Active(3));
}

UTEST(test_MSArray, CreateMSArrayFromCopyConstructor)
{
    Siege::Utils::MSArray<uint32_t, 2> arrayA = {0, 1};
    Siege::Utils::MSArray<uint32_t, 2> arrayB(arrayA);

    ASSERT_EQ(arrayA.Size(), arrayB.Size());
    ASSERT_EQ(arrayA.Count(), arrayB.Count());

    ASSERT_EQ(arrayA[0], arrayB[0]);
    ASSERT_EQ(arrayA[1], arrayB[1]);
}

UTEST(test_MSArray, CopyEmptyArrayWithConstructor)
{
    Siege::Utils::MSArray<uint32_t, 0> arrayA;

    Siege::Utils::MSArray<uint32_t, 0> arrayB(arrayA);

    ASSERT_NE(arrayA.Data(), arrayB.Data());
    ASSERT_EQ(arrayA.Count(), arrayB.Count());
    ASSERT_EQ(arrayA.Size(), arrayB.Size());
}

UTEST(test_MSArray, CreateMSArrayFromMoveConstructor)
{
    Siege::Utils::MSArray<uint32_t, 4> arrayA = {1, 2, 3, 4};

    ASSERT_EQ(arrayA.Count(), 4);
    ASSERT_EQ(arrayA.Size(), 4);
    ASSERT_NE(arrayA.Data(), nullptr);

    ASSERT_EQ(arrayA[0], 1);
    ASSERT_EQ(arrayA[1], 2);
    ASSERT_EQ(arrayA[2], 3);
    ASSERT_EQ(arrayA[3], 4);

    Siege::Utils::MSArray<uint32_t, 4> arrayB(std::move(arrayA));

    ASSERT_EQ(arrayB.Count(), 4);
    ASSERT_EQ(arrayB.Size(), 4);
    ASSERT_NE(arrayB.Data(), nullptr);

    ASSERT_EQ(arrayB[0], 1);
    ASSERT_EQ(arrayB[1], 2);
    ASSERT_EQ(arrayB[2], 3);
    ASSERT_EQ(arrayB[3], 4);
}

UTEST(test_MSArray, CreateMSArrayFromRawArray)
{
    uint32_t values[] = {1, 2, 3, 4};
    Siege::Utils::MSArray<uint32_t, 4> array(values, 4);

    ASSERT_EQ(array.Count(), 4);
    ASSERT_EQ(array.Size(), 4);
    ASSERT_NE(array.Data(), nullptr);

    ASSERT_TRUE(array.Active(0));
    ASSERT_TRUE(array.Active(1));
    ASSERT_TRUE(array.Active(2));
    ASSERT_TRUE(array.Active(3));

    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 3);
    ASSERT_EQ(array[3], 4);
}

UTEST(test_MSArray, CreateMSArrayFromLargerRawArray)
{
    uint32_t values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    Siege::Utils::MSArray<uint32_t, 4> array(values, 9);

    ASSERT_EQ(array.Count(), 4);
    ASSERT_EQ(array.Size(), 4);
    ASSERT_NE(array.Data(), nullptr);

    ASSERT_TRUE(array.Active(0));
    ASSERT_TRUE(array.Active(1));
    ASSERT_TRUE(array.Active(2));
    ASSERT_TRUE(array.Active(3));

    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 3);
    ASSERT_EQ(array[3], 4);
}

UTEST(test_MSArray, CreateMSArrayFromSmallerRawArray)
{
    uint32_t values[] = {1, 2};
    Siege::Utils::MSArray<uint32_t, 4> array(values, 2);

    ASSERT_EQ(array.Count(), 2);
    ASSERT_EQ(array.Size(), 4);
    ASSERT_NE(array.Data(), nullptr);

    ASSERT_TRUE(array.Active(0));
    ASSERT_TRUE(array.Active(1));
    ASSERT_FALSE(array.Active(2));
    ASSERT_FALSE(array.Active(3));

    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
}

UTEST(test_MSArray, ClearArray)
{
    Siege::Utils::MSArray<uint32_t, 4> array = {1, 2, 3, 4};
    ASSERT_EQ(array.Count(), 4);
    ASSERT_EQ(array.Size(), 4);
    ASSERT_NE(array.Data(), nullptr);

    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 3);
    ASSERT_EQ(array[3], 4);

    array.Clear();

    ASSERT_EQ(array.Count(), 0);
}

UTEST(test_MSArray, TestIsEmpty)
{
    Siege::Utils::MSArray<uint32_t, 4> array = {1, 2, 3, 4};
    ASSERT_EQ(array.Count(), 4);
    ASSERT_EQ(array.Size(), 4);
    ASSERT_NE(array.Data(), nullptr);

    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 3);
    ASSERT_EQ(array[3], 4);

    ASSERT_FALSE(array.Empty());

    array.Clear();

    ASSERT_EQ(array.Count(), 0);
    ASSERT_TRUE(array.Empty());
}

UTEST(test_MSArray, InsertElementUsingFunction)
{
    Siege::Utils::MSArray<uint32_t, 2> array;

    ASSERT_TRUE(array.Empty());

    array.Insert(0, 1);

    ASSERT_FALSE(array.Empty());
    ASSERT_EQ(1, array.Count());

    array.Insert(1, 2);

    ASSERT_EQ(2, array.Count());

    ASSERT_EQ(1, array[0]);
    ASSERT_EQ(2, array[1]);
}

UTEST(test_MSArray, InsertElementUsingBrackets)
{
    Siege::Utils::MSArray<uint32_t, 2> array;

    ASSERT_TRUE(array.Empty());

    array[0] = 1;

    ASSERT_FALSE(array.Empty());
    ASSERT_EQ(1, array.Count());

    array[1] = 2;

    ASSERT_EQ(2, array.Count());

    ASSERT_EQ(1, array[0]);
    ASSERT_EQ(2, array[1]);
}

UTEST(test_MSArray, AppendValuesToArray)
{
    Siege::Utils::MSArray<uint32_t, 2> array;

    array.Append(1);

    ASSERT_EQ(1, array.Count());
    ASSERT_TRUE(array.Active(0));
    ASSERT_EQ(2, array.Size());

    array.Append(2);

    ASSERT_EQ(2, array.Count());
    ASSERT_TRUE(array.Active(1));
    ASSERT_EQ(2, array.Size());

    ASSERT_EQ(1, array[0]);
    ASSERT_EQ(2, array[1]);
}

UTEST(test_MSArray, GetElementUsingFunction)
{
    Siege::Utils::MSArray<uint32_t, 2> array;

    array[0] = 1;

    ASSERT_EQ(array.Get(0), 1);

    array[1] = 2;

    ASSERT_EQ(array.Get(1), 2);
}

UTEST(test_MSArray, GetElementUsingBrackets)
{
    Siege::Utils::MSArray<uint32_t, 2> array;

    array[0] = 1;

    ASSERT_EQ(array[0], 1);

    array[1] = 2;

    ASSERT_EQ(array[1], 2);
}

UTEST(test_MSArray, CheckElementIsActive)
{
    Siege::Utils::MSArray<uint32_t, 2> array;

    array[0] = 1;

    ASSERT_TRUE(array.Active(0));
    ASSERT_FALSE(array.Active(1));

    array[1] = 2;

    ASSERT_TRUE(array.Active(1));
}

UTEST(test_MSArray, RemoveElementByIndex)
{
    Siege::Utils::MSArray<uint32_t, 2> array;

    array[0] = 1;

    ASSERT_EQ(array.Count(), 1);
    ASSERT_TRUE(array.Active(0));

    array[1] = 2;

    ASSERT_EQ(array.Count(), 2);
    ASSERT_TRUE(array.Active(1));

    array.Remove(1);

    ASSERT_EQ(array.Count(), 1);
    ASSERT_FALSE(array.Active(1));

    array.Remove(0);

    ASSERT_EQ(array.Count(), 0);
    ASSERT_FALSE(array.Active(0));
    ASSERT_TRUE(array.Empty());
}

UTEST(test_MSArray, CopyArray)
{
    Siege::Utils::MSArray<uint32_t, 2> arrayA = {0, 1};
    Siege::Utils::MSArray<uint32_t, 2> arrayB = {3, 4};

    arrayA = arrayB;

    ASSERT_EQ(arrayA[0], arrayB[0]);
    ASSERT_EQ(arrayA[1], arrayB[1]);

    ASSERT_EQ(arrayA.Size(), arrayB.Size());
    ASSERT_EQ(arrayA.Count(), arrayB.Count());
}

UTEST(test_MSArray, CopyEmptyArray)
{
    Siege::Utils::MSArray<uint32_t, 0> arrayA;
    Siege::Utils::MSArray<uint32_t, 0> arrayB(arrayA);

    arrayA = arrayB;

    ASSERT_EQ(arrayA.Size(), arrayB.Size());
    ASSERT_EQ(arrayA.Count(), arrayB.Count());
}

UTEST(test_MSArray, CopyEmptyArrayToFullArray)
{
    Siege::Utils::MSArray<uint32_t, 4> arrayA;

    Siege::Utils::MSArray<uint32_t, 4> arrayB = {1, 2, 3, 4};

    arrayB = arrayA;

    ASSERT_EQ(arrayA.Count(), arrayB.Count());
    ASSERT_EQ(arrayA.Size(), arrayB.Size());
}

UTEST(test_MSArray, MoveArray)
{
    Siege::Utils::MSArray<uint32_t, 2> arrayA;

    ASSERT_EQ(arrayA.Size(), 2);
    ASSERT_EQ(arrayA.Count(), 0);

    Siege::Utils::MSArray<uint32_t, 2> arrayB = {3, 4};

    ASSERT_EQ(arrayB.Size(), 2);
    ASSERT_EQ(arrayB.Count(), 2);

    ASSERT_NE(arrayB.Data(), nullptr);

    ASSERT_EQ(arrayB[0], 3);
    ASSERT_EQ(arrayB[1], 4);

    arrayA = std::move(arrayB);

    ASSERT_EQ(arrayA.Size(), 2);
    ASSERT_EQ(arrayA.Count(), 2);

    ASSERT_EQ(arrayA[0], 3);
    ASSERT_EQ(arrayA[1], 4);
}

UTEST(test_MSArray, IterateUsingBrackets)
{
    Siege::Utils::MSArray<uint32_t, 4> array({1, 2, 3, 4});

    uint32_t expectedResults[] = {1, 2, 3, 4};

    for (size_t i = 0; i < array.Count(); i++)
    {
        ASSERT_EQ(array[i], expectedResults[i]);
    }
}

UTEST(test_MSArray, IterateUsingRangedForLoop)
{
    Siege::Utils::MSArray<uint32_t, 4> array = {1, 2, 3, 4};

    uint32_t expectedResults[] = {1, 2, 3, 4};

    const size_t ARR_COUNT = 4;
    uint32_t actualResults[ARR_COUNT];

    size_t index = 0;
    for (auto& element : array)
    {
        actualResults[index++] = element;
    }

    for(size_t i = 0; i < ARR_COUNT; i++)
    {
        ASSERT_EQ(expectedResults[i], actualResults[i]);
    }
}

UTEST(test_MSArray, IterateUsingRangedForLoopWithPartiallyFilledArray)
{
    Siege::Utils::MSArray<uint32_t, 4> array;

    array[0] = 1;
    array[1] = 2;
    array[3] = 4;

    uint32_t expectedResults[] = {1, 2, 4};

    const size_t ARR_COUNT = 3;
    uint32_t actualResults[ARR_COUNT];

    size_t index = 0;
    for (auto& element : array)
    {
        actualResults[index++] = element;
    }

    for(size_t i = 0; i < ARR_COUNT; i++)
    {
        ASSERT_EQ(expectedResults[i], actualResults[i]);
    }
}

UTEST(test_MSArray, IterateOverSingleElementArray)
{
    Siege::Utils::MSArray<uint32_t, 1> array = {1};

    uint32_t expectedResults[] = {1};

    const size_t ARR_COUNT = 1;
    uint32_t actualResults[ARR_COUNT];

    size_t index = 0;
    for (auto& element : array)
    {
        actualResults[index++] = element;
    }

    for(size_t i = 0; i < ARR_COUNT; i++)
    {
        ASSERT_EQ(expectedResults[i], actualResults[i]);
    }
}

UTEST(test_MSArray, IterateOverLargeArray)
{
    Siege::Utils::MSArray<uint32_t, 20> array = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                                                 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

    uint32_t expectedResults[] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                                  11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

    const size_t ARR_COUNT = 20;
    uint32_t actualResults[ARR_COUNT];

    size_t index = 0;
    for (auto& element : array)
    {
        actualResults[index++] = element;
    }

    for(size_t i = 0; i < ARR_COUNT; i++)
    {
        ASSERT_EQ(expectedResults[i], actualResults[i]);
    }
}

UTEST(test_MSArray, IterateOverEmptyArray)
{
    Siege::Utils::MSArray<uint32_t, 20> array;

    size_t index = 0;
    for (auto& element : array)
    {
        element = element+1;
        index++;
    }

    ASSERT_EQ(0, index);
}

UTEST(test_MSArray, IterateOverPartiallyFilledArray)
{
    Siege::Utils::MSArray<uint32_t, 20> array;
    int expected[] = {2, 1};

    array[1] = 2;
    array[3] = 1;

    const size_t ARR_COUNT = 2;
    uint32_t actualResults[ARR_COUNT];

    size_t index = 0;
    for (auto& element : array)
    {
        actualResults[index++] = element;
    }

    for(size_t i = 0; i < ARR_COUNT; i++)
    {
        ASSERT_EQ(expected[i], actualResults[i]);
    }
}

UTEST(test_MSArray, IterateOverArrayWithMissingFrontAndEnd)
{
    Siege::Utils::MSArray<uint32_t, 4> array;
    int expected[] = {2, 1};

    array[1] = 2;
    array[2] = 1;

    const size_t ARR_COUNT = 2;
    uint32_t actualResults[ARR_COUNT];

    size_t index = 0;
    for (auto& element : array)
    {
        actualResults[index++] = element;
    }

    for(size_t i = 0; i < ARR_COUNT; i++)
    {
        ASSERT_EQ(expected[i], actualResults[i]);
    }
}


UTEST(test_MSArray, IterateOverArrayWithLargeGaps)
{
    Siege::Utils::MSArray<uint32_t, 10> array;
    int expected[] = {2, 1, 3};

    array[1] = 2;
    array[5] = 1;
    array[8] = 3;

    const size_t ARR_COUNT = 3;
    uint32_t actualResults[ARR_COUNT];

    size_t index = 0;
    for (auto& element : array)
    {
        actualResults[index++] = element;
    }

    for(size_t i = 0; i < ARR_COUNT; i++)
    {
        ASSERT_EQ(expected[i], actualResults[i]);
    }
}