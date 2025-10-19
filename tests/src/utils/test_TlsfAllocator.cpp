//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <utest.h>
#include <utils/String.h>
#include <utils/allocators/Tlsf.h>

#include <chrono>
#include <memory>
#include <random>
#include <string>

using namespace Siege;

struct TestStruct
{
    uint64_t inta {0};
    uint64_t intb {0};
};

UTEST(test_MediumTlsfAllocator, EmptyConstructor)
{
    MediumTlsfAllocator a;
    ASSERT_EQ(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 0);
    ASSERT_EQ(0, a.BytesRemaining());
}

UTEST(test_MediumTlsfAllocator, ConstructorWithSize)
{
    MediumTlsfAllocator a(64);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.TotalSize(), 72);
    ASSERT_EQ(64, a.BytesRemaining());

    ASSERT_EQ(a.FirstLevelBitmask(), 4);
    ASSERT_EQ(4, a.SecondLevelBitmask(2));

    MediumTlsfAllocator::FreeBlockNode* block = a.GetFreeBlock(2, 2);
    ASSERT_TRUE(block);
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);

    MediumTlsfAllocator::BlockHeader* header = a.GetHeader(block);
    ASSERT_TRUE(header);
    ASSERT_EQ(72, a.GetHeaderSize(header));
    ASSERT_TRUE(a.IsFree(header));
    ASSERT_FALSE(a.PrevBlockIsFree(header));

    MediumTlsfAllocator::BlockFooter* footer = a.GetFooter(header);
    ASSERT_TRUE(footer);
    ASSERT_EQ(72, footer->totalBlockSize);
}

UTEST(test_MediumTlsfAllocator, ConstructorWithTooSmallSize)
{
    Siege::MediumTlsfAllocator a(7);
    ASSERT_EQ(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 0);
}

UTEST(test_MediumTlsfAllocator, TestAllocateFunction)
{
    MediumTlsfAllocator a(64);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 64);
    ASSERT_EQ(64, a.BytesRemaining());

    ASSERT_EQ(a.FirstLevelBitmask(), 4);
    ASSERT_EQ(4, a.SecondLevelBitmask(2));

    TestStruct* p = (TestStruct*) a.Allocate(sizeof(TestStruct));

    p->inta = 10;
    p->intb = 20;

    ASSERT_EQ(10, p->inta);
    ASSERT_EQ(20, p->intb);

    ASSERT_EQ(2, a.FirstLevelBitmask());
    ASSERT_EQ(256, a.SecondLevelBitmask(1));

    MediumTlsfAllocator::FreeBlockNode* block = a.GetFreeBlock(1, 8);
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);

    auto header = (MediumTlsfAllocator::BlockHeader*) a.Data();
    auto data = (TestStruct*) a.GetBlockData(header);
    auto footer = a.GetFooter(header);

    ASSERT_EQ(192, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(MediumTlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(MediumTlsfAllocator::BlockFooter),
              footer->totalBlockSize);
    ASSERT_EQ(a.BytesRemaining(), 48);

    Siege::String* str = (Siege::String*) a.Allocate(sizeof(Siege::String));

    *str = "Hello There!";
    ASSERT_STREQ(str->Str(), "Hello There!");

    ASSERT_EQ(1, a.FirstLevelBitmask());
    ASSERT_EQ(0, a.SecondLevelBitmask(1));

    auto strHeader = a.GetNextHeader(header);
    auto strData = (String*) a.GetBlockData(strHeader);
    auto strFooter = a.GetFooter(strHeader);

    ASSERT_EQ(192, strHeader->sizeAndFlags);
    ASSERT_EQ(strData, str);
    ASSERT_EQ(sizeof(MediumTlsfAllocator::BlockHeader) + sizeof(String) +
                  sizeof(MediumTlsfAllocator::BlockFooter),
              strFooter->totalBlockSize);
    ASSERT_EQ(32, a.BytesRemaining());

    // Edge cases

    // Empty allocator
    MediumTlsfAllocator emptyA;

    TestStruct* ptr = (TestStruct*) emptyA.Allocate(sizeof(TestStruct));
    ASSERT_FALSE(ptr);

    // Allocate 0 bytes
    void* emptyAllocPtr = a.Allocate(0);
    ASSERT_FALSE(emptyAllocPtr);

    // Allocate an amount that causes a value overflow
    MediumTlsfAllocator overflowAlloc(UINT32_MAX);
    void* overflowPtr = overflowAlloc.Allocate(UINT32_MAX);
    ASSERT_FALSE(overflowPtr);

    void* tooLargeCase = a.Allocate(100);
    ASSERT_FALSE(tooLargeCase);

    MediumTlsfAllocator tooSmallAlloc(64);
    void* tooLargeAlloc = tooSmallAlloc.Allocate(100);
    ASSERT_FALSE(tooLargeAlloc);
}

UTEST(Test_MediumTlsfAllocator, TestAllocateFunctionWithRandomInputs)
{
    MediumTlsfAllocator a(1024 * 1024);

    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    std::uniform_int_distribution<int> sizeDist(1, 256);

    MediumTlsfAllocator::BlockHeader* header = (MediumTlsfAllocator::BlockHeader*) a.Data();

    for (int i = 0; i < 10000; i++)
    {
        size_t randomSize = sizeDist(generator);
        void* ptr = a.Allocate(randomSize);

        if (ptr)
        {
            *((uint32_t*) ptr) = 0xDEADC0DE;

            uint64_t expectedSize = header->sizeAndFlags >> 3;
            uint32_t* data = (uint32_t*) a.GetBlockData(header);
            ASSERT_EQ(0xDEADC0DE, *data);
            MediumTlsfAllocator::BlockFooter* footer = a.GetFooter(header);
            header = a.GetNextHeader(header);
        }
        else continue;
    }
}

UTEST(Test_MediumTlsfAllocator, TestDeallocateFunction)
{
    MediumTlsfAllocator a(64);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 64);
    ASSERT_EQ(64, a.BytesRemaining());

    ASSERT_EQ(a.FirstLevelBitmask(), 4);
    ASSERT_EQ(4, a.SecondLevelBitmask(2));

    TestStruct* p = (TestStruct*) a.Allocate(sizeof(TestStruct));

    p->inta = 10;
    p->intb = 20;

    ASSERT_EQ(10, p->inta);
    ASSERT_EQ(20, p->intb);

    ASSERT_EQ(2, a.FirstLevelBitmask());
    ASSERT_EQ(256, a.SecondLevelBitmask(1));

    MediumTlsfAllocator::FreeBlockNode* block = a.GetFreeBlock(1, 8);
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);

    auto header = (MediumTlsfAllocator::BlockHeader*) a.Data();
    auto data = (TestStruct*) a.GetBlockData(header);
    auto footer = a.GetFooter(header);

    ASSERT_EQ(192, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(MediumTlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(MediumTlsfAllocator::BlockFooter),
              footer->totalBlockSize);
    ASSERT_EQ(48, a.BytesRemaining());

    a.TLSF_ALLOC_FREE(p);

    // Should be empty
    ASSERT_EQ(577, header->sizeAndFlags);
    ASSERT_TRUE(header->sizeAndFlags & MediumTlsfAllocator::FREE);
    ASSERT_FALSE(p);
    ASSERT_EQ(4, a.FirstLevelBitmask());
    ASSERT_EQ(4, a.SecondLevelBitmask(2));
    ASSERT_EQ(64, a.BytesRemaining());

    p = (TestStruct*) a.Allocate(sizeof(TestStruct));
    p->inta = 10;
    p->intb = 20;

    ASSERT_EQ(10, p->inta);
    ASSERT_EQ(20, p->intb);

    ASSERT_EQ(2, a.FirstLevelBitmask());
    ASSERT_EQ(256, a.SecondLevelBitmask(1));

    block = a.GetFreeBlock(1, 8);
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);

    header = (MediumTlsfAllocator::BlockHeader*) a.Data();
    data = (TestStruct*) a.GetBlockData(header);
    footer = a.GetFooter(header);

    ASSERT_EQ(192, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(MediumTlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(MediumTlsfAllocator::BlockFooter),
              footer->totalBlockSize);
    ASSERT_EQ(48, a.BytesRemaining());

    Siege::String* str = (Siege::String*) a.Allocate(sizeof(Siege::String));

    *str = "Hello There!";
    ASSERT_STREQ(str->Str(), "Hello There!");

    ASSERT_EQ(1, a.FirstLevelBitmask());
    ASSERT_EQ(0, a.SecondLevelBitmask(1));

    auto strHeader = a.GetNextHeader(header);
    auto strData = (String*) a.GetBlockData(strHeader);
    auto strFooter = a.GetFooter(strHeader);

    ASSERT_EQ(192, strHeader->sizeAndFlags);
    ASSERT_EQ(strData, str);
    ASSERT_EQ(sizeof(MediumTlsfAllocator::BlockHeader) + sizeof(String) +
                  sizeof(MediumTlsfAllocator::BlockFooter),
              strFooter->totalBlockSize);
    ASSERT_EQ(32, a.BytesRemaining());

    a.TLSF_ALLOC_FREE(p);
    ASSERT_FALSE(p);
    ASSERT_TRUE(strHeader->sizeAndFlags & MediumTlsfAllocator::PREV_IS_FREE);
    ASSERT_EQ(193, header->sizeAndFlags);
    ASSERT_TRUE(header->sizeAndFlags & MediumTlsfAllocator::FREE);
    MediumTlsfAllocator::FreeBlockNode* newFreeBlock = a.GetFreeBlock(header);
    ASSERT_NE(nullptr, newFreeBlock->next);
    ASSERT_EQ(nullptr, newFreeBlock->prev);
    ASSERT_EQ(48, a.BytesRemaining());

    a.TLSF_ALLOC_FREE(str);
    ASSERT_FALSE(str);
    ASSERT_EQ(577, header->sizeAndFlags);
    ASSERT_EQ(4, a.FirstLevelBitmask());
    ASSERT_EQ(4, a.SecondLevelBitmask(2));
    ASSERT_EQ(64, a.BytesRemaining());

    MediumTlsfAllocator::FreeBlockNode* newNode = (MediumTlsfAllocator::FreeBlockNode*) data;
    ASSERT_TRUE(newNode);
    // Check if the new node is head
    ASSERT_EQ(nullptr, newNode->prev);
    ASSERT_EQ(nullptr, newNode->next);
}

UTEST(Test_MediumTlsfAllocator, TestBlockCoalescing)
{
    MediumTlsfAllocator a(128);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 128);
    ASSERT_EQ(128, a.BytesRemaining());

    ASSERT_EQ(a.FirstLevelBitmask(), 8);
    ASSERT_EQ(2, a.SecondLevelBitmask(3));

    TestStruct* p = (TestStruct*) a.Allocate(sizeof(TestStruct));

    p->inta = 10;
    p->intb = 20;

    ASSERT_EQ(10, p->inta);
    ASSERT_EQ(20, p->intb);

    ASSERT_EQ(4, a.FirstLevelBitmask());
    ASSERT_EQ(4096, a.SecondLevelBitmask(2));
    // 0001 0000 0000 0000
    MediumTlsfAllocator::FreeBlockNode* block = a.GetFreeBlock(2, 12);
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);
    ASSERT_EQ(112, a.BytesRemaining());

    auto header = (MediumTlsfAllocator::BlockHeader*) a.Data();
    auto data = (TestStruct*) a.GetBlockData(header);
    auto footer = a.GetFooter(header);

    ASSERT_EQ(192, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(MediumTlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(MediumTlsfAllocator::BlockFooter),
              footer->totalBlockSize);

    Siege::String* str = (Siege::String*) a.Allocate(sizeof(Siege::String));

    *str = "Hello There!";
    ASSERT_STREQ(str->Str(), "Hello There!");

    ASSERT_EQ(4, a.FirstLevelBitmask());
    ASSERT_EQ(64, a.SecondLevelBitmask(2));

    MediumTlsfAllocator::FreeBlockNode* NewBlock = a.GetFreeBlock(2, 6);
    ASSERT_EQ(NewBlock->next, nullptr);
    ASSERT_EQ(NewBlock->prev, nullptr);
    ASSERT_EQ(96, a.BytesRemaining());

    auto strHeader = a.GetNextHeader(header);
    auto strData = (String*) a.GetBlockData(strHeader);
    auto strFooter = a.GetFooter(strHeader);

    ASSERT_EQ(192, strHeader->sizeAndFlags);
    ASSERT_EQ(strData, str);
    ASSERT_EQ(sizeof(MediumTlsfAllocator::BlockHeader) + sizeof(Siege::String) +
                  sizeof(MediumTlsfAllocator::BlockFooter),
              strFooter->totalBlockSize);

    TestStruct* p2 = (TestStruct*) a.Allocate(sizeof(TestStruct));

    p2->inta = 10;
    p2->intb = 20;

    ASSERT_EQ(10, p2->inta);
    ASSERT_EQ(20, p2->intb);

    ASSERT_EQ(4, a.FirstLevelBitmask());
    ASSERT_EQ(1, a.SecondLevelBitmask(2));

    MediumTlsfAllocator::FreeBlockNode* newFreeBlock = a.GetFreeBlock(2, 0);
    ASSERT_EQ(newFreeBlock->next, nullptr);
    ASSERT_EQ(newFreeBlock->prev, nullptr);
    ASSERT_EQ(80, a.BytesRemaining());

    auto newHeader = a.GetNextHeader(strHeader);
    auto newData = (String*) a.GetBlockData(newHeader);
    auto newFooter = a.GetFooter(newHeader);

    ASSERT_EQ(192, newHeader->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(MediumTlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(MediumTlsfAllocator::BlockFooter),
              footer->totalBlockSize);

    a.TLSF_ALLOC_FREE(p);

    MediumTlsfAllocator::FreeBlockNode* firstFree = a.GetFreeBlock(0, 8);
    ASSERT_EQ(firstFree->next, nullptr);
    ASSERT_EQ(firstFree->prev, nullptr);
    ASSERT_EQ(96, a.BytesRemaining());

    MediumTlsfAllocator::BlockHeader* firstFreeHeader = a.GetHeader((uint8_t*) firstFree);
    ASSERT_TRUE(firstFreeHeader->sizeAndFlags & MediumTlsfAllocator::FREE);
    ASSERT_FALSE(firstFreeHeader->sizeAndFlags & MediumTlsfAllocator::PREV_IS_FREE);

    ASSERT_FALSE(p);
    ASSERT_EQ(a.FirstLevelBitmask(), 5);
    ASSERT_EQ(256, a.SecondLevelBitmask(0));

    a.TLSF_ALLOC_FREE(p2);
    ASSERT_FALSE(p2);
    ASSERT_EQ(112, a.BytesRemaining());
    ASSERT_EQ(a.FirstLevelBitmask(), 5);
    ASSERT_EQ(256, a.SecondLevelBitmask(0));
    ASSERT_EQ(64, a.SecondLevelBitmask(2));

    a.TLSF_ALLOC_FREE(str);
    ASSERT_FALSE(str);
    ASSERT_EQ(128, a.BytesRemaining());
    ASSERT_EQ(8, a.FirstLevelBitmask());
    ASSERT_EQ(2, a.SecondLevelBitmask(3));

    // Edge Cases

    void* badPointer = nullptr;
    a.TLSF_ALLOC_FREE(badPointer);
    ASSERT_FALSE(badPointer);

    // try to deallocate pointer not in allocator;

    uint64_t* val = new uint64_t;
    a.TLSF_ALLOC_FREE(val); // Should do nothing and be ignored.
    free(val);
}

UTEST(Test_MediumTlsfAllocator, TestAllocationWhenNoAppropriateFragmentExists)
{
    MediumTlsfAllocator a(128);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 128);
    ASSERT_EQ(128, a.BytesRemaining());

    void* p0 = a.Allocate(16);
    void* p1 = a.Allocate(32);
    void* p2 = a.Allocate(16);
    void* p3 = a.Allocate(32);
    ASSERT_EQ(0, a.TotalBytesRemaining());
    ASSERT_TRUE(a.IsFull());

    a.TLSF_ALLOC_FREE(p0);
    a.TLSF_ALLOC_FREE(p2);

    ASSERT_EQ(64, a.BytesRemaining());
    void* tooLargeVal = a.Allocate(24);
    ASSERT_FALSE(tooLargeVal);
}

UTEST(test_ResourceSystem, TestRandomAllocationsAndDeallocations)
{
    MediumTlsfAllocator a(1024 * 1024);

    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    std::uniform_int_distribution<int> actionDist(0, 10);
    std::uniform_int_distribution<int> sizeDist(1, 256);

    std::vector<void*> pointers;
    pointers.reserve(10000);

    for (int i = 0; i < 10000; i++)
    {
        int action = actionDist(generator);

        if (action < 8)
        {
            size_t randomSize = sizeDist(generator);

            void* ptr = a.Allocate(randomSize);
            if (ptr)
            {
                *((uint32_t*) ptr) = 0xDEADC0DE;
                pointers.push_back(ptr);
                MediumTlsfAllocator::BlockHeader* header = a.GetHeader((uint8_t*) ptr);
            }
        }
        else if (action <= 10 && !pointers.empty())
        {
            std::uniform_int_distribution<int> indices(0, pointers.size() - 1);
            size_t index = indices(generator);

            void*& ptrToFree = pointers[index];
            ASSERT_EQ(0xDEADC0DE, *(uint32_t*) ptrToFree);
            MediumTlsfAllocator::BlockHeader* header = a.GetHeader((uint8_t*) ptrToFree);

            a.TLSF_ALLOC_FREE(ptrToFree);
            MediumTlsfAllocator::FreeBlockNode* newNode = a.GetFreeBlock(header);

            ASSERT_FALSE(ptrToFree);
            pointers.erase(pointers.begin() + index);
        }
    }

    for (void* ptr : pointers)
    {
        a.TLSF_ALLOC_FREE(ptr);
    }

    ASSERT_EQ(a.TotalSize(), a.TotalBytesRemaining());
}

// Small AllocatorTests

UTEST(Test_SmallTlsfAllocator, EmptyConstructor)
{
    SmallTlsfAllocator a;
    ASSERT_EQ(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 0);
    ASSERT_EQ(0, a.BytesRemaining());
}

UTEST(Test_SmallTlsfAllocator, ConstructorWithSize)
{
    SmallTlsfAllocator a(64);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(68, a.TotalSize());
    ASSERT_EQ(64, a.BytesRemaining());

    ASSERT_EQ(a.FirstLevelBitmask(), 4);
    ASSERT_EQ(2, a.SecondLevelBitmask(2));

    SmallTlsfAllocator::FreeBlockNode* block = a.GetFreeBlock(2, 1);
    ASSERT_TRUE(block);
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);

    SmallTlsfAllocator::BlockHeader* header = a.GetHeader(block);
    ASSERT_TRUE(header);
    ASSERT_EQ(68, a.GetHeaderSize(header));
    ASSERT_TRUE(a.IsFree(header));
    ASSERT_FALSE(a.PrevBlockIsFree(header));

    SmallTlsfAllocator::BlockFooter* footer = a.GetFooter(header);
    ASSERT_TRUE(footer);
    ASSERT_EQ(68, footer->totalBlockSize);
}

UTEST(Test_SmallTlsfAllocator, ConstructorWithTooSmallSize)
{
    Siege::SmallTlsfAllocator a(15);
    ASSERT_EQ(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 0);
}

UTEST(Test_SmallTlsfAllocator, TestAllocateFunction)
{
    SmallTlsfAllocator a(64);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 64);
    ASSERT_EQ(64, a.BytesRemaining());

    ASSERT_EQ(a.FirstLevelBitmask(), 4);
    ASSERT_EQ(2, a.SecondLevelBitmask(2));

    TestStruct* p = (TestStruct*) a.Allocate(sizeof(TestStruct));

    p->inta = 10;
    p->intb = 20;

    ASSERT_EQ(10, p->inta);
    ASSERT_EQ(20, p->intb);

    ASSERT_EQ(2, a.FirstLevelBitmask());
    ASSERT_EQ(256, a.SecondLevelBitmask(1));

    SmallTlsfAllocator::FreeBlockNode* block = a.GetFreeBlock(1, 8);
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);

    auto header = (SmallTlsfAllocator::BlockHeader*) a.Data();
    auto data = (TestStruct*) a.GetBlockData(header);
    auto footer = a.GetFooter(header);

    ASSERT_EQ(160, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(SmallTlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(SmallTlsfAllocator::BlockFooter),
              footer->totalBlockSize);
    ASSERT_EQ(a.BytesRemaining(), 48);

    Siege::String* str = (Siege::String*) a.Allocate(sizeof(Siege::String));

    *str = "Hello There!";
    ASSERT_STREQ(str->Str(), "Hello There!");

    ASSERT_EQ(1, a.FirstLevelBitmask());
    ASSERT_EQ(0, a.SecondLevelBitmask(1));

    auto strHeader = a.GetNextHeader(header);
    auto strData = (String*) a.GetBlockData(strHeader);
    auto strFooter = a.GetFooter(strHeader);

    ASSERT_EQ(160, strHeader->sizeAndFlags);
    ASSERT_EQ(strData, str);
    ASSERT_EQ(sizeof(SmallTlsfAllocator::BlockHeader) + sizeof(String) +
                  sizeof(SmallTlsfAllocator::BlockFooter),
              strFooter->totalBlockSize);
    ASSERT_EQ(32, a.BytesRemaining());

    // Edge cases

    // Empty allocator
    SmallTlsfAllocator emptyA;

    TestStruct* ptr = (TestStruct*) emptyA.Allocate(sizeof(TestStruct));
    ASSERT_FALSE(ptr);

    // Allocate 0 bytes
    void* emptyAllocPtr = a.Allocate(0);
    ASSERT_FALSE(emptyAllocPtr);

    // Allocate an amount that causes a value overflow
    SmallTlsfAllocator overflowAlloc(UINT16_MAX);
    void* overflowPtr = overflowAlloc.Allocate(UINT16_MAX);
    ASSERT_FALSE(overflowPtr);

    void* tooLargeCase = a.Allocate(100);
    ASSERT_FALSE(tooLargeCase);

    SmallTlsfAllocator tooSmallAlloc(64);
    void* tooLargeAlloc = tooSmallAlloc.Allocate(100);
    ASSERT_FALSE(tooLargeAlloc);
}

UTEST(Test_SmallTlsfAllocator, TestAllocateFunctionWithRandomInputs)
{
    SmallTlsfAllocator a(8000);

    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    std::uniform_int_distribution<int> sizeDist(1, 256);

    SmallTlsfAllocator::BlockHeader* header = (SmallTlsfAllocator::BlockHeader*) a.Data();

    for (int i = 0; i < 10000; i++)
    {
        size_t randomSize = sizeDist(generator);
        void* ptr = a.Allocate(randomSize);

        if (ptr)
        {
            *((uint32_t*) ptr) = 0xDEADC0DE;

            uint64_t expectedSize = header->sizeAndFlags >> 3;
            uint32_t* data = (uint32_t*) a.GetBlockData(header);
            ASSERT_EQ(0xDEADC0DE, *data);
            SmallTlsfAllocator::BlockFooter* footer = a.GetFooter(header);
            header = a.GetNextHeader(header);
        }
        else continue;
    }
}

UTEST(Test_SmallTlsfAllocator, TestDeallocateFunction)
{
    SmallTlsfAllocator a(64);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 64);
    ASSERT_EQ(64, a.BytesRemaining());

    ASSERT_EQ(a.FirstLevelBitmask(), 4);
    ASSERT_EQ(2, a.SecondLevelBitmask(2));

    TestStruct* p = (TestStruct*) a.Allocate(sizeof(TestStruct));

    p->inta = 10;
    p->intb = 20;

    ASSERT_EQ(10, p->inta);
    ASSERT_EQ(20, p->intb);

    ASSERT_EQ(2, a.FirstLevelBitmask());
    ASSERT_EQ(256, a.SecondLevelBitmask(1));

    SmallTlsfAllocator::FreeBlockNode* block = a.GetFreeBlock(1, 8);
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);

    auto header = (SmallTlsfAllocator::BlockHeader*) a.Data();
    auto data = (TestStruct*) a.GetBlockData(header);
    auto footer = a.GetFooter(header);

    ASSERT_EQ(160, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(SmallTlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(SmallTlsfAllocator::BlockFooter),
              footer->totalBlockSize);
    ASSERT_EQ(48, a.BytesRemaining());

    a.TLSF_ALLOC_FREE(p);

    // Should be empty
    ASSERT_EQ(545, header->sizeAndFlags);
    ASSERT_TRUE(header->sizeAndFlags & SmallTlsfAllocator::FREE);
    ASSERT_FALSE(p);
    ASSERT_EQ(4, a.FirstLevelBitmask());
    ASSERT_EQ(2, a.SecondLevelBitmask(2));
    ASSERT_EQ(64, a.BytesRemaining());

    p = (TestStruct*) a.Allocate(sizeof(TestStruct));
    p->inta = 10;
    p->intb = 20;

    ASSERT_EQ(10, p->inta);
    ASSERT_EQ(20, p->intb);

    ASSERT_EQ(2, a.FirstLevelBitmask());
    ASSERT_EQ(256, a.SecondLevelBitmask(1));

    block = a.GetFreeBlock(1, 8);
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);

    header = (SmallTlsfAllocator::BlockHeader*) a.Data();
    data = (TestStruct*) a.GetBlockData(header);
    footer = a.GetFooter(header);

    ASSERT_EQ(160, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(SmallTlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(SmallTlsfAllocator::BlockFooter),
              footer->totalBlockSize);
    ASSERT_EQ(48, a.BytesRemaining());

    Siege::String* str = (Siege::String*) a.Allocate(sizeof(Siege::String));

    *str = "Hello There!";
    ASSERT_STREQ(str->Str(), "Hello There!");

    ASSERT_EQ(1, a.FirstLevelBitmask());
    ASSERT_EQ(0, a.SecondLevelBitmask(1));

    auto strHeader = a.GetNextHeader(header);
    auto strData = (String*) a.GetBlockData(strHeader);
    auto strFooter = a.GetFooter(strHeader);

    ASSERT_EQ(160, strHeader->sizeAndFlags);
    ASSERT_EQ(strData, str);
    ASSERT_EQ(sizeof(SmallTlsfAllocator::BlockHeader) + sizeof(String) +
                  sizeof(SmallTlsfAllocator::BlockFooter),
              strFooter->totalBlockSize);
    ASSERT_EQ(32, a.BytesRemaining());

    a.TLSF_ALLOC_FREE(p);
    ASSERT_FALSE(p);
    ASSERT_TRUE(strHeader->sizeAndFlags & SmallTlsfAllocator::PREV_IS_FREE);
    ASSERT_EQ(161, header->sizeAndFlags);
    ASSERT_TRUE(header->sizeAndFlags & SmallTlsfAllocator::FREE);
    SmallTlsfAllocator::FreeBlockNode* newFreeBlock = a.GetFreeBlock(header);
    ASSERT_EQ(nullptr, newFreeBlock->next);
    ASSERT_EQ(nullptr, newFreeBlock->prev);
    ASSERT_EQ(48, a.BytesRemaining());

    a.TLSF_ALLOC_FREE(str);
    ASSERT_FALSE(str);
    ASSERT_EQ(545, header->sizeAndFlags);
    ASSERT_EQ(4, a.FirstLevelBitmask());
    ASSERT_EQ(2, a.SecondLevelBitmask(2));
    ASSERT_EQ(64, a.BytesRemaining());

    SmallTlsfAllocator::FreeBlockNode* newNode = (SmallTlsfAllocator::FreeBlockNode*) data;
    ASSERT_TRUE(newNode);
    // Check if the new node is head
    ASSERT_EQ(nullptr, newNode->prev);
    ASSERT_EQ(nullptr, newNode->next);
}

UTEST(Test_SmallTlsfAllocator, TestBlockCoalescing)
{
    SmallTlsfAllocator a(128);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 128);
    ASSERT_EQ(128, a.BytesRemaining());

    ASSERT_EQ(a.FirstLevelBitmask(), 8);
    ASSERT_EQ(1, a.SecondLevelBitmask(3));

    TestStruct* p = (TestStruct*) a.Allocate(sizeof(TestStruct));

    p->inta = 10;
    p->intb = 20;

    ASSERT_EQ(10, p->inta);
    ASSERT_EQ(20, p->intb);

    ASSERT_EQ(4, a.FirstLevelBitmask());
    ASSERT_EQ(4096, a.SecondLevelBitmask(2));
    // 0001 0000 0000 0000
    SmallTlsfAllocator::FreeBlockNode* block = a.GetFreeBlock(2, 12);
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);
    ASSERT_EQ(112, a.BytesRemaining());

    auto header = (SmallTlsfAllocator::BlockHeader*) a.Data();
    auto data = (TestStruct*) a.GetBlockData(header);
    auto footer = a.GetFooter(header);

    ASSERT_EQ(160, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(SmallTlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(SmallTlsfAllocator::BlockFooter),
              footer->totalBlockSize);

    Siege::String* str = (Siege::String*) a.Allocate(sizeof(Siege::String));

    *str = "Hello There!";
    ASSERT_STREQ(str->Str(), "Hello There!");

    ASSERT_EQ(4, a.FirstLevelBitmask());
    ASSERT_EQ(128, a.SecondLevelBitmask(2));

    SmallTlsfAllocator::FreeBlockNode* NewBlock = a.GetFreeBlock(2, 7);
    ASSERT_EQ(NewBlock->next, nullptr);
    ASSERT_EQ(NewBlock->prev, nullptr);
    ASSERT_EQ(96, a.BytesRemaining());

    auto strHeader = a.GetNextHeader(header);
    auto strData = (String*) a.GetBlockData(strHeader);
    auto strFooter = a.GetFooter(strHeader);

    ASSERT_EQ(160, strHeader->sizeAndFlags);
    ASSERT_EQ(strData, str);
    ASSERT_EQ(sizeof(SmallTlsfAllocator::BlockHeader) + sizeof(Siege::String) +
                  sizeof(SmallTlsfAllocator::BlockFooter),
              strFooter->totalBlockSize);

    TestStruct* p2 = (TestStruct*) a.Allocate(sizeof(TestStruct));

    p2->inta = 10;
    p2->intb = 20;

    ASSERT_EQ(10, p2->inta);
    ASSERT_EQ(20, p2->intb);

    ASSERT_EQ(4, a.FirstLevelBitmask());
    ASSERT_EQ(4, a.SecondLevelBitmask(2));

    SmallTlsfAllocator::FreeBlockNode* newFreeBlock = a.GetFreeBlock(2, 2);
    ASSERT_EQ(newFreeBlock->next, nullptr);
    ASSERT_EQ(newFreeBlock->prev, nullptr);
    ASSERT_EQ(80, a.BytesRemaining());

    auto newHeader = a.GetNextHeader(strHeader);
    auto newData = (String*) a.GetBlockData(newHeader);
    auto newFooter = a.GetFooter(newHeader);

    ASSERT_EQ(160, newHeader->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(SmallTlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(SmallTlsfAllocator::BlockFooter),
              footer->totalBlockSize);

    a.TLSF_ALLOC_FREE(p);

    SmallTlsfAllocator::FreeBlockNode* firstFree = a.GetFreeBlock(0, 4);
    ASSERT_EQ(firstFree->next, nullptr);
    ASSERT_EQ(firstFree->prev, nullptr);
    ASSERT_EQ(96, a.BytesRemaining());

    SmallTlsfAllocator::BlockHeader* firstFreeHeader = a.GetHeader((uint8_t*) firstFree);
    ASSERT_TRUE(firstFreeHeader->sizeAndFlags & SmallTlsfAllocator::FREE);
    ASSERT_FALSE(firstFreeHeader->sizeAndFlags & SmallTlsfAllocator::PREV_IS_FREE);

    ASSERT_FALSE(p);
    ASSERT_EQ(a.FirstLevelBitmask(), 5);
    ASSERT_EQ(16, a.SecondLevelBitmask(0));

    a.TLSF_ALLOC_FREE(p2);
    ASSERT_FALSE(p2);
    ASSERT_EQ(112, a.BytesRemaining());
    ASSERT_EQ(a.FirstLevelBitmask(), 5);
    ASSERT_EQ(16, a.SecondLevelBitmask(0));
    ASSERT_EQ(128, a.SecondLevelBitmask(2));

    a.TLSF_ALLOC_FREE(str);
    ASSERT_FALSE(str);
    ASSERT_EQ(128, a.BytesRemaining());
    ASSERT_EQ(8, a.FirstLevelBitmask());
    ASSERT_EQ(1, a.SecondLevelBitmask(3));

    // Edge Cases

    void* badPointer = nullptr;
    a.TLSF_ALLOC_FREE(badPointer);
    ASSERT_FALSE(badPointer);

    // try to deallocate pointer not in allocator;

    uint64_t* val = new uint64_t;
    a.TLSF_ALLOC_FREE(val); // Should do nothing and be ignored.
    free(val);
}

UTEST(Test_SmallTlsfAllocator, TestAllocationWhenNoAppropriateFragmentExists)
{
    SmallTlsfAllocator a(128);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 128);
    ASSERT_EQ(128, a.BytesRemaining());

    void* p0 = a.Allocate(20);
    void* p1 = a.Allocate(36);
    void* p2 = a.Allocate(20);
    void* p3 = a.Allocate(36);
    ASSERT_EQ(0, a.TotalBytesRemaining());
    ASSERT_TRUE(a.IsFull());

    a.TLSF_ALLOC_FREE(p0);
    a.TLSF_ALLOC_FREE(p2);

    ASSERT_EQ(48, a.TotalBytesRemaining());
    void* tooLargeVal = a.Allocate(24);
    ASSERT_FALSE(tooLargeVal);
}

UTEST(Test_SmallTlsfAllocator, TestRandomAllocationsAndDeallocations)
{
    SmallTlsfAllocator a(8000);

    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    std::uniform_int_distribution<int> actionDist(0, 10);
    std::uniform_int_distribution<int> sizeDist(1, 256);

    std::vector<void*> pointers;
    pointers.reserve(10000);

    for (int i = 0; i < 10000; i++)
    {
        int action = actionDist(generator);

        if (action < 8)
        {
            size_t randomSize = sizeDist(generator);

            void* ptr = a.Allocate(randomSize);
            if (ptr)
            {
                *((uint32_t*) ptr) = 0xDEADC0DE;
                pointers.push_back(ptr);
                SmallTlsfAllocator::BlockHeader* header = a.GetHeader((uint8_t*) ptr);
            }
        }
        else if (action <= 10 && !pointers.empty())
        {
            std::uniform_int_distribution<int> indices(0, pointers.size() - 1);
            size_t index = indices(generator);

            void*& ptrToFree = pointers[index];
            ASSERT_EQ(0xDEADC0DE, *(uint32_t*) ptrToFree);
            SmallTlsfAllocator::BlockHeader* header = a.GetHeader((uint8_t*) ptrToFree);

            a.TLSF_ALLOC_FREE(ptrToFree);
            SmallTlsfAllocator::FreeBlockNode* newNode = a.GetFreeBlock(header);

            ASSERT_FALSE(ptrToFree);
            pointers.erase(pointers.begin() + index);
        }
    }

    for (void* ptr : pointers)
    {
        a.TLSF_ALLOC_FREE(ptr);
    }

    ASSERT_EQ(a.TotalSize(), a.TotalBytesRemaining());
}

// Large Allocator Tests

UTEST(Test_LargeTlsfAllocator, EmptyConstructor)
{
    LargeTlsfAllocator a;
    ASSERT_EQ(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 0);
    ASSERT_EQ(0, a.BytesRemaining());
}

UTEST(Test_LargeTlsfAllocator, ConstructorWithSize)
{
    LargeTlsfAllocator a(256);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.TotalSize(), 272);
    ASSERT_EQ(256, a.BytesRemaining());

    ASSERT_EQ(16, a.FirstLevelBitmask());
    ASSERT_EQ(2, a.SecondLevelBitmask(4));

    LargeTlsfAllocator::FreeBlockNode* block = a.GetFreeBlock(4, 1);
    ASSERT_TRUE(block);
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);

    LargeTlsfAllocator::BlockHeader* header = a.GetHeader(block);
    ASSERT_TRUE(header);
    ASSERT_EQ(272, a.GetHeaderSize(header));
    ASSERT_TRUE(a.IsFree(header));
    ASSERT_FALSE(a.PrevBlockIsFree(header));

    LargeTlsfAllocator::BlockFooter* footer = a.GetFooter(header);
    ASSERT_TRUE(footer);
    ASSERT_EQ(272, footer->totalBlockSize);
}

UTEST(Test_LargeTlsfAllocator, ConstructorWithTooSmallSize)
{
    Siege::LargeTlsfAllocator a(15);
    ASSERT_EQ(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 0);
}

UTEST(Test_LargeTlsfAllocator, TestAllocateFunction)
{
    LargeTlsfAllocator a(256);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 256);
    ASSERT_EQ(256, a.BytesRemaining());

    ASSERT_EQ(16, a.FirstLevelBitmask());
    ASSERT_EQ(2, a.SecondLevelBitmask(4));

    TestStruct* p = (TestStruct*) a.Allocate(sizeof(TestStruct));

    p->inta = 10;
    p->intb = 20;

    ASSERT_EQ(10, p->inta);
    ASSERT_EQ(20, p->intb);

    ASSERT_EQ(8, a.FirstLevelBitmask());
    ASSERT_EQ(16384, a.SecondLevelBitmask(3));

    LargeTlsfAllocator::FreeBlockNode* block = a.GetFreeBlock(3, 14);
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);

    auto header = (LargeTlsfAllocator::BlockHeader*) a.Data();
    auto data = (TestStruct*) a.GetBlockData(header);
    auto footer = a.GetFooter(header);

    ASSERT_EQ(256, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(LargeTlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(LargeTlsfAllocator::BlockFooter),
              footer->totalBlockSize);
    ASSERT_EQ(a.BytesRemaining(), 240);

    Siege::String* str = (Siege::String*) a.Allocate(sizeof(Siege::String));

    *str = "Hello There!";
    ASSERT_STREQ(str->Str(), "Hello There!");

    ASSERT_EQ(8, a.FirstLevelBitmask());
    ASSERT_EQ(1024, a.SecondLevelBitmask(3));

    auto strHeader = a.GetNextHeader(header);
    auto strData = (String*) a.GetBlockData(strHeader);
    auto strFooter = a.GetFooter(strHeader);

    ASSERT_EQ(256, strHeader->sizeAndFlags);
    ASSERT_EQ(strData, str);
    ASSERT_EQ(sizeof(LargeTlsfAllocator::BlockHeader) + sizeof(String) +
                  sizeof(LargeTlsfAllocator::BlockFooter),
              strFooter->totalBlockSize);
    ASSERT_EQ(224, a.BytesRemaining());

    // Edge cases

    // Empty allocator
    LargeTlsfAllocator emptyA;

    TestStruct* ptr = (TestStruct*) emptyA.Allocate(sizeof(TestStruct));
    ASSERT_FALSE(ptr);

    // Allocate 0 bytes
    void* emptyAllocPtr = a.Allocate(0);
    ASSERT_FALSE(emptyAllocPtr);

    // Allocate an amount that causes a value overflow
    LargeTlsfAllocator overflowAlloc(UINT64_MAX);
    void* overflowPtr = overflowAlloc.Allocate(UINT64_MAX);
    ASSERT_FALSE(overflowPtr);

    LargeTlsfAllocator tooSmallAlloc(256);
    void* tooLargeAlloc = tooSmallAlloc.Allocate(1024);
    ASSERT_FALSE(tooLargeAlloc);
}

UTEST(Test_LargeTlsfAllocator, TestAllocateFunctionWithRandomInputs)
{
    LargeTlsfAllocator a(4096 * 4096);

    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    std::uniform_int_distribution<int> sizeDist(1, 2048);

    LargeTlsfAllocator::BlockHeader* header = (LargeTlsfAllocator::BlockHeader*) a.Data();

    for (int i = 0; i < 10000; i++)
    {
        size_t randomSize = sizeDist(generator);
        void* ptr = a.Allocate(randomSize);

        if (ptr)
        {
            *((uint32_t*) ptr) = 0xDEADC0DE;

            uint64_t expectedSize = header->sizeAndFlags >> 3;
            uint32_t* data = (uint32_t*) a.GetBlockData(header);
            ASSERT_EQ(0xDEADC0DE, *data);
            LargeTlsfAllocator::BlockFooter* footer = a.GetFooter(header);
            header = a.GetNextHeader(header);
        }
        else continue;
    }
}

UTEST(Test_LargeTlsfAllocator, TestDeallocateFunction)
{
    LargeTlsfAllocator a(256);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 256);
    ASSERT_EQ(256, a.BytesRemaining());

    ASSERT_EQ(16, a.FirstLevelBitmask());
    ASSERT_EQ(2, a.SecondLevelBitmask(4));

    TestStruct* p = (TestStruct*) a.Allocate(sizeof(TestStruct));

    p->inta = 10;
    p->intb = 20;

    ASSERT_EQ(10, p->inta);
    ASSERT_EQ(20, p->intb);

    ASSERT_EQ(8, a.FirstLevelBitmask());
    ASSERT_EQ(16384, a.SecondLevelBitmask(3));

    LargeTlsfAllocator::FreeBlockNode* block = a.GetFreeBlock(3, 14);
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);

    auto header = (LargeTlsfAllocator::BlockHeader*) a.Data();
    auto data = (TestStruct*) a.GetBlockData(header);
    auto footer = a.GetFooter(header);

    ASSERT_EQ(256, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(LargeTlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(LargeTlsfAllocator::BlockFooter),
              footer->totalBlockSize);
    ASSERT_EQ(240, a.BytesRemaining());

    a.TLSF_ALLOC_FREE(p);

    // Should be empty
    ASSERT_EQ(2177, header->sizeAndFlags);
    ASSERT_TRUE(header->sizeAndFlags & LargeTlsfAllocator::FREE);
    ASSERT_FALSE(p);
    ASSERT_EQ(16, a.FirstLevelBitmask());
    ASSERT_EQ(2, a.SecondLevelBitmask(4));
    ASSERT_EQ(256, a.BytesRemaining());

    p = (TestStruct*) a.Allocate(sizeof(TestStruct));
    p->inta = 10;
    p->intb = 20;

    ASSERT_EQ(10, p->inta);
    ASSERT_EQ(20, p->intb);

    ASSERT_EQ(8, a.FirstLevelBitmask());
    ASSERT_EQ(16384, a.SecondLevelBitmask(3));

    block = a.GetFreeBlock(3, 14);
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);

    header = (LargeTlsfAllocator::BlockHeader*) a.Data();
    data = (TestStruct*) a.GetBlockData(header);
    footer = a.GetFooter(header);

    ASSERT_EQ(256, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(LargeTlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(LargeTlsfAllocator::BlockFooter),
              footer->totalBlockSize);
    ASSERT_EQ(240, a.BytesRemaining());

    Siege::String* str = (Siege::String*) a.Allocate(sizeof(Siege::String));

    *str = "Hello There!";
    ASSERT_STREQ(str->Str(), "Hello There!");

    ASSERT_EQ(8, a.FirstLevelBitmask());
    ASSERT_EQ(1024, a.SecondLevelBitmask(3));

    auto strHeader = a.GetNextHeader(header);
    auto strData = (String*) a.GetBlockData(strHeader);
    auto strFooter = a.GetFooter(strHeader);

    ASSERT_EQ(256, strHeader->sizeAndFlags);
    ASSERT_EQ(strData, str);
    ASSERT_EQ(sizeof(LargeTlsfAllocator::BlockHeader) + sizeof(String) +
                  sizeof(LargeTlsfAllocator::BlockFooter),
              strFooter->totalBlockSize);
    ASSERT_EQ(224, a.BytesRemaining());

    a.TLSF_ALLOC_FREE(p);
    ASSERT_FALSE(p);
    ASSERT_TRUE(strHeader->sizeAndFlags & LargeTlsfAllocator::PREV_IS_FREE);
    ASSERT_EQ(257, header->sizeAndFlags);
    ASSERT_TRUE(header->sizeAndFlags & LargeTlsfAllocator::FREE);
    LargeTlsfAllocator::FreeBlockNode* newFreeBlock = a.GetFreeBlock(header);
    ASSERT_EQ(nullptr, newFreeBlock->next);
    ASSERT_EQ(nullptr, newFreeBlock->prev);
    ASSERT_EQ(240, a.BytesRemaining());

    a.TLSF_ALLOC_FREE(str);
    ASSERT_FALSE(str);
    ASSERT_EQ(2177, header->sizeAndFlags);
    ASSERT_EQ(16, a.FirstLevelBitmask());
    ASSERT_EQ(2, a.SecondLevelBitmask(4));
    ASSERT_EQ(256, a.BytesRemaining());

    LargeTlsfAllocator::FreeBlockNode* newNode = (LargeTlsfAllocator::FreeBlockNode*) data;
    ASSERT_TRUE(newNode);
    // Check if the new node is head
    ASSERT_EQ(nullptr, newNode->prev);
    ASSERT_EQ(nullptr, newNode->next);
}

UTEST(Test_LargeTlsfAllocator, TestBlockCoalescing)
{
    LargeTlsfAllocator a(256);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 256);
    ASSERT_EQ(256, a.BytesRemaining());

    ASSERT_EQ(16, a.FirstLevelBitmask());
    ASSERT_EQ(2, a.SecondLevelBitmask(4));

    TestStruct* p = (TestStruct*) a.Allocate(sizeof(TestStruct));

    p->inta = 10;
    p->intb = 20;

    ASSERT_EQ(10, p->inta);
    ASSERT_EQ(20, p->intb);

    ASSERT_EQ(8, a.FirstLevelBitmask());
    ASSERT_EQ(16384, a.SecondLevelBitmask(3));

    LargeTlsfAllocator::FreeBlockNode* block = a.GetFreeBlock(3, 14);
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);
    ASSERT_EQ(240, a.BytesRemaining());

    auto header = (LargeTlsfAllocator::BlockHeader*) a.Data();
    auto data = (TestStruct*) a.GetBlockData(header);
    auto footer = a.GetFooter(header);

    ASSERT_EQ(256, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(LargeTlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(LargeTlsfAllocator::BlockFooter),
              footer->totalBlockSize);

    Siege::String* str = (Siege::String*) a.Allocate(sizeof(Siege::String));

    *str = "Hello There!";
    ASSERT_STREQ(str->Str(), "Hello There!");

    ASSERT_EQ(8, a.FirstLevelBitmask());
    ASSERT_EQ(1024, a.SecondLevelBitmask(3));

    LargeTlsfAllocator::FreeBlockNode* NewBlock = a.GetFreeBlock(3, 10);
    ASSERT_EQ(NewBlock->next, nullptr);
    ASSERT_EQ(NewBlock->prev, nullptr);
    ASSERT_EQ(224, a.BytesRemaining());

    auto strHeader = a.GetNextHeader(header);
    auto strData = (String*) a.GetBlockData(strHeader);
    auto strFooter = a.GetFooter(strHeader);

    ASSERT_EQ(256, strHeader->sizeAndFlags);
    ASSERT_EQ(strData, str);
    ASSERT_EQ(sizeof(LargeTlsfAllocator::BlockHeader) + sizeof(Siege::String) +
                  sizeof(LargeTlsfAllocator::BlockFooter),
              strFooter->totalBlockSize);

    TestStruct* p2 = (TestStruct*) a.Allocate(sizeof(TestStruct));

    p2->inta = 10;
    p2->intb = 20;

    ASSERT_EQ(10, p2->inta);
    ASSERT_EQ(20, p2->intb);

    ASSERT_EQ(8, a.FirstLevelBitmask());
    ASSERT_EQ(64, a.SecondLevelBitmask(3));

    LargeTlsfAllocator::FreeBlockNode* newFreeBlock = a.GetFreeBlock(3, 6);
    ASSERT_EQ(newFreeBlock->next, nullptr);
    ASSERT_EQ(newFreeBlock->prev, nullptr);
    ASSERT_EQ(208, a.BytesRemaining());

    auto newHeader = a.GetNextHeader(strHeader);
    auto newData = (String*) a.GetBlockData(newHeader);
    auto newFooter = a.GetFooter(newHeader);

    ASSERT_EQ(256, newHeader->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(LargeTlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(LargeTlsfAllocator::BlockFooter),
              footer->totalBlockSize);

    a.TLSF_ALLOC_FREE(p);

    LargeTlsfAllocator::FreeBlockNode* firstFree = a.GetFreeBlock(3, 6);
    ASSERT_EQ(firstFree->next, nullptr);
    ASSERT_EQ(firstFree->prev, nullptr);
    ASSERT_EQ(224, a.BytesRemaining());

    LargeTlsfAllocator::BlockHeader* firstFreeHeader = a.GetHeader((uint8_t*) firstFree);
    ASSERT_TRUE(firstFreeHeader->sizeAndFlags & LargeTlsfAllocator::FREE);
    ASSERT_FALSE(firstFreeHeader->sizeAndFlags & LargeTlsfAllocator::PREV_IS_FREE);

    ASSERT_FALSE(p);
    ASSERT_EQ(a.FirstLevelBitmask(), 10);
    ASSERT_EQ(1, a.SecondLevelBitmask(1));

    a.TLSF_ALLOC_FREE(p2);
    ASSERT_FALSE(p2);
    ASSERT_EQ(240, a.BytesRemaining());
    ASSERT_EQ(a.FirstLevelBitmask(), 10);
    ASSERT_EQ(1024, a.SecondLevelBitmask(3));
    ASSERT_EQ(1, a.SecondLevelBitmask(1));

    a.TLSF_ALLOC_FREE(str);
    ASSERT_FALSE(str);
    ASSERT_EQ(256, a.BytesRemaining());
    ASSERT_EQ(16, a.FirstLevelBitmask());
    ASSERT_EQ(2, a.SecondLevelBitmask(4));

    // Edge Cases

    void* badPointer = nullptr;
    a.TLSF_ALLOC_FREE(badPointer);
    ASSERT_FALSE(badPointer);

    // try to deallocate pointer not in allocator;

    uint64_t* val = new uint64_t;
    a.TLSF_ALLOC_FREE(val); // Should do nothing and be ignored.
    free(val);
}

UTEST(Test_LargeTlsfAllocator, TestAllocationWhenNoAppropriateFragmentExists)
{
    LargeTlsfAllocator a(256);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 256);
    ASSERT_EQ(256, a.BytesRemaining());

    void* p0 = a.Allocate(32);
    void* p1 = a.Allocate(64);
    void* p2 = a.Allocate(32);
    void* p3 = a.Allocate(64);
    ASSERT_EQ(0, a.TotalBytesRemaining());
    ASSERT_TRUE(a.IsFull());

    a.TLSF_ALLOC_FREE(p0);
    a.TLSF_ALLOC_FREE(p2);

    ASSERT_EQ(128, a.BytesRemaining());
    void* tooLargeVal = a.Allocate(34);
    ASSERT_FALSE(tooLargeVal);
}

UTEST(Test_LargeTlsfAllocator, TestRandomAllocationsAndDeallocations)
{
    LargeTlsfAllocator a(4096 * 4096);

    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    std::uniform_int_distribution<int> actionDist(0, 10);
    std::uniform_int_distribution<int> sizeDist(1, 4096);

    std::vector<void*> pointers;
    pointers.reserve(10000);

    for (int i = 0; i < 10000; i++)
    {
        int action = actionDist(generator);

        if (action < 8)
        {
            size_t randomSize = sizeDist(generator);

            void* ptr = a.Allocate(randomSize);
            if (ptr)
            {
                *((uint32_t*) ptr) = 0xDEADC0DE;
                pointers.push_back(ptr);
                LargeTlsfAllocator::BlockHeader* header = a.GetHeader((uint8_t*) ptr);
            }
        }
        else if (action <= 10 && !pointers.empty())
        {
            std::uniform_int_distribution<int> indices(0, pointers.size() - 1);
            size_t index = indices(generator);

            void*& ptrToFree = pointers[index];
            ASSERT_EQ(0xDEADC0DE, *(uint32_t*) ptrToFree);
            LargeTlsfAllocator::BlockHeader* header = a.GetHeader((uint8_t*) ptrToFree);

            a.TLSF_ALLOC_FREE(ptrToFree);
            LargeTlsfAllocator::FreeBlockNode* newNode = a.GetFreeBlock(header);

            ASSERT_FALSE(ptrToFree);
            pointers.erase(pointers.begin() + index);
        }
    }

    for (void* ptr : pointers)
    {
        a.TLSF_ALLOC_FREE(ptr);
    }

    ASSERT_EQ(a.TotalSize(), a.TotalBytesRemaining());
}