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

UTEST(test_SArray, CreateEmptySMSArray)
{
    Siege::Utils::SArray<uint32_t, 1> array;

    ASSERT_TRUE(array.Size() == 1);
    ASSERT_TRUE(array.Data() != nullptr);
}

UTEST(test_SArray, CreateSMSArrayWithInitialiserList)
{
    Siege::Utils::SArray<uint32_t, 3> array = { 1, 2, 3 };

    ASSERT_TRUE(array.Size() == 3);
    ASSERT_TRUE(array.Data() != nullptr);

    ASSERT_EQ(1, array[0]);
    ASSERT_EQ(2, array[1]);
    ASSERT_EQ(3, array[2]);
}

UTEST(test_SArray, CreateConstexprSMSArray)
{
    constexpr Siege::Utils::SArray<uint32_t, 3> array = {1, 2, 3};

    ASSERT_TRUE(array.Size() == 3);
    ASSERT_TRUE(array.Data() != nullptr);

    ASSERT_EQ(1, array[0]);
    ASSERT_EQ(2, array[1]);
    ASSERT_EQ(3, array[2]);
}

UTEST(test_SArray, CreateMSSArrayWithCopyConstructor)
{
    Siege::Utils::SArray<uint32_t, 3> arrayA = {2, 3, 4};

    Siege::Utils::SArray<uint32_t, 3> arrayB(arrayA);

    ASSERT_TRUE(arrayB.Size() == 3);
    ASSERT_TRUE(arrayB.Data() != nullptr);

    ASSERT_EQ(arrayA[0], arrayB[0]);
    ASSERT_EQ(arrayA[1], arrayB[1]);
    ASSERT_EQ(arrayA[2], arrayB[2]);
}

UTEST(test_SArray, CreateSSArrayFromMoveConstructor)
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
    Siege::Utils::SArray<uint32_t, 0> arrayA;
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
    for(auto& element : array)
    {
        ASSERT_EQ(expectedResults[index++], element);
    }
}

UTEST(test_SArray, IterateOverSingleElementSArray)
{
    Siege::Utils::SArray<uint32_t ,1> array = {1};

    uint32_t expectedResults[] = {1};

    size_t index = 0;
    for (auto& element : array)
    {
        ASSERT_EQ(expectedResults[index++], element);
    }
}