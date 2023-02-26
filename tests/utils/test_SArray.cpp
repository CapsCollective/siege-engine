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

using Siege::Utils::SArray;

UTEST(test_SArray, CreateEmptySArray)
{
    SArray<uint32_t, 1> array = {};

    ASSERT_TRUE(array.Size() == 1);
    ASSERT_TRUE(array.Data() != nullptr);
}

UTEST(test_SArray, CreateSArrayWithInitialiserList)
{
    SArray<uint32_t, 3> array = {1, 2, 3};

    ASSERT_TRUE(array.Size() == 3);
    ASSERT_TRUE(array.Data() != nullptr);

    ASSERT_EQ(1, array[0]);
    ASSERT_EQ(2, array[1]);
    ASSERT_EQ(3, array[2]);
}

UTEST(test_SArray, CreateConstexprSArray)
{
    constexpr SArray<uint32_t, 3> array = {1, 2, 3};

    ASSERT_TRUE(array.Size() == 3);
    ASSERT_TRUE(array.Data() != nullptr);

    ASSERT_EQ(1, array[0]);
    ASSERT_EQ(2, array[1]);
    ASSERT_EQ(3, array[2]);
}

UTEST(test_SArray, CreateSArrayWithCopyConstructor)
{
    SArray<uint32_t, 3> arrayA = {2, 3, 4};

    SArray<uint32_t, 3> arrayB(arrayA);

    ASSERT_TRUE(arrayB.Size() == 3);
    ASSERT_TRUE(arrayB.Data() != nullptr);

    ASSERT_EQ(arrayA[0], arrayB[0]);
    ASSERT_EQ(arrayA[1], arrayB[1]);
    ASSERT_EQ(arrayA[2], arrayB[2]);
}

UTEST(test_SArray, CreateSArrayFromMoveConstructor)
{
    SArray<uint32_t, 4> arrayA = {1, 2, 3, 4};

    ASSERT_EQ(arrayA.Size(), 4);
    ASSERT_NE(arrayA.Data(), nullptr);

    ASSERT_EQ(arrayA[0], 1);
    ASSERT_EQ(arrayA[1], 2);
    ASSERT_EQ(arrayA[2], 3);
    ASSERT_EQ(arrayA[3], 4);

    SArray<uint32_t, 4> arrayB(std::move(arrayA));

    ASSERT_EQ(arrayB.Size(), 4);
    ASSERT_NE(arrayB.Data(), nullptr);

    ASSERT_EQ(arrayB[0], 1);
    ASSERT_EQ(arrayB[1], 2);
    ASSERT_EQ(arrayB[2], 3);
    ASSERT_EQ(arrayB[3], 4);
}

UTEST(test_SArray, CheckIndexInBounds)
{
    SArray<uint32_t, 1> array;

    ASSERT_TRUE(array.IsInBounds(0));
    ASSERT_FALSE(array.IsInBounds(1));
}

UTEST(test_SArray, InsertElementUsingFunction)
{
    SArray<uint32_t, 2> array;

    ASSERT_EQ(array.Size(), 2);

    array.Insert(0, 1);
    array.Insert(1, 2);

    ASSERT_EQ(1, array[0]);
    ASSERT_EQ(2, array[1]);
}

UTEST(test_SArray, InsertElementUsingBrackets)
{
    SArray<uint32_t, 2> array;

    ASSERT_EQ(array.Size(), 2);

    array[0] = 1;
    array[1] = 2;

    ASSERT_EQ(1, array[0]);
    ASSERT_EQ(2, array[1]);
}

UTEST(test_SArray, GetElementUsingFunction)
{
    SArray<uint32_t, 2> array;

    array[0] = 1;

    ASSERT_EQ(1, array.Get(0));

    array[1] = 2;

    ASSERT_EQ(2, array.Get(1));
}

UTEST(test_SArray, GetElementUsingBracket)
{
    SArray<uint32_t, 2> array;

    array[0] = 1;

    ASSERT_EQ(1, array[0]);

    array[1] = 2;

    ASSERT_EQ(2, array[1]);
}

UTEST(test_SArray, CopySArray)
{
    SArray<uint32_t, 2> arrayA = {0, 1};
    SArray<uint32_t, 2> arrayB = {3, 4};

    arrayA = arrayB;

    ASSERT_EQ(arrayA[0], arrayB[0]);
    ASSERT_EQ(arrayA[1], arrayB[1]);

    ASSERT_EQ(arrayA.Size(), arrayB.Size());
}

UTEST(test_SArray, CopyEmptySArray)
{
    SArray<uint32_t, 0> arrayA {};
    SArray<uint32_t, 0> arrayB(arrayA);

    arrayA = arrayB;

    ASSERT_EQ(arrayA.Size(), arrayB.Size());
}

UTEST(test_SArray, MoveSArray)
{
    SArray<uint32_t, 2> arrayA = {1, 2};

    SArray<uint32_t, 2> arrayB = {3, 4};

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
    SArray<uint32_t, 4> array({1, 2, 3, 4});

    uint32_t expectedResults[] = {1, 2, 3, 4};

    for (size_t i = 0; i < array.Size(); i++)
    {
        ASSERT_EQ(array[i], expectedResults[i]);
    }
}

UTEST(test_SArray, IterateUsingIterator)
{
    SArray<uint32_t, 4> array({1, 2, 3, 4});

    uint32_t expectedResults[] = {1, 2, 3, 4};

    size_t index = 0;
    for (auto it = array.CreateIterator(); it; ++it)
    {
        ASSERT_EQ(expectedResults[index++], *it);
    }
}

UTEST(test_SArray, IterateOverSingleElementSArray)
{
    SArray<uint32_t, 1> array = {1};

    uint32_t expectedResults[] = {1};

    size_t index = 0;
    for (auto it = array.CreateIterator(); it; ++it)
    {
        ASSERT_EQ(expectedResults[index++], *it);
    }
}

UTEST(test_SArray, IterateWithFunction)
{
    SArray<uint32_t, 5> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    size_t i = 0;
    array.ForEach([&](uint32_t& val) {
        ASSERT_EQ(expectedResults[i], val);
        i++;
    });
}

UTEST(test_SArray, IterateWithFunctionAndIndex)
{
    SArray<uint32_t, 5> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    array.ForEachI([&](uint32_t& val, size_t i) { ASSERT_EQ(expectedResults[i], val); });
}

UTEST(test_SArray, IterateWithConstFunction)
{
    SArray<uint32_t, 5> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    size_t i = 0;
    array.ForEach([&](const uint32_t& val) {
        ASSERT_EQ(expectedResults[i], val);
        i++;
    });
}

UTEST(test_SArray, IterateWithConstFunctionAndIndex)
{
    SArray<uint32_t, 5> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    array.ForEachI([&](const uint32_t& val, size_t i) { ASSERT_EQ(expectedResults[i], val); });
}

UTEST(test_SArray, IterateWithConstFunctionOnConstArray)
{
    const SArray<uint32_t, 5> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    size_t i = 0;
    array.ForEach([&](const uint32_t& val) {
        ASSERT_EQ(expectedResults[i], val);
        i++;
    });
}

UTEST(test_SArray, IterateWithConstFunctionAndIndexOnConstArray)
{
    const SArray<uint32_t, 5> array = {1, 2, 3, 4, 5};

    uint32_t expectedResults[] = {1, 2, 3, 4, 5};

    array.ForEachI([&](const uint32_t& val, size_t i) { ASSERT_EQ(expectedResults[i], val); });
}

UTEST(test_SArray, IterateWithFunctionOverZeroSizedArray)
{
    SArray<uint32_t, 0> array;

    size_t i = 0;
    array.ForEach([&](uint32_t& val) { i++; });

    ASSERT_EQ(i, 0);
}

UTEST(test_SArray, IterateWithFunctionOverEmptyArray)
{
    SArray<uint32_t, 5> array;

    size_t i = 0;
    array.ForEach([&](uint32_t& val) { i++; });

    ASSERT_EQ(i, 5);
}

UTEST(test_SArray, IterateWithFunctionOverZeroSizedArrayWithIndex)
{
    SArray<uint32_t, 0> array;

    size_t iterations = 0;
    array.ForEachI([&](uint32_t& val, size_t i) { iterations++; });

    ASSERT_EQ(iterations, 0);
}

UTEST(test_SArray, IterateWithFunctionOverEmptyArrayWithIndex)
{
    SArray<uint32_t, 5> array;

    size_t iterations = 0;
    array.ForEachI([&](uint32_t& val, size_t i) { iterations++; });

    ASSERT_EQ(iterations, 5);
}