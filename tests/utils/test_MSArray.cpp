//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include <utest.h>

#include <cmath>
#include <utility>

#include "utils/collections/StackArray.h"

using Siege::MSArray;

UTEST(test_MSArray, CreateEmptyMSArray)
{
    MSArray<uint32_t, 0> array;

    ASSERT_EQ(0, array.Size());
    ASSERT_EQ(0, array.Count());
    ASSERT_TRUE(array.Data() != nullptr);
}

UTEST(test_MSArray, CreateMSArrayWithSize)
{
    MSArray<uint32_t, 2> array;

    ASSERT_EQ(2, array.Size());
    ASSERT_EQ(0, array.Count());
    ASSERT_FALSE(array.Data() == nullptr);
}

UTEST(test_MSArray, CreateMSArrayWithInitializerList)
{
    MSArray<uint32_t, 2> array = {0, 1};

    ASSERT_TRUE(array.Size() == 2);
    ASSERT_TRUE(array.Count() == 2);
    ASSERT_FALSE(array.Data() == nullptr);

    ASSERT_EQ(array[0], 0);
    ASSERT_EQ(array[1], 1);

    ASSERT_EQ(2, array.Count());
}

UTEST(test_MSArray, CreateMSArrayWithLargerInitializerList)
{
    MSArray<uint32_t, 2> array = {0, 1, 2, 3, 4, 5};

    ASSERT_TRUE(array.Size() == 2);
    ASSERT_TRUE(array.Count() == 2);
    ASSERT_FALSE(array.Data() == nullptr);

    ASSERT_EQ(array[0], 0);
    ASSERT_EQ(array[1], 1);

    ASSERT_EQ(2, array.Count());
}

UTEST(test_MSArray, CreateMSArrayWithSmallerInitializerList)
{
    MSArray<uint32_t, 4> array = {0, 1};

    ASSERT_TRUE(array.Size() == 4);
    ASSERT_TRUE(array.Count() == 2);
    ASSERT_FALSE(array.Data() == nullptr);

    ASSERT_EQ(array[0], 0);
    ASSERT_EQ(array[1], 1);

    ASSERT_EQ(2, array.Count());

    ASSERT_FALSE(array.IsActive(2));
    ASSERT_FALSE(array.IsActive(3));
}

UTEST(test_MSArray, CreateMSArrayFromCopyConstructor)
{
    MSArray<uint32_t, 2> arrayA = {0, 1};
    MSArray<uint32_t, 2> arrayB(arrayA);

    ASSERT_EQ(arrayA.Size(), arrayB.Size());
    ASSERT_EQ(arrayA.Count(), arrayB.Count());

    ASSERT_EQ(arrayA[0], arrayB[0]);
    ASSERT_EQ(arrayA[1], arrayB[1]);
}

UTEST(test_MSArray, CopyEmptyArrayWithConstructor)
{
    MSArray<uint32_t, 0> arrayA;

    MSArray<uint32_t, 0> arrayB(arrayA);

    ASSERT_NE(arrayA.Data(), arrayB.Data());
    ASSERT_EQ(arrayA.Count(), arrayB.Count());
    ASSERT_EQ(arrayA.Size(), arrayB.Size());
}

UTEST(test_MSArray, CreateMSArrayFromMoveConstructor)
{
    MSArray<uint32_t, 4> arrayA = {1, 2, 3, 4};

    ASSERT_EQ(arrayA.Count(), 4);
    ASSERT_EQ(arrayA.Size(), 4);
    ASSERT_NE(arrayA.Data(), nullptr);

    ASSERT_EQ(arrayA[0], 1);
    ASSERT_EQ(arrayA[1], 2);
    ASSERT_EQ(arrayA[2], 3);
    ASSERT_EQ(arrayA[3], 4);

    MSArray<uint32_t, 4> arrayB(std::move(arrayA));

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
    MSArray<uint32_t, 4> array(values, 4);

    ASSERT_EQ(array.Count(), 4);
    ASSERT_EQ(array.Size(), 4);
    ASSERT_NE(array.Data(), nullptr);

    ASSERT_TRUE(array.IsActive(0));
    ASSERT_TRUE(array.IsActive(1));
    ASSERT_TRUE(array.IsActive(2));
    ASSERT_TRUE(array.IsActive(3));

    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 3);
    ASSERT_EQ(array[3], 4);
}

UTEST(test_MSArray, CreateMSArrayFromLargerRawArray)
{
    uint32_t values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    MSArray<uint32_t, 4> array(values, 9);

    ASSERT_EQ(array.Count(), 4);
    ASSERT_EQ(array.Size(), 4);
    ASSERT_NE(array.Data(), nullptr);

    ASSERT_TRUE(array.IsActive(0));
    ASSERT_TRUE(array.IsActive(1));
    ASSERT_TRUE(array.IsActive(2));
    ASSERT_TRUE(array.IsActive(3));

    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 3);
    ASSERT_EQ(array[3], 4);
}

UTEST(test_MSArray, CreateMSArrayFromSmallerRawArray)
{
    uint32_t values[] = {1, 2};
    MSArray<uint32_t, 4> array(values, 2);

    ASSERT_EQ(array.Count(), 2);
    ASSERT_EQ(array.Size(), 4);
    ASSERT_NE(array.Data(), nullptr);

    ASSERT_TRUE(array.IsActive(0));
    ASSERT_TRUE(array.IsActive(1));
    ASSERT_FALSE(array.IsActive(2));
    ASSERT_FALSE(array.IsActive(3));

    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
}

UTEST(test_MSArray, ClearArray)
{
    MSArray<uint32_t, 4> array = {1, 2, 3, 4};
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
    MSArray<uint32_t, 4> array = {1, 2, 3, 4};
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
    MSArray<uint32_t, 2> array;

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
    MSArray<uint32_t, 2> array;

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
    MSArray<uint32_t, 2> array;

    array.Append(1);

    ASSERT_EQ(1, array.Count());
    ASSERT_TRUE(array.IsActive(0));
    ASSERT_EQ(2, array.Size());

    array.Append(2);

    ASSERT_EQ(2, array.Count());
    ASSERT_TRUE(array.IsActive(1));
    ASSERT_EQ(2, array.Size());

    ASSERT_EQ(1, array[0]);
    ASSERT_EQ(2, array[1]);
}

UTEST(test_MSArray, GetElementUsingFunction)
{
    MSArray<uint32_t, 2> array;

    array[0] = 1;

    ASSERT_EQ(array.Get(0), 1);

    array[1] = 2;

    ASSERT_EQ(array.Get(1), 2);
}

UTEST(test_MSArray, GetElementUsingBrackets)
{
    MSArray<uint32_t, 2> array;

    array[0] = 1;

    ASSERT_EQ(array[0], 1);

    array[1] = 2;

    ASSERT_EQ(array[1], 2);
}

UTEST(test_MSArray, CheckElementIsActive)
{
    MSArray<uint32_t, 2> array;

    array[0] = 1;

    ASSERT_TRUE(array.IsActive(0));
    ASSERT_FALSE(array.IsActive(1));

    array[1] = 2;

    ASSERT_TRUE(array.IsActive(1));
}

UTEST(test_MSArray, RemoveElementByIndex)
{
    MSArray<uint32_t, 2> array;

    array[0] = 1;

    ASSERT_EQ(array.Count(), 1);
    ASSERT_TRUE(array.IsActive(0));

    array[1] = 2;

    ASSERT_EQ(array.Count(), 2);
    ASSERT_TRUE(array.IsActive(1));

    array.Remove(1);

    ASSERT_EQ(array.Count(), 1);
    ASSERT_FALSE(array.IsActive(1));

    array.Remove(0);

    ASSERT_EQ(array.Count(), 0);
    ASSERT_FALSE(array.IsActive(0));
    ASSERT_TRUE(array.Empty());
}

UTEST(test_MSArray, CopyArray)
{
    MSArray<uint32_t, 2> arrayA = {0, 1};
    MSArray<uint32_t, 2> arrayB = {3, 4};

    arrayA = arrayB;

    ASSERT_EQ(arrayA[0], arrayB[0]);
    ASSERT_EQ(arrayA[1], arrayB[1]);

    ASSERT_EQ(arrayA.Size(), arrayB.Size());
    ASSERT_EQ(arrayA.Count(), arrayB.Count());
}

UTEST(test_MSArray, CopyEmptyArray)
{
    MSArray<uint32_t, 0> arrayA;
    MSArray<uint32_t, 0> arrayB(arrayA);

    arrayA = arrayB;

    ASSERT_EQ(arrayA.Size(), arrayB.Size());
    ASSERT_EQ(arrayA.Count(), arrayB.Count());
}

UTEST(test_MSArray, CopyEmptyArrayToFullArray)
{
    MSArray<uint32_t, 4> arrayA;

    MSArray<uint32_t, 4> arrayB = {1, 2, 3, 4};

    arrayB = arrayA;

    ASSERT_EQ(arrayA.Count(), arrayB.Count());
    ASSERT_EQ(arrayA.Size(), arrayB.Size());
}

UTEST(test_MSArray, MoveArray)
{
    MSArray<uint32_t, 2> arrayA;

    ASSERT_EQ(arrayA.Size(), 2);
    ASSERT_EQ(arrayA.Count(), 0);

    MSArray<uint32_t, 2> arrayB = {3, 4};

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
    MSArray<uint32_t, 4> array({1, 2, 3, 4});

    uint32_t expectedResults[] = {1, 2, 3, 4};

    for (size_t i = 0; i < array.Count(); i++)
    {
        ASSERT_EQ(array[i], expectedResults[i]);
    }
}

UTEST(test_MSArray, IterateUsingRangedForLoop)
{
    MSArray<uint32_t, 4> array = {1, 2, 3, 4};

    uint32_t expectedResults[] = {1, 2, 3, 4};

    const size_t ARR_COUNT = 4;
    uint32_t actualResults[ARR_COUNT];

    size_t index = 0;
    for (auto it = array.CreateIterator(); it; ++it)
    {
        actualResults[index++] = *it;
    }

    for (size_t i = 0; i < ARR_COUNT; i++)
    {
        ASSERT_EQ(expectedResults[i], actualResults[i]);
    }
}

UTEST(test_MSArray, IterateUsingRangedForLoopWithPartiallyFilledArray)
{
    MSArray<uint32_t, 4> array;

    array[0] = 1;
    array[1] = 2;
    array[3] = 4;

    uint32_t expectedResults[] = {1, 2, 4};

    const size_t ARR_COUNT = 3;
    uint32_t actualResults[ARR_COUNT];

    size_t index = 0;
    for (auto it = array.CreateIterator(); it; ++it)
    {
        actualResults[index++] = *it;
    }

    for (size_t i = 0; i < ARR_COUNT; i++)
    {
        ASSERT_EQ(expectedResults[i], actualResults[i]);
    }
}

UTEST(test_MSArray, IterateOverSingleElementArray)
{
    MSArray<uint32_t, 1> array = {1};

    uint32_t expectedResults[] = {1};

    const size_t ARR_COUNT = 1;
    uint32_t actualResults[ARR_COUNT];

    size_t index = 0;
    for (auto it = array.CreateIterator(); it; ++it)
    {
        actualResults[index++] = *it;
    }

    for (size_t i = 0; i < ARR_COUNT; i++)
    {
        ASSERT_EQ(expectedResults[i], actualResults[i]);
    }
}

UTEST(test_MSArray, IterateOverLargeArray)
{
    MSArray<uint32_t, 20> array = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                                   11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

    uint32_t expectedResults[] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                                  11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

    const size_t ARR_COUNT = 20;
    uint32_t actualResults[ARR_COUNT];

    size_t index = 0;
    for (auto it = array.CreateIterator(); it; ++it)
    {
        actualResults[index++] = *it;
    }

    for (size_t i = 0; i < ARR_COUNT; i++)
    {
        ASSERT_EQ(expectedResults[i], actualResults[i]);
    }
}

UTEST(test_MSArray, IterateOverEmptyArray)
{
    MSArray<uint32_t, 20> array;

    uint32_t expectedIndex {0};

    size_t index = 0;
    for (auto it = array.CreateIterator(); it; ++it)
    {
        index++;
    }

    ASSERT_EQ(expectedIndex, index);
}

UTEST(test_MSArray, IterateOverPartiallyFilledArray)
{
    MSArray<uint32_t, 20> array;
    int expected[] = {2, 1};

    array[1] = 2;
    array[3] = 1;

    const size_t ARR_COUNT = 2;
    uint32_t actualResults[ARR_COUNT];

    size_t index = 0;
    for (auto it = array.CreateIterator(); it; ++it)
    {
        actualResults[index++] = *it;
    }

    for (size_t i = 0; i < ARR_COUNT; i++)
    {
        ASSERT_EQ(expected[i], actualResults[i]);
    }
}

UTEST(test_MSArray, IterateOverArrayWithMissingFrontAndEnd)
{
    MSArray<uint32_t, 4> array;
    int expected[] = {2, 1};

    array[1] = 2;
    array[2] = 1;

    const size_t ARR_COUNT = 2;
    uint32_t actualResults[ARR_COUNT];

    size_t index = 0;
    for (auto it = array.CreateIterator(); it; ++it)
    {
        actualResults[index++] = *it;
    }

    for (size_t i = 0; i < ARR_COUNT; i++)
    {
        ASSERT_EQ(expected[i], actualResults[i]);
    }
}

UTEST(test_MSArray, IterateOverArrayWithLargeGaps)
{
    MSArray<uint32_t, 10> array;
    int expected[] = {2, 1, 3};

    array[1] = 2;
    array[5] = 1;
    array[8] = 3;

    const size_t ARR_COUNT = 3;
    uint32_t actualResults[ARR_COUNT];

    size_t index = 0;
    for (auto it = array.CreateIterator(); it; ++it)
    {
        actualResults[index++] = *it;
    }

    for (size_t i = 0; i < ARR_COUNT; i++)
    {
        ASSERT_EQ(expected[i], actualResults[i]);
    }
}

UTEST(test_MSArray, IterateOverSparseArray)
{
    MSArray<uint32_t, 20> array;
    int expected[] = {2, 1, 3};

    // 0000 0001 0000 0000 0010 0010
    array[1] = 2;
    array[5] = 1;
    array[16] = 3;

    const size_t ARR_COUNT = 3;
    uint32_t actualResults[ARR_COUNT];

    size_t index = 0;
    for (auto it = array.CreateIterator(); it; ++it)
    {
        actualResults[index++] = *it;
    }

    for (size_t i = 0; i < ARR_COUNT; i++)
    {
        ASSERT_EQ(expected[i], actualResults[i]);
    }
}

UTEST(test_MSArray, IterateWithFastIterator)
{
    MSArray<uint32_t, 3> array = {2, 1, 3};
    int expected[] = {2, 1, 3};

    const size_t ARR_COUNT = 3;
    uint32_t actualResults[ARR_COUNT];

    size_t index = 0;
    for (auto it = array.CreateFIterator(); it; ++it)
    {
        actualResults[index++] = *it;
    }

    for (size_t i = 0; i < ARR_COUNT; i++)
    {
        ASSERT_EQ(expected[i], actualResults[i]);
    }
}

UTEST(test_MSArray, IterateWithFastIteratorWithGaps)
{
    const size_t ARR_COUNT = 20;

    MSArray<uint32_t, ARR_COUNT> array;
    int expected[] = {0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0};

    array[1] = 2;
    array[5] = 1;
    array[16] = 3;

    uint32_t actualResults[ARR_COUNT];

    size_t index = 0;
    for (auto it = array.CreateFIterator(); it; ++it)
    {
        actualResults[index++] = *it;
    }

    for (size_t i = 0; i < ARR_COUNT; i++)
    {
        ASSERT_EQ(expected[i], actualResults[i]);
    }
}

UTEST(test_MSArray, IterateWithFunction)
{
    MSArray<uint32_t, 5> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    size_t i = 0;
    array.ForEach([&](uint32_t& val) {
        ASSERT_EQ(expectedResults[i], val);
        i++;
    });
}

UTEST(test_MSArray, IterateWithFunctionAndIndex)
{
    MSArray<uint32_t, 5> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    array.ForEachI([&](uint32_t& val, size_t i) { ASSERT_EQ(expectedResults[i], val); });
}

UTEST(test_MSArray, IterateWithConstFunction)
{
    MSArray<uint32_t, 5> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    size_t i = 0;
    array.ForEach([&](const uint32_t& val) {
        ASSERT_EQ(expectedResults[i], val);
        i++;
    });
}

UTEST(test_MSArray, IterateWithConstFunctionAndIndex)
{
    MSArray<uint32_t, 5> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    array.ForEachI([&](const uint32_t& val, size_t i) { ASSERT_EQ(expectedResults[i], val); });
}

UTEST(test_MSArray, IterateWithConstFunctionOnConstArray)
{
    const MSArray<uint32_t, 5> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    size_t i = 0;
    array.ForEach([&](const uint32_t& val) {
        ASSERT_EQ(expectedResults[i], val);
        i++;
    });
}

UTEST(test_MSArray, IterateWithConstFunctionAndIndexOnConstArray)
{
    const MSArray<uint32_t, 5> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    array.ForEachI([&](const uint32_t& val, size_t i) { ASSERT_EQ(expectedResults[i], val); });
}

UTEST(test_MSArray, IterateWithFunctionOverZeroSizedArray)
{
    MSArray<uint32_t, 0> array;

    size_t i = 0;
    array.ForEach([&](uint32_t& val) { i++; });

    ASSERT_EQ(i, 0);
}

UTEST(test_MSArray, IterateWithFunctionOverEmptyArray)
{
    MSArray<uint32_t, 5> array;

    size_t i = 0;
    array.ForEach([&](uint32_t& val) { i++; });

    ASSERT_EQ(i, 5);
}

UTEST(test_MSArray, IterateWithFunctionOverZeroSizedArrayWithIndex)
{
    MSArray<uint32_t, 0> array;

    size_t iterations = 0;
    array.ForEachI([&](uint32_t& val, size_t i) { iterations++; });

    ASSERT_EQ(iterations, 0);
}

UTEST(test_MSArray, IterateWithFunctionOverEmptyArrayWithIndex)
{
    MSArray<uint32_t, 5> array;

    size_t iterations = 0;
    array.ForEachI([&](uint32_t& val, size_t i) { iterations++; });

    ASSERT_EQ(iterations, 5);
}

UTEST(test_MSArray, IterateWithManagedFunction)
{
    MSArray<uint32_t, 5> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    size_t i = 0;
    array.MForEach([&](uint32_t& val) {
        ASSERT_EQ(expectedResults[i], val);
        i++;
    });
}

UTEST(test_MSArray, IterateWithManagedFunctionAndIndex)
{
    MSArray<uint32_t, 5> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    array.MForEachI([&](uint32_t& val, size_t i) { ASSERT_EQ(expectedResults[i], val); });
}

UTEST(test_MSArray, IterateWithManagedFunctionOnConstArray)
{
    const MSArray<uint32_t, 5> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    size_t i = 0;
    array.MForEach([&](const uint32_t& val) {
        ASSERT_EQ(expectedResults[i], val);
        i++;
    });
}

UTEST(test_MSArray, IterateWithManagedFunctionAndIndexOnConstArray)
{
    const MSArray<uint32_t, 5> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    array.MForEachI([&](const uint32_t& val, size_t i) { ASSERT_EQ(expectedResults[i], val); });
}

UTEST(test_MSArray, IterateWithManagedFunctionAndConstant)
{
    MSArray<uint32_t, 5> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    size_t i = 0;
    array.MForEach([&](const uint32_t& val) {
        ASSERT_EQ(expectedResults[i], val);
        i++;
    });
}

UTEST(test_MSArray, IterateWithManagedFunctionAndIndexAndConstant)
{
    MSArray<uint32_t, 5> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    array.MForEachI([&](const uint32_t& val, size_t i) { ASSERT_EQ(expectedResults[i], val); });
}

UTEST(test_MSArray, IterateWithManagedFunctionWithGaps)
{
    MSArray<uint32_t, 5> array;

    array[1] = 2;
    array[3] = 4;
    array[4] = 0;

    uint32_t expectedResults[] = {2, 4, 0};

    size_t i = 0;
    array.MForEach([&](uint32_t& val) {
        ASSERT_EQ(expectedResults[i], val);
        i++;
    });
}

UTEST(test_MSArray, IterateWithManagedFunctionAndIndexWithGaps)
{
    MSArray<uint32_t, 5> array;

    array[1] = 2;
    array[3] = 4;
    array[4] = 0;

    uint32_t expectedResults[] = {2, 4, 0};

    array.MForEachI([&](uint32_t& val, size_t i) { ASSERT_EQ(expectedResults[i], val); });
}

UTEST(test_MSArray, IterateOverEmptyArrayWithManagedFunction)
{
    MSArray<uint32_t, 5> array;

    size_t iterations = 0;
    array.MForEach([&](uint32_t& val) { iterations++; });

    ASSERT_EQ(0, iterations);
}

UTEST(test_MSArray, IterateOverEmptyArrayWithManagedFunctionAndIndex)
{
    MSArray<uint32_t, 5> array;

    size_t iterations = 0;
    array.MForEachI([&](uint32_t& val, size_t i) { iterations++; });

    ASSERT_EQ(0, iterations);
}