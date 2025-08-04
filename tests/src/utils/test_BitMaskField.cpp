//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <utest.h>

#include <utility>

#include "utils/collections/BitSet.h"

UTEST(test_BitSet, CreateBitSet)
{
    Siege::BitUtils::BitSet bitMask;

    ASSERT_EQ(0, bitMask.LeftMostBit());
}

UTEST(test_BitSet, CreateBitMaskFieldWithSize)
{
    Siege::BitUtils::BitSet bitMask(3);

    ASSERT_EQ(0, bitMask.LeftMostBit());
}

UTEST(test_BitSet, CreateBitMaskWithCopyConstructor)
{
    Siege::BitUtils::BitSet bitMaskA(3);

    bitMaskA.SetBit(1);
    bitMaskA.SetBit(10);

    Siege::BitUtils::BitSet bitMaskB(bitMaskA);

    ASSERT_EQ(10, bitMaskB.LeftMostBit());
    ASSERT_TRUE(bitMaskB.IsSet(1));
    ASSERT_TRUE(bitMaskB.IsSet(10));
}

UTEST(test_BitSet, CreateBitMaskFieldWithMoveConstructor)
{
    Siege::BitUtils::BitSet bitMaskA(3);

    bitMaskA.SetBit(1);
    bitMaskA.SetBit(10);

    Siege::BitUtils::BitSet bitMaskB(std::move(bitMaskA));

    ASSERT_EQ(10, bitMaskB.LeftMostBit());
    ASSERT_TRUE(bitMaskB.IsSet(1));
    ASSERT_TRUE(bitMaskB.IsSet(10));
}

UTEST(test_BitSet, AddValueToFirstBit)
{
    Siege::BitUtils::BitSet bitMask(1);

    ASSERT_EQ(0, bitMask.LeftMostBit());

    bitMask.SetBit(1);

    ASSERT_TRUE(bitMask.IsSet(1));
    ASSERT_EQ(1, bitMask.LeftMostBit());
}

UTEST(test_BitSet, AddValueToLastBit)
{
    Siege::BitUtils::BitSet bitMask(1);

    ASSERT_EQ(0, bitMask.LeftMostBit());

    bitMask.SetBit(8);

    ASSERT_TRUE(bitMask.IsSet(8));
    ASSERT_EQ(8, bitMask.LeftMostBit());
}

UTEST(test_BitSet, AddValueToBitMask)
{
    Siege::BitUtils::BitSet bitMask(1);

    ASSERT_EQ(0, bitMask.LeftMostBit());

    bitMask.SetBit(2);

    ASSERT_TRUE(bitMask.IsSet(2));
    ASSERT_EQ(2, bitMask.LeftMostBit());
}

UTEST(test_BitSet, GetLeftMostBit)
{
    Siege::BitUtils::BitSet bitMask(1);

    ASSERT_EQ(0, bitMask.LeftMostBit());

    bitMask.SetBit(2);

    ASSERT_TRUE(bitMask.IsSet(2));
    ASSERT_EQ(2, bitMask.LeftMostBit());

    bitMask.SetBit(1);

    ASSERT_TRUE(bitMask.IsSet(1));
    ASSERT_EQ(2, bitMask.LeftMostBit());

    bitMask.SetBit(4);
    ASSERT_TRUE(bitMask.IsSet(4));
    ASSERT_EQ(4, bitMask.LeftMostBit());
}

UTEST(test_BitSet, RemoveBit)
{
    Siege::BitUtils::BitSet bitMask(1);

    ASSERT_EQ(0, bitMask.LeftMostBit());

    // 0001 0100
    bitMask.SetBit(2);
    bitMask.SetBit(5);

    ASSERT_TRUE(bitMask.IsSet(2));
    ASSERT_TRUE(bitMask.IsSet(5));
    ASSERT_EQ(5, bitMask.LeftMostBit());

    bitMask.UnsetBit(5);
    ASSERT_EQ(2, bitMask.LeftMostBit());
}

UTEST(test_BitSet, RemoveBitFromAnotherByte)
{
    Siege::BitUtils::BitSet bitMask(2);

    ASSERT_EQ(0, bitMask.LeftMostBit());

    // 0000 0001 0001 0010
    bitMask.SetBit(2);
    bitMask.SetBit(5);
    bitMask.SetBit(9);

    ASSERT_TRUE(bitMask.IsSet(2));
    ASSERT_TRUE(bitMask.IsSet(5));
    ASSERT_TRUE(bitMask.IsSet(9));

    ASSERT_EQ(9, bitMask.LeftMostBit());

    bitMask.UnsetBit(5); // 0000 0001 0000 0010
    ASSERT_EQ(9, bitMask.LeftMostBit());

    bitMask.UnsetBit(9);
    ASSERT_EQ(2, bitMask.LeftMostBit());
}

UTEST(test_BitSet, RemoveBitFromALargeMask)
{
    // 0000 0000 | 0100 0000 | 0000 1000 | 0100 0000 | 0001 0000
    Siege::BitUtils::BitSet bitMask(5);

    ASSERT_EQ(0, bitMask.LeftMostBit());

    // 0000 0001 0001 0010
    bitMask.SetBit(5);
    bitMask.SetBit(15);
    bitMask.SetBit(20);
    bitMask.SetBit(31);

    ASSERT_TRUE(bitMask.IsSet(5));
    ASSERT_TRUE(bitMask.IsSet(15));
    ASSERT_TRUE(bitMask.IsSet(20));
    ASSERT_TRUE(bitMask.IsSet(31));

    ASSERT_EQ(31, bitMask.LeftMostBit());

    bitMask.UnsetBit(5); // 0000 0001 0000 0010
    ASSERT_EQ(31, bitMask.LeftMostBit());

    bitMask.UnsetBit(31);
    ASSERT_EQ(20, bitMask.LeftMostBit());

    bitMask.UnsetBit(20);
    ASSERT_EQ(15, bitMask.LeftMostBit());

    bitMask.UnsetBit(15);
    ASSERT_EQ(0, bitMask.LeftMostBit());
}

UTEST(test_BitSet, SetBitsToOne)
{
    Siege::BitUtils::BitSet bitMask(1);

    bitMask.SetBitsToOne(2);

    ASSERT_EQ(2, bitMask.LeftMostBit());
    ASSERT_EQ(3, bitMask[0]);
}

UTEST(test_BitSet, SetLargeBitSetToOne)
{
    Siege::BitUtils::BitSet bitMask(3);

    // 0000 1111 1111 1111 1111 1111
    bitMask.SetBitsToOne(20);

    ASSERT_EQ(20, bitMask.LeftMostBit());
    ASSERT_EQ(255, bitMask[0]);
    ASSERT_EQ(255, bitMask[1]);
    ASSERT_EQ(15, bitMask[2]);
}

UTEST(test_BitSet, CopyBitField)
{
    Siege::BitUtils::BitSet bitMaskA(3);
    Siege::BitUtils::BitSet bitMaskB(3);

    // BitMask: 0000 0001 0010 0000 0000 1010
    bitMaskA.SetBit(2);
    bitMaskA.SetBit(4);
    bitMaskA.SetBit(14);
    bitMaskA.SetBit(17);

    ASSERT_EQ(17, bitMaskA.LeftMostBit());
    ASSERT_EQ(10, bitMaskA[0]);
    ASSERT_EQ(32, bitMaskA[1]);
    ASSERT_EQ(1, bitMaskA[2]);

    bitMaskB = bitMaskA;

    ASSERT_EQ(17, bitMaskB.LeftMostBit());
    ASSERT_EQ(10, bitMaskB[0]);
    ASSERT_EQ(32, bitMaskB[1]);
    ASSERT_EQ(1, bitMaskB[2]);
}

UTEST(test_BitSet, MoveBitField)
{
    Siege::BitUtils::BitSet bitMaskA(3);
    Siege::BitUtils::BitSet bitMaskB(3);

    // BitMask: 0000 0001 0010 0000 0000 1010
    bitMaskA.SetBit(2);
    bitMaskA.SetBit(4);
    bitMaskA.SetBit(14);
    bitMaskA.SetBit(17);

    ASSERT_EQ(17, bitMaskA.LeftMostBit());
    ASSERT_EQ(10, bitMaskA[0]);
    ASSERT_EQ(32, bitMaskA[1]);
    ASSERT_EQ(1, bitMaskA[2]);

    bitMaskB = std::move(bitMaskA);

    ASSERT_EQ(17, bitMaskB.LeftMostBit());
    ASSERT_EQ(10, bitMaskB[0]);
    ASSERT_EQ(32, bitMaskB[1]);
    ASSERT_EQ(1, bitMaskB[2]);

    ASSERT_EQ(0, bitMaskA.LeftMostBit());
}

UTEST(test_BitSet, TestGetFirstSetBit)
{
    Siege::BitUtils::BitSet bitMask(2);
    auto firstSet = bitMask.FirstSetBit();

    ASSERT_EQ(0, firstSet);

    bitMask.SetBit(5);
    bitMask.SetBit(7); // 0000 1010

    ASSERT_EQ(5, bitMask.FirstSetBit());
    ASSERT_EQ(7, bitMask.LeftMostBit());

    bitMask.UnsetBit(5);
    ASSERT_EQ(7, bitMask.FirstSetBit());

    bitMask.SetBit(12);
    ASSERT_EQ(7, bitMask.FirstSetBit());

    bitMask.SetBit(1);
    ASSERT_EQ(1, bitMask.FirstSetBit());
}

// --------------------------------------- SBitSet ------------------------------------------------

UTEST(test_SBitSet, CreateBitSet)
{
    Siege::BitUtils::SBitSet<1> bitMask;

    ASSERT_EQ(0, bitMask.LeftMostBit());
}

UTEST(test_SBitSet, AddValueToFirstBit)
{
    Siege::BitUtils::SBitSet<1> bitMask;

    ASSERT_EQ(0, bitMask.LeftMostBit());

    bitMask.SetBit(1);

    ASSERT_TRUE(bitMask.IsSet(1));
    ASSERT_EQ(1, bitMask.LeftMostBit());
}

UTEST(test_SBitSet, AddValueToBitMask)
{
    Siege::BitUtils::SBitSet<1> bitMask;

    ASSERT_EQ(0, bitMask.LeftMostBit());

    bitMask.SetBit(2);

    ASSERT_TRUE(bitMask.IsSet(2));
    ASSERT_EQ(2, bitMask.LeftMostBit());
}

UTEST(test_SBitSet, GetLeftMostBit)
{
    Siege::BitUtils::SBitSet<1> bitMask;

    ASSERT_EQ(0, bitMask.LeftMostBit());

    bitMask.SetBit(2);

    ASSERT_TRUE(bitMask.IsSet(2));
    ASSERT_EQ(2, bitMask.LeftMostBit());

    bitMask.SetBit(1);

    ASSERT_TRUE(bitMask.IsSet(1));
    ASSERT_EQ(2, bitMask.LeftMostBit());

    bitMask.SetBit(4);
    ASSERT_TRUE(bitMask.IsSet(4));
    ASSERT_EQ(4, bitMask.LeftMostBit());
}

UTEST(test_SBitSet, RemoveBit)
{
    Siege::BitUtils::SBitSet<1> bitMask;

    ASSERT_EQ(0, bitMask.LeftMostBit());

    // 0001 0100
    bitMask.SetBit(2);
    bitMask.SetBit(5);

    ASSERT_TRUE(bitMask.IsSet(2));
    ASSERT_TRUE(bitMask.IsSet(5));
    ASSERT_EQ(5, bitMask.LeftMostBit());

    bitMask.UnsetBit(5);
    ASSERT_EQ(2, bitMask.LeftMostBit());
}

UTEST(test_SBitSet, RemoveBitFromAnotherByte)
{
    Siege::BitUtils::SBitSet<2> bitMask;

    ASSERT_EQ(0, bitMask.LeftMostBit());

    // 0000 0001 0001 0010
    bitMask.SetBit(2);
    bitMask.SetBit(5);
    bitMask.SetBit(9);

    ASSERT_TRUE(bitMask.IsSet(2));
    ASSERT_TRUE(bitMask.IsSet(5));
    ASSERT_TRUE(bitMask.IsSet(9));

    ASSERT_EQ(9, bitMask.LeftMostBit());

    bitMask.UnsetBit(5); // 0000 0001 0000 0010
    ASSERT_EQ(9, bitMask.LeftMostBit());

    bitMask.UnsetBit(9);
    ASSERT_EQ(2, bitMask.LeftMostBit());
}

UTEST(test_SBitSet, RemoveBitFromALargeMask)
{
    // 0000 0000 | 0100 0000 | 0000 1000 | 0100 0000 | 0001 0000
    Siege::BitUtils::SBitSet<5> bitMask;

    ASSERT_EQ(0, bitMask.LeftMostBit());

    // 0000 0001 0001 0010
    bitMask.SetBit(5);
    bitMask.SetBit(15);
    bitMask.SetBit(20);
    bitMask.SetBit(31);

    ASSERT_TRUE(bitMask.IsSet(5));
    ASSERT_TRUE(bitMask.IsSet(15));
    ASSERT_TRUE(bitMask.IsSet(20));
    ASSERT_TRUE(bitMask.IsSet(31));

    ASSERT_EQ(31, bitMask.LeftMostBit());

    bitMask.UnsetBit(5); // 0000 0001 0000 0010
    ASSERT_EQ(31, bitMask.LeftMostBit());

    bitMask.UnsetBit(31);
    ASSERT_EQ(20, bitMask.LeftMostBit());

    bitMask.UnsetBit(20);
    ASSERT_EQ(15, bitMask.LeftMostBit());

    bitMask.UnsetBit(15);
    ASSERT_EQ(0, bitMask.LeftMostBit());
}

UTEST(test_SBitSet, SetBitsToOne)
{
    Siege::BitUtils::SBitSet<1> bitMask;

    bitMask.SetBitsToOne(2);

    ASSERT_EQ(2, bitMask.LeftMostBit());
    ASSERT_EQ(3, bitMask[0]);
}

UTEST(test_SBitSet, SetLargeBitSetToOne)
{
    Siege::BitUtils::SBitSet<3> bitMask;

    // 0000 1111 1111 1111 1111 1111
    bitMask.SetBitsToOne(20);

    ASSERT_EQ(20, bitMask.LeftMostBit());
    ASSERT_EQ(255, bitMask[0]);
    ASSERT_EQ(255, bitMask[1]);
    ASSERT_EQ(15, bitMask[2]);
}

UTEST(test_SBitSet, CopyBitField)
{
    Siege::BitUtils::SBitSet<3> bitMaskA;
    Siege::BitUtils::SBitSet<3> bitMaskB;

    // BitMask: 0000 0001 0010 0000 0000 1010
    bitMaskA.SetBit(2);
    bitMaskA.SetBit(4);
    bitMaskA.SetBit(14);
    bitMaskA.SetBit(17);

    ASSERT_EQ(17, bitMaskA.LeftMostBit());
    ASSERT_EQ(10, bitMaskA[0]);
    ASSERT_EQ(32, bitMaskA[1]);
    ASSERT_EQ(1, bitMaskA[2]);

    bitMaskB = bitMaskA;

    ASSERT_EQ(17, bitMaskB.LeftMostBit());
    ASSERT_EQ(10, bitMaskB[0]);
    ASSERT_EQ(32, bitMaskB[1]);
    ASSERT_EQ(1, bitMaskB[2]);
}