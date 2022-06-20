//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <render/renderer/utils/HeapArray.h>
#include <utest.h>
#include <utility>

UTEST(test_HeapArray, CreateEmptyHeapArray)
{
    Siege::Utils::HeapArray<uint32_t> array;

    ASSERT_TRUE(array.Size() == 0);
    ASSERT_TRUE(array.Data() == nullptr);
}

UTEST(test_HeapArray, CreateHeapArrayWithSize)
{
    Siege::Utils::HeapArray<uint32_t> array(2);

    ASSERT_TRUE(array.Size() == 2);
    ASSERT_FALSE(array.Data() == nullptr);
}

UTEST(test_HeapArray, CreateHeapArrayWithInitializerList)
{
    Siege::Utils::HeapArray<uint32_t> array = {0, 1};

    ASSERT_TRUE(array.Size() == 2);
    ASSERT_FALSE(array.Data() == nullptr);

    ASSERT_EQ(array[0], 0);
    ASSERT_EQ(array[1], 1);
}

UTEST(test_HeapArray, CreateHeapArrayFromCopyConstructor)
{
    Siege::Utils::HeapArray<uint32_t> arrayA = {0, 1};
    Siege::Utils::HeapArray<uint32_t> arrayB(arrayA);

    ASSERT_EQ(arrayA.Size(), arrayB.Size());
    ASSERT_EQ(arrayA.Count(), arrayB.Count());

    ASSERT_EQ(arrayA[0], arrayB[0]);
    ASSERT_EQ(arrayA[1], arrayB[1]);
}

UTEST(test_HeapArray, CopyEmptyArrayWithConstructor)
{
    Siege::Utils::HeapArray<uint32_t> arrayA;

    Siege::Utils::HeapArray<uint32_t> arrayB(arrayA);

    ASSERT_EQ(arrayA.Data(), arrayB.Data());
    ASSERT_EQ(arrayA.Count(), arrayB.Count());
    ASSERT_EQ(arrayA.Size(), arrayB.Size());
}

UTEST(test_HeapArray, CreateHeapArrayFromMoveConstructor)
{
    Siege::Utils::HeapArray<uint32_t> arrayA({1, 2, 3, 4});

    ASSERT_EQ(arrayA.Count(), 4);
    ASSERT_EQ(arrayA.Size(), 4);
    ASSERT_NE(arrayA.Data(), nullptr);

    ASSERT_EQ(arrayA[0], 1);
    ASSERT_EQ(arrayA[1], 2);
    ASSERT_EQ(arrayA[2], 3);
    ASSERT_EQ(arrayA[3], 4);

    Siege::Utils::HeapArray<uint32_t> arrayB(std::move(arrayA));

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
    Siege::Utils::HeapArray<uint32_t> emptyArray;
    Siege::Utils::HeapArray<uint32_t> filledArray(2);

    emptyArray.Destroy();
    filledArray.Destroy();

    ASSERT_TRUE(emptyArray.Data() == nullptr);
    ASSERT_TRUE(filledArray.Data() == nullptr);

    ASSERT_TRUE(emptyArray.Size() == 0);
    ASSERT_TRUE(filledArray.Size() == 0);
}

UTEST(test_HeapArray, CheckIndexInBounds)
{
    Siege::Utils::HeapArray<uint32_t> array(1);

    ASSERT_TRUE(array.IsInBounds(0));
    ASSERT_FALSE(array.IsInBounds(1));
}

UTEST(test_HeapArray, InsertElementUsingFunction)
{
    Siege::Utils::HeapArray<uint32_t> array(2);

    array.Insert(0, 1);

    ASSERT_TRUE(array.Count() == 1);

    array.Insert(1, 2);

    ASSERT_TRUE(array.Count() == 2);

    ASSERT_TRUE(array[0] == 1);
    ASSERT_TRUE(array[1] == 2);
}

UTEST(test_HeapArray, InsertElementUsingBrackets)
{
    Siege::Utils::HeapArray<uint32_t> array(2);

    array[0] = 1;

    ASSERT_TRUE(array.Count() == 1);

    array[1] = 2;

    ASSERT_TRUE(array.Count() == 2);

    ASSERT_TRUE(array[0] == 1);
    ASSERT_TRUE(array[1] == 2);
}

UTEST(test_HeapArray, GetElementUsingFunction)
{
    Siege::Utils::HeapArray<uint32_t> array(2);

    array[0] = 1;

    ASSERT_EQ(array.Get(0), 1);

    array[1] = 2;

    ASSERT_EQ(array.Get(1), 2);
}

UTEST(test_HeapArray, GetElementUsingBrackets)
{
    Siege::Utils::HeapArray<uint32_t> array(2);

    array[0] = 1;

    ASSERT_EQ(array[0], 1);

    array[1] = 2;

    ASSERT_EQ(array[1], 2);
}

UTEST(test_HeapArray, CheckElementIsActive)
{
    Siege::Utils::HeapArray<uint32_t> array(2);

    array[0] = 1;

    ASSERT_TRUE(array.Active(0));
    ASSERT_FALSE(array.Active(1));

    array[1] = 2;

    ASSERT_TRUE(array.Active(1));
}

UTEST(test_HeapArray, RemoveElementByIndex)
{
    Siege::Utils::HeapArray<uint32_t> array(2);

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
}

UTEST(test_HeapArray, CopyArray)
{
    Siege::Utils::HeapArray<uint32_t> arrayA = {0, 1};
    Siege::Utils::HeapArray<uint32_t> arrayB = {3, 4};

    arrayA = arrayB;

    ASSERT_EQ(arrayA[0], arrayB[0]);
    ASSERT_EQ(arrayA[1], arrayB[1]);

    ASSERT_EQ(arrayA.Size(), arrayB.Size());
    ASSERT_EQ(arrayA.Count(), arrayB.Count());
}

UTEST(test_HeapArray, CopyEmptyArray)
{
    Siege::Utils::HeapArray<uint32_t> arrayA;
    Siege::Utils::HeapArray<uint32_t> arrayB(arrayA);

    arrayA = arrayB;

    ASSERT_EQ(arrayA.Data(), arrayB.Data());
    ASSERT_EQ(arrayA.Size(), arrayB.Size());
    ASSERT_EQ(arrayA.Count(), arrayB.Count());
}

UTEST(test_HeapArray, CopyEmptyArrayToFullArray)
{
    Siege::Utils::HeapArray<uint32_t> arrayA;

    Siege::Utils::HeapArray<uint32_t> arrayB = {1, 2, 3, 4};
    arrayB = arrayA;

    ASSERT_EQ(arrayA.Data(), arrayB.Data());
    ASSERT_EQ(arrayA.Count(), arrayB.Count());
    ASSERT_EQ(arrayA.Size(), arrayB.Size());
}

UTEST(test_HeapArray, MoveArray)
{
    Siege::Utils::HeapArray<uint32_t> arrayA;

    ASSERT_EQ(arrayA.Size(), 0);
    ASSERT_EQ(arrayA.Count(), 0);

    ASSERT_EQ(arrayA.Data(), nullptr);

    Siege::Utils::HeapArray<uint32_t> arrayB({3, 4});

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
    Siege::Utils::HeapArray<uint32_t> array({1, 2, 3, 4});

    uint32_t expectedResults[] = {1, 2, 3, 4};

    for (size_t i = 0; i < array.Count(); i++)
    {
        ASSERT_EQ(array[i], expectedResults[i]);
    }
}

UTEST(test_HeapArray, IterateUsingRangedForLoop)
{
    Siege::Utils::HeapArray<uint32_t> array({1, 2, 3, 4});

    uint32_t expectedResults[] = {1, 2, 3, 4};

    size_t index = 0;
    for (auto& element : array)
    {
        ASSERT_EQ(element, expectedResults[index++]);
    }
}

UTEST(test_HeapArray, ResizeArray)
{
    Siege::Utils::HeapArray<uint32_t> array({1, 2, 3, 4});

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

    ASSERT_FALSE(array.Active(4));
    ASSERT_FALSE(array.Active(5));

    array[4] = 5;
    array[5] = 6;

    ASSERT_TRUE(array.Active(4));
    ASSERT_TRUE(array.Active(5));

    ASSERT_EQ(array[4], 5);
    ASSERT_EQ(array[5], 6);
    ASSERT_EQ(array.Count(), 6);
}

UTEST(test_HeapArray, ResizeArrayToSmallerSize)
{
    Siege::Utils::HeapArray<uint32_t> array({1, 2, 3, 4});

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

    ASSERT_TRUE(array.Active(0));
    ASSERT_TRUE(array.Active(1));
    ASSERT_FALSE(array.Active(2));
    ASSERT_FALSE(array.Active(3));
}

UTEST(test_HeapArray, ResizeLargeArray)
{
    Siege::Utils::HeapArray<uint32_t> array({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    ASSERT_EQ(array.Size(), 10);
    ASSERT_EQ(array.Count(), 10);

    uint32_t number = 1;

    for (auto& value : array) ASSERT_EQ(number++, value);

    array.Resize(9);

    ASSERT_EQ(array.Size(), 9);
    ASSERT_EQ(array.Count(), 9);

    ASSERT_FALSE(array.Active(9));
}

UTEST(test_HeapArray, ClearArray)
{
    Siege::Utils::HeapArray<uint32_t> array({1, 2});

    ASSERT_EQ(array.Size(), 2);
    ASSERT_EQ(array.Count(), 2);

    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);

    array.Clear();

    ASSERT_EQ(array.Size(), 2);
    ASSERT_EQ(array.Count(), 0);

    ASSERT_FALSE(array.Active(0));
    ASSERT_FALSE(array.Active(1));
}
