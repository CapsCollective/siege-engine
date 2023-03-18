//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include <utest.h>
#include <utils/collections/HeapArray.h>

#include <utility>

using Siege::MHArray;

UTEST(test_HeapArray, CreateEmptyHeapArray)
{
    MHArray<uint32_t> array;

    ASSERT_TRUE(array.Size() == 0);
    ASSERT_TRUE(array.Data() == nullptr);
}

UTEST(test_HeapArray, CreateHeapArrayWithSize)
{
    MHArray<uint32_t> array(2);

    ASSERT_TRUE(array.Size() == 2);
    ASSERT_FALSE(array.Data() == nullptr);
}

UTEST(test_HeapArray, CreateHeapArrayWithInitializerList)
{
    MHArray<uint32_t> array = {0, 1};

    ASSERT_TRUE(array.Size() == 2);
    ASSERT_FALSE(array.Data() == nullptr);

    ASSERT_EQ(array[0], 0);
    ASSERT_EQ(array[1], 1);
}

UTEST(test_HeapArray, CreateHeapArrayFromRawArray)
{
    uint32_t values[] = {1, 2, 3, 4};
    Siege::MHArray<uint32_t> array(values, 4);

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

UTEST(test_HeapArray, CreateHeapArrayWithDefaultValue)
{
    Siege::MHArray<uint32_t> array(5, 5);

    ASSERT_TRUE(array.IsActive(0));
    ASSERT_TRUE(array.IsActive(1));
    ASSERT_TRUE(array.IsActive(2));
    ASSERT_TRUE(array.IsActive(3));
    ASSERT_TRUE(array.IsActive(4));
    ASSERT_EQ(array.Count(), 5);
}

UTEST(test_HeapArray, CreateHeapArrayFromCopyConstructor)
{
    MHArray<uint32_t> arrayA = {0, 1};
    MHArray<uint32_t> arrayB(arrayA);

    ASSERT_EQ(arrayA.Size(), arrayB.Size());
    ASSERT_EQ(arrayA.Count(), arrayB.Count());

    ASSERT_EQ(arrayA[0], arrayB[0]);
    ASSERT_EQ(arrayA[1], arrayB[1]);
}

UTEST(test_HeapArray, CopyEmptyArrayWithConstructor)
{
    MHArray<uint32_t> arrayA;

    MHArray<uint32_t> arrayB(arrayA);

    ASSERT_EQ(arrayA.Data(), arrayB.Data());
    ASSERT_EQ(arrayA.Count(), arrayB.Count());
    ASSERT_EQ(arrayA.Size(), arrayB.Size());
}

UTEST(test_HeapArray, CreateHeapArrayFromMoveConstructor)
{
    MHArray<uint32_t> arrayA({1, 2, 3, 4});

    ASSERT_EQ(arrayA.Count(), 4);
    ASSERT_EQ(arrayA.Size(), 4);
    ASSERT_NE(arrayA.Data(), nullptr);

    ASSERT_EQ(arrayA[0], 1);
    ASSERT_EQ(arrayA[1], 2);
    ASSERT_EQ(arrayA[2], 3);
    ASSERT_EQ(arrayA[3], 4);

    MHArray<uint32_t> arrayB(std::move(arrayA));

    ASSERT_EQ(arrayB.Count(), 4);
    ASSERT_EQ(arrayB.Size(), 4);
    ASSERT_NE(arrayB.Data(), nullptr);

    ASSERT_EQ(arrayB[0], 1);
    ASSERT_EQ(arrayB[1], 2);
    ASSERT_EQ(arrayB[2], 3);
    ASSERT_EQ(arrayB[3], 4);
}

UTEST(test_HeapArray, DestroyHeapArray)
{
    MHArray<uint32_t> emptyArray;
    MHArray<uint32_t> filledArray(2);

    emptyArray.Destroy();
    filledArray.Destroy();

    ASSERT_TRUE(emptyArray.Data() == nullptr);
    ASSERT_TRUE(filledArray.Data() == nullptr);

    ASSERT_TRUE(emptyArray.Size() == 0);
    ASSERT_TRUE(filledArray.Size() == 0);
}

UTEST(test_HeapArray, InsertElementUsingFunction)
{
    MHArray<uint32_t> array(2);

    array.Insert(0, 1);

    ASSERT_TRUE(array.Count() == 1);

    array.Insert(1, 2);

    ASSERT_TRUE(array.Count() == 2);

    ASSERT_TRUE(array[0] == 1);
    ASSERT_TRUE(array[1] == 2);
}

UTEST(test_HeapArray, InsertElementUsingBrackets)
{
    MHArray<uint32_t> array(2);

    array[0] = 1;

    ASSERT_TRUE(array.Count() == 1);

    array[1] = 2;

    ASSERT_TRUE(array.Count() == 2);

    ASSERT_TRUE(array[0] == 1);
    ASSERT_TRUE(array[1] == 2);
}

UTEST(test_HeapArray, GetElementUsingFunction)
{
    MHArray<uint32_t> array(2);

    array[0] = 1;

    ASSERT_EQ(array.Get(0), 1);

    array[1] = 2;

    ASSERT_EQ(array.Get(1), 2);
}

UTEST(test_HeapArray, GetElementUsingBrackets)
{
    MHArray<uint32_t> array(2);

    array[0] = 1;

    ASSERT_EQ(array[0], 1);

    array[1] = 2;

    ASSERT_EQ(array[1], 2);
}

UTEST(test_HeapArray, CheckElementIsActive)
{
    MHArray<uint32_t> array(2);

    array[0] = 1;

    ASSERT_TRUE(array.IsActive(0));
    ASSERT_FALSE(array.IsActive(1));

    array[1] = 2;

    ASSERT_TRUE(array.IsActive(1));
}

UTEST(test_HeapArray, RemoveElementByIndex)
{
    MHArray<uint32_t> array(2);

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
}

UTEST(test_HeapArray, CopyArray)
{
    MHArray<uint32_t> arrayA = {0, 1};
    MHArray<uint32_t> arrayB = {3, 4};

    arrayA = arrayB;

    ASSERT_EQ(arrayA[0], arrayB[0]);
    ASSERT_EQ(arrayA[1], arrayB[1]);

    ASSERT_EQ(arrayA.Size(), arrayB.Size());
    ASSERT_EQ(arrayA.Count(), arrayB.Count());
}

UTEST(test_HeapArray, CopyEmptyArray)
{
    MHArray<uint32_t> arrayA;
    MHArray<uint32_t> arrayB(arrayA);

    arrayA = arrayB;

    ASSERT_EQ(arrayA.Data(), arrayB.Data());
    ASSERT_EQ(arrayA.Size(), arrayB.Size());
    ASSERT_EQ(arrayA.Count(), arrayB.Count());
}

UTEST(test_HeapArray, CopyEmptyArrayToFullArray)
{
    MHArray<uint32_t> arrayA;

    MHArray<uint32_t> arrayB = {1, 2, 3, 4};
    arrayB = arrayA;

    ASSERT_EQ(arrayA.Data(), arrayB.Data());
    ASSERT_EQ(arrayA.Count(), arrayB.Count());
    ASSERT_EQ(arrayA.Size(), arrayB.Size());
}

UTEST(test_HeapArray, MoveArray)
{
    MHArray<uint32_t> arrayA;

    ASSERT_EQ(arrayA.Size(), 0);
    ASSERT_EQ(arrayA.Count(), 0);

    ASSERT_EQ(arrayA.Data(), nullptr);

    MHArray<uint32_t> arrayB({3, 4});

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

UTEST(test_HeapArray, IterateUsingBrackets)
{
    MHArray<uint32_t> array({1, 2, 3, 4});

    uint32_t expectedResults[] = {1, 2, 3, 4};

    for (size_t i = 0; i < array.Count(); i++)
    {
        ASSERT_EQ(array[i], expectedResults[i]);
    }
}

UTEST(test_HeapArray, IterateUsingRangedForLoop)
{
    MHArray<uint32_t> array({1, 2, 3, 4});

    uint32_t expectedResults[] = {1, 2, 3, 4};

    size_t index = 0;
    for (auto it = array.CreateIterator(); it; ++it)
    {
        ASSERT_EQ(expectedResults[index++], *it);
    }
}

UTEST(test_HeapArray, IterateUsingRangedForLoopWithPartiallyFilledArray)
{
    MHArray<uint32_t> array(5);

    array[0] = 1;
    array[1] = 2;
    array[3] = 4;
    array[4] = 5;

    uint32_t expectedResults[] = {1, 2, 4, 5};

    size_t index = 0;
    for (auto it = array.CreateIterator(); it; ++it)
    {
        ASSERT_EQ(expectedResults[index++], *it);
    }
}

UTEST(test_HeapArray, IterateOverSingleElementArray)
{
    MHArray<uint32_t> array({1});

    uint32_t expectedResults[] = {1};

    size_t index = 0;
    for (auto it = array.CreateIterator(); it; ++it)
    {
        ASSERT_EQ(expectedResults[index++], *it);
    }
}

UTEST(test_HeapArray, IterateOverEmptyArray)
{
    MHArray<uint32_t> array(20);

    uint32_t expectedIndex {0};

    size_t index = 0;
    for (auto it = array.CreateIterator(); it; ++it)
    {
        index++;
    }

    ASSERT_EQ(expectedIndex, index);
}

UTEST(test_MHArray, IterateOverPartiallyFilledArray)
{
    MHArray<uint32_t> array(4);
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

UTEST(test_MHArray, IterateOverArrayWithMissingFrontAndEnd)
{
    MHArray<uint32_t> array(4);
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

UTEST(test_MHArray, IterateOverArrayWithLargeGaps)
{
    MHArray<uint32_t> array(10);
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

UTEST(test_MHArray, IterateOverSparseArray)
{
    MHArray<uint32_t> array(20);
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

UTEST(test_MHArray, IterateWithFastIterator)
{
    MHArray<uint32_t> array = {2, 1, 3};
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

UTEST(test_MHArray, IterateWithFastIteratorWithGaps)
{
    const size_t ARR_COUNT = 20;

    MHArray<uint32_t> array(ARR_COUNT);
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

UTEST(test_MHArray, ResizeArray)
{
    MHArray<uint32_t> array({1, 2, 3, 4});

    ASSERT_EQ(array.Size(), 4);
    ASSERT_EQ(array.Count(), 4);

    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 3);
    ASSERT_EQ(array[3], 4);

    array.Resize(6);

    ASSERT_EQ(array.Size(), 6);
    ASSERT_EQ(array.Count(), 4);

    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 3);
    ASSERT_EQ(array[3], 4);

    ASSERT_FALSE(array.IsActive(4));
    ASSERT_FALSE(array.IsActive(5));

    array[4] = 5;
    array[5] = 6;

    ASSERT_TRUE(array.IsActive(4));
    ASSERT_TRUE(array.IsActive(5));

    ASSERT_EQ(array[4], 5);
    ASSERT_EQ(array[5], 6);
    ASSERT_EQ(array.Count(), 6);
}

UTEST(test_MHArray, ResizeArrayToSmallerSize)
{
    MHArray<uint32_t> array({1, 2, 3, 4});

    ASSERT_EQ(array.Size(), 4);
    ASSERT_EQ(array.Count(), 4);

    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 3);
    ASSERT_EQ(array[3], 4);

    array.Resize(2);

    ASSERT_EQ(array.Size(), 2);
    ASSERT_EQ(array.Count(), 2);

    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);

    ASSERT_TRUE(array.IsActive(0));
    ASSERT_TRUE(array.IsActive(1));
    ASSERT_FALSE(array.IsActive(2));
    ASSERT_FALSE(array.IsActive(3));
}

UTEST(test_MHArray, ResizeLargeArray)
{
    MHArray<uint32_t> array({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    ASSERT_EQ(array.Size(), 10);
    ASSERT_EQ(array.Count(), 10);

    uint32_t number = 1;

    for (auto it = array.CreateIterator(); it; ++it) ASSERT_EQ(number++, *it);

    array.Resize(9);

    ASSERT_EQ(array.Size(), 9);
    ASSERT_EQ(array.Count(), 9);

    ASSERT_FALSE(array.IsActive(9));
}

UTEST(test_MHArray, ClearArray)
{
    MHArray<uint32_t> array({1, 2});

    ASSERT_EQ(array.Size(), 2);
    ASSERT_EQ(array.Count(), 2);

    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);

    array.Clear();

    ASSERT_EQ(array.Size(), 2);
    ASSERT_EQ(array.Count(), 0);

    ASSERT_FALSE(array.IsActive(0));
    ASSERT_FALSE(array.IsActive(1));
}

UTEST(test_MHArray, ClearEmptyArray)
{
    MHArray<uint32_t> array;

    ASSERT_EQ(array.Count(), 0);
    ASSERT_EQ(array.Data(), nullptr);

    array.Clear();

    ASSERT_EQ(array.Count(), 0);
    ASSERT_EQ(array.Data(), nullptr);
}

UTEST(test_MHArray, IterateWithFunction)
{
    MHArray<uint32_t> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    size_t i = 0;
    array.ForEach([&](uint32_t& val) {
        ASSERT_EQ(expectedResults[i], val);
        i++;
    });
}

UTEST(test_MHArray, IterateWithFunctionAndIndex)
{
    MHArray<uint32_t> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    array.ForEachI([&](uint32_t& val, size_t i) { ASSERT_EQ(expectedResults[i], val); });
}

UTEST(test_MHArray, IterateWithConstFunction)
{
    MHArray<uint32_t> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    size_t i = 0;
    array.ForEach([&](const uint32_t& val) {
        ASSERT_EQ(expectedResults[i], val);
        i++;
    });
}

UTEST(test_MHArray, IterateWithConstFunctionOnConstArray)
{
    const MHArray<uint32_t> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    size_t i = 0;
    array.ForEach([&](const uint32_t& val) {
        ASSERT_EQ(expectedResults[i], val);
        i++;
    });
}

UTEST(test_MHArray, IterateWithConstFunctionAndIndex)
{
    MHArray<uint32_t> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    array.ForEachI([&](const uint32_t& val, size_t i) { ASSERT_EQ(expectedResults[i], val); });
}

UTEST(test_MHArray, IterateWithConstFunctionAndIndexOnConstArray)
{
    const MHArray<uint32_t> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    array.ForEachI([&](const uint32_t& val, size_t i) { ASSERT_EQ(expectedResults[i], val); });
}

UTEST(test_MHArray, IterateWithFunctionOverZeroSizedArray)
{
    MHArray<uint32_t> array;

    size_t i = 0;
    array.ForEach([&](uint32_t& val) { i++; });

    ASSERT_EQ(i, 0);
}

UTEST(test_MHArray, IterateWithFunctionOverEmptyArray)
{
    MHArray<uint32_t> array(5);

    size_t i = 0;
    array.ForEach([&](uint32_t& val) { i++; });

    ASSERT_EQ(i, 5);
}

UTEST(test_MHArray, IterateWithFunctionOverZeroSizedArrayWithIndex)
{
    MHArray<uint32_t> array;

    size_t iterations = 0;
    array.ForEachI([&](uint32_t& val, size_t i) { iterations++; });

    ASSERT_EQ(iterations, 0);
}

UTEST(test_MHArray, IterateWithFunctionOverEmptyArrayWithIndex)
{
    MHArray<uint32_t> array(5);

    size_t iterations = 0;
    array.ForEachI([&](uint32_t& val, size_t i) { iterations++; });

    ASSERT_EQ(iterations, 5);
}

UTEST(test_MHArray, IterateWithManagedFunction)
{
    MHArray<uint32_t> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    size_t i = 0;
    array.MForEach([&](uint32_t& val) {
        ASSERT_EQ(expectedResults[i], val);
        i++;
    });
}

UTEST(test_MHArray, IterateWithManagedFunctionAndIndex)
{
    MHArray<uint32_t> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    array.MForEachI([&](uint32_t& val, size_t i) { ASSERT_EQ(expectedResults[i], val); });
}

UTEST(test_MHArray, IterateWithManagedFunctionOnConstArray)
{
    const MHArray<uint32_t> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    size_t i = 0;
    array.MForEach([&](const uint32_t& val) {
        ASSERT_EQ(expectedResults[i], val);
        i++;
    });
}

UTEST(test_MHArray, IterateWithManagedFunctionAndIndexOnConstArray)
{
    const MHArray<uint32_t> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    array.MForEachI([&](const uint32_t& val, size_t i) { ASSERT_EQ(expectedResults[i], val); });
}

UTEST(test_MHArray, IterateWithManagedFunctionAndConstant)
{
    MHArray<uint32_t> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    size_t i = 0;
    array.MForEach([&](const uint32_t& val) {
        ASSERT_EQ(expectedResults[i], val);
        i++;
    });
}

UTEST(test_MHArray, IterateWithManagedFunctionAndIndexAndConstant)
{
    MHArray<uint32_t> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    array.MForEachI([&](const uint32_t& val, size_t i) { ASSERT_EQ(expectedResults[i], val); });
}

UTEST(test_MHArray, IterateWithManagedFunctionWithGaps)
{
    MHArray<uint32_t> array(5);

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

UTEST(test_MHArray, IterateWithManagedFunctionAndIndexWithGaps)
{
    MHArray<uint32_t> array(5);

    array[1] = 2;
    array[3] = 4;
    array[4] = 0;

    uint32_t expectedResults[] = {2, 4, 0};

    array.MForEachI([&](uint32_t& val, size_t i) { ASSERT_EQ(expectedResults[i], val); });
}

UTEST(test_MHArray, IterateOverEmptyArrayWithManagedFunction)
{
    MHArray<uint32_t> array;

    size_t iterations = 0;
    array.MForEach([&](uint32_t& val) { iterations++; });

    ASSERT_EQ(0, iterations);
}

UTEST(test_MHArray, IterateOverEmptyArrayWithManagedFunctionAndIndex)
{
    MHArray<uint32_t> array;

    size_t iterations = 0;
    array.MForEachI([&](uint32_t& val, size_t i) { iterations++; });

    ASSERT_EQ(0, iterations);
}

UTEST(test_MHArray, GetItemAtBackOfMHArray)
{
    MHArray<uint32_t> arrayA {0, 1, 2, 3, 4};
    const MHArray<uint32_t> arrayB {0, 1, 2, 3, 4};

    ASSERT_EQ(4, arrayA.Back());

    ASSERT_EQ(4, arrayB.Back());
}
