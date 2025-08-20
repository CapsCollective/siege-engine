//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <utest.h>
#include <utils/TlsfAllocator.h>
#include <utils/String.h>

using namespace Siege;

struct TestStruct
{
    uint64_t inta {0};
    uint64_t intb {0};
};


UTEST(test_TlsfAllocator, EmptyConstructor)
{
    TlsfAllocator a;
    ASSERT_EQ(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 0);
    ASSERT_EQ(0, a.BytesRemaining());
}

UTEST(test_TlsfAllocator, ConstructorWithSize)
{
    TlsfAllocator a(64);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 64);
    ASSERT_EQ(64, a.BytesRemaining());

    ASSERT_EQ(a.FlBitmask(), 4);
    ASSERT_EQ(4, a.SlBitmask(2));

    TlsfAllocator::FreeBlockNode* block = a.GetFreeBlock(2, 2);
    ASSERT_TRUE(block);
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);

    TlsfAllocator::BlockHeader* header = a.GetHeader(block);
    ASSERT_TRUE(header);
    ASSERT_EQ(72, a.GetHeaderSize(header));
    ASSERT_TRUE(a.IsFree(header));
    ASSERT_FALSE(a.PrevBlockIsFree(header));

    TlsfAllocator::BlockFooter* footer = a.GetFooter(header);
    ASSERT_TRUE(footer);
    ASSERT_EQ(72, footer->totalBlockSize);
}

UTEST(test_TlsfAllocator, ConstructorWithTooSmallSize)
{
    Siege::TlsfAllocator a(15);
    ASSERT_EQ(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 0);
}

UTEST(test_TlsfAllocator, TestAllocateFunction)
{
    TlsfAllocator a(64);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 64);
    ASSERT_EQ(64, a.BytesRemaining());

    ASSERT_EQ(a.FlBitmask(), 4);
    ASSERT_EQ(4, a.SlBitmask(2));

    TestStruct* p = (TestStruct*) a.Allocate(sizeof(TestStruct));

    p->inta = 10;
    p->intb = 20;

    ASSERT_EQ(10, p->inta);
    ASSERT_EQ(20, p->intb);

    ASSERT_EQ(2, a.FlBitmask());
    ASSERT_EQ(256, a.SlBitmask(1));

    TlsfAllocator::FreeBlockNode* block = a.GetFreeBlock(1, 8);
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);

    auto header = (TlsfAllocator::BlockHeader*)a.Data();
    auto data = (TestStruct*)a.GetBlockData(header);
    auto footer = a.GetFooter(header);

    ASSERT_EQ(192, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(TlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(TlsfAllocator::BlockFooter), footer->totalBlockSize);
    ASSERT_EQ(40, a.BytesRemaining());

    Siege::String* str = (Siege::String*) a.Allocate(sizeof(Siege::String));

    *str = "Hello There!";
    ASSERT_STREQ(str->Str(),"Hello There!");

    ASSERT_EQ(1, a.FlBitmask());
    ASSERT_EQ(0, a.SlBitmask(1));

    auto strHeader = a.GetNextHeader(header);
    auto strData = (String*)a.GetBlockData(strHeader);
    auto strFooter = a.GetFooter(strHeader);

    ASSERT_EQ(192, strHeader->sizeAndFlags);
    ASSERT_EQ(strData, str);
    ASSERT_EQ(sizeof(TlsfAllocator::BlockHeader) + sizeof(String) + sizeof(TlsfAllocator::BlockFooter), strFooter->totalBlockSize);
    ASSERT_EQ(16, a.BytesRemaining());

    // Edge cases

    // Empty allocator
    TlsfAllocator emptyA;

    TestStruct* ptr = (TestStruct*) emptyA.Allocate(sizeof(TestStruct));
    ASSERT_FALSE(ptr);

    // Allocate 0 bytes
    void* emptyAllocPtr = a.Allocate(0);
    ASSERT_FALSE(emptyAllocPtr);

    // Allocate an amount that causes a value overflow
    TlsfAllocator overflowAlloc(UINT16_MAX);
    void* overflowPtr = overflowAlloc.Allocate(UINT16_MAX);
    ASSERT_FALSE(overflowPtr);

    void* tooLargeCase = a.Allocate(100);
    ASSERT_FALSE(tooLargeCase);

    TlsfAllocator tooSmallAlloc(64);
    void* tooLargeAlloc = tooSmallAlloc.Allocate(100);
    ASSERT_FALSE(tooLargeAlloc);
}