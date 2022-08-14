//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include <utest.h>
#include <utils/ArrayUtils.h>

UTEST(test_SBitMaskField, CreateBitMaskField)
{
    Siege::Utils::ArrayUtils::SBitMaskField<1> bitMask;

    ASSERT_EQ(0, bitMask.LeftMostBit());
}

UTEST(test_SBitMaskField, AddValueToFirstBit)
{
    Siege::Utils::ArrayUtils::SBitMaskField<1> bitMask;

    ASSERT_EQ(0, bitMask.LeftMostBit());

    bitMask.SetBit(1);

    ASSERT_TRUE(bitMask.IsSet(1));
    ASSERT_EQ(1, bitMask.LeftMostBit());
}

UTEST(test_SBitMaskField, AddValueToBitMask)
{
    Siege::Utils::ArrayUtils::SBitMaskField<1> bitMask;

    ASSERT_EQ(0, bitMask.LeftMostBit());

    bitMask.SetBit(2);

    ASSERT_TRUE(bitMask.IsSet(2));
    ASSERT_EQ(2, bitMask.LeftMostBit());
}

UTEST(test_SBitMaskField, GetLeftMostBit)
{
    Siege::Utils::ArrayUtils::SBitMaskField<1> bitMask;

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

UTEST(test_SBitMaskField, RemoveBit)
{
    Siege::Utils::ArrayUtils::SBitMaskField<1> bitMask;

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

UTEST(test_SBitMaskField, RemoveBitFromAnotherByte)
{
    Siege::Utils::ArrayUtils::SBitMaskField<2> bitMask;

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

UTEST(test_SBitMaskField, RemoveBitFromALargeMask)
{
    // 0000 0000 | 0100 0000 | 0000 1000 | 0100 0000 | 0001 0000
    Siege::Utils::ArrayUtils::SBitMaskField<5> bitMask;

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

UTEST(test_SBitMaskField, SetBitsToOne)
{
    Siege::Utils::ArrayUtils::SBitMaskField<1> bitMask;

    bitMask.SetBitsToOne(2);

    ASSERT_EQ(2, bitMask.LeftMostBit());
    ASSERT_EQ(3, bitMask[0]);
}

UTEST(test_SBitMaskField, SetLargeBitSetToOne)
{
    Siege::Utils::ArrayUtils::SBitMaskField<3> bitMask;

    // 0000 1111 1111 1111 1111 1111
    bitMask.SetBitsToOne(20);

    ASSERT_EQ(20, bitMask.LeftMostBit());
    ASSERT_EQ(255, bitMask[0]);
    ASSERT_EQ(255, bitMask[1]);
    ASSERT_EQ(15, bitMask[2]);
}