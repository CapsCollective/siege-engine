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
#include <utils/TlsfAllocator.h>

#include <chrono>
#include <random>

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
    ASSERT_EQ(a.TotalSize(), 72);
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

    auto header = (TlsfAllocator::BlockHeader*) a.Data();
    auto data = (TestStruct*) a.GetBlockData(header);
    auto footer = a.GetFooter(header);

    ASSERT_EQ(192, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(TlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(TlsfAllocator::BlockFooter),
              footer->totalBlockSize);
    ASSERT_EQ(a.BytesRemaining(), 48);

    Siege::String* str = (Siege::String*) a.Allocate(sizeof(Siege::String));

    *str = "Hello There!";
    ASSERT_STREQ(str->Str(), "Hello There!");

    ASSERT_EQ(1, a.FlBitmask());
    ASSERT_EQ(0, a.SlBitmask(1));

    auto strHeader = a.GetNextHeader(header);
    auto strData = (String*) a.GetBlockData(strHeader);
    auto strFooter = a.GetFooter(strHeader);

    ASSERT_EQ(192, strHeader->sizeAndFlags);
    ASSERT_EQ(strData, str);
    ASSERT_EQ(
        sizeof(TlsfAllocator::BlockHeader) + sizeof(String) + sizeof(TlsfAllocator::BlockFooter),
        strFooter->totalBlockSize);
    ASSERT_EQ(32, a.BytesRemaining());

    // Edge cases

    // Empty allocator
    TlsfAllocator emptyA;

    TestStruct* ptr = (TestStruct*) emptyA.Allocate(sizeof(TestStruct));
    ASSERT_FALSE(ptr);

    // Allocate 0 bytes
    void* emptyAllocPtr = a.Allocate(0);
    ASSERT_FALSE(emptyAllocPtr);

    // Allocate an amount that causes a value overflow
    TlsfAllocator overflowAlloc(UINT64_MAX);
    void* overflowPtr = overflowAlloc.Allocate(UINT64_MAX);
    ASSERT_FALSE(overflowPtr);

    void* tooLargeCase = a.Allocate(100);
    ASSERT_FALSE(tooLargeCase);

    TlsfAllocator tooSmallAlloc(64);
    void* tooLargeAlloc = tooSmallAlloc.Allocate(100);
    ASSERT_FALSE(tooLargeAlloc);
}

UTEST(test_TlsfAllocator, TestAllocateFunctionWithRandomInputs)
{
    TlsfAllocator a(1024 * 1024);

    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine generator(12345);

    std::uniform_int_distribution<int> sizeDist(1, 256);

    TlsfAllocator::BlockHeader* header = (TlsfAllocator::BlockHeader*) a.Data();

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
            TlsfAllocator::BlockFooter* footer = a.GetFooter(header);
            header = a.GetNextHeader(header);
        }
        else continue;
    }
}

UTEST(test_TlsfAllocator, TestDeallocateFunction)
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

    auto header = (TlsfAllocator::BlockHeader*) a.Data();
    auto data = (TestStruct*) a.GetBlockData(header);
    auto footer = a.GetFooter(header);

    ASSERT_EQ(192, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(TlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(TlsfAllocator::BlockFooter),
              footer->totalBlockSize);
    ASSERT_EQ(48, a.BytesRemaining());

    a.Deallocate(p);

    // Should be empty
    ASSERT_EQ(577, header->sizeAndFlags);
    ASSERT_TRUE(header->sizeAndFlags & TlsfAllocator::FREE);
    ASSERT_FALSE(p);
    ASSERT_EQ(4, a.FlBitmask());
    ASSERT_EQ(4, a.SlBitmask(2));
    ASSERT_EQ(64, a.BytesRemaining());

    p = (TestStruct*) a.Allocate(sizeof(TestStruct));
    p->inta = 10;
    p->intb = 20;

    ASSERT_EQ(10, p->inta);
    ASSERT_EQ(20, p->intb);

    ASSERT_EQ(2, a.FlBitmask());
    ASSERT_EQ(256, a.SlBitmask(1));

    block = a.GetFreeBlock(1, 8);
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);

    header = (TlsfAllocator::BlockHeader*) a.Data();
    data = (TestStruct*) a.GetBlockData(header);
    footer = a.GetFooter(header);

    ASSERT_EQ(192, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(TlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(TlsfAllocator::BlockFooter),
              footer->totalBlockSize);
    ASSERT_EQ(48, a.BytesRemaining());

    Siege::String* str = (Siege::String*) a.Allocate(sizeof(Siege::String));

    *str = "Hello There!";
    ASSERT_STREQ(str->Str(), "Hello There!");

    ASSERT_EQ(1, a.FlBitmask());
    ASSERT_EQ(0, a.SlBitmask(1));

    auto strHeader = a.GetNextHeader(header);
    auto strData = (String*) a.GetBlockData(strHeader);
    auto strFooter = a.GetFooter(strHeader);

    ASSERT_EQ(192, strHeader->sizeAndFlags);
    ASSERT_EQ(strData, str);
    ASSERT_EQ(
        sizeof(TlsfAllocator::BlockHeader) + sizeof(String) + sizeof(TlsfAllocator::BlockFooter),
        strFooter->totalBlockSize);
    ASSERT_EQ(32, a.BytesRemaining());

    a.Deallocate(p);
    ASSERT_FALSE(p);
    ASSERT_TRUE(strHeader->sizeAndFlags & TlsfAllocator::PREV_IS_FREE);
    ASSERT_EQ(193, header->sizeAndFlags);
    ASSERT_TRUE(header->sizeAndFlags & TlsfAllocator::FREE);
    TlsfAllocator::FreeBlockNode* newFreeBlock = a.GetFreeBlock(header);
    ASSERT_NE(nullptr, newFreeBlock->next);
    ASSERT_EQ(nullptr, newFreeBlock->prev);
    ASSERT_EQ(48, a.BytesRemaining());

    a.Deallocate(str);
    ASSERT_FALSE(str);
    ASSERT_EQ(577, header->sizeAndFlags);
    ASSERT_EQ(4, a.FlBitmask());
    ASSERT_EQ(4, a.SlBitmask(2));
    ASSERT_EQ(64, a.BytesRemaining());

    TlsfAllocator::FreeBlockNode* newNode = (TlsfAllocator::FreeBlockNode*) data;
    ASSERT_TRUE(newNode);
    // Check if the new node is head
    ASSERT_EQ(nullptr, newNode->prev);
    ASSERT_EQ(nullptr, newNode->next);
}

UTEST(test_TlsfAllocator, TestBlockCoalescing)
{
    TlsfAllocator a(128);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 128);
    ASSERT_EQ(128, a.BytesRemaining());

    ASSERT_EQ(a.FlBitmask(), 8);
    ASSERT_EQ(2, a.SlBitmask(3));

    TestStruct* p = (TestStruct*) a.Allocate(sizeof(TestStruct));

    p->inta = 10;
    p->intb = 20;

    ASSERT_EQ(10, p->inta);
    ASSERT_EQ(20, p->intb);

    ASSERT_EQ(4, a.FlBitmask());
    ASSERT_EQ(4096, a.SlBitmask(2));
    // 0001 0000 0000 0000
    TlsfAllocator::FreeBlockNode* block = a.GetFreeBlock(2, 12);
    ASSERT_EQ(block->next, nullptr);
    ASSERT_EQ(block->prev, nullptr);
    ASSERT_EQ(112, a.BytesRemaining());

    auto header = (TlsfAllocator::BlockHeader*) a.Data();
    auto data = (TestStruct*) a.GetBlockData(header);
    auto footer = a.GetFooter(header);

    ASSERT_EQ(192, header->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(TlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(TlsfAllocator::BlockFooter),
              footer->totalBlockSize);

    Siege::String* str = (Siege::String*) a.Allocate(sizeof(Siege::String));

    *str = "Hello There!";
    ASSERT_STREQ(str->Str(), "Hello There!");

    ASSERT_EQ(4, a.FlBitmask());
    ASSERT_EQ(64, a.SlBitmask(2));

    TlsfAllocator::FreeBlockNode* NewBlock = a.GetFreeBlock(2, 6);
    ASSERT_EQ(NewBlock->next, nullptr);
    ASSERT_EQ(NewBlock->prev, nullptr);
    ASSERT_EQ(96, a.BytesRemaining());

    auto strHeader = a.GetNextHeader(header);
    auto strData = (String*) a.GetBlockData(strHeader);
    auto strFooter = a.GetFooter(strHeader);

    ASSERT_EQ(192, strHeader->sizeAndFlags);
    ASSERT_EQ(strData, str);
    ASSERT_EQ(sizeof(TlsfAllocator::BlockHeader) + sizeof(Siege::String) +
                  sizeof(TlsfAllocator::BlockFooter),
              strFooter->totalBlockSize);

    TestStruct* p2 = (TestStruct*) a.Allocate(sizeof(TestStruct));

    p2->inta = 10;
    p2->intb = 20;

    ASSERT_EQ(10, p2->inta);
    ASSERT_EQ(20, p2->intb);

    ASSERT_EQ(4, a.FlBitmask());
    ASSERT_EQ(1, a.SlBitmask(2));

    TlsfAllocator::FreeBlockNode* newFreeBlock = a.GetFreeBlock(2, 0);
    ASSERT_EQ(newFreeBlock->next, nullptr);
    ASSERT_EQ(newFreeBlock->prev, nullptr);
    ASSERT_EQ(80, a.BytesRemaining());

    auto newHeader = a.GetNextHeader(strHeader);
    auto newData = (String*) a.GetBlockData(newHeader);
    auto newFooter = a.GetFooter(newHeader);

    ASSERT_EQ(192, newHeader->sizeAndFlags);
    ASSERT_EQ(data, p);
    ASSERT_EQ(sizeof(TlsfAllocator::BlockHeader) + sizeof(TestStruct) +
                  sizeof(TlsfAllocator::BlockFooter),
              footer->totalBlockSize);

    a.Deallocate(p);

    TlsfAllocator::FreeBlockNode* firstFree = a.GetFreeBlock(0, 8);
    ASSERT_EQ(firstFree->next, nullptr);
    ASSERT_EQ(firstFree->prev, nullptr);
    ASSERT_EQ(96, a.BytesRemaining());

    TlsfAllocator::BlockHeader* firstFreeHeader = a.GetHeader((uint8_t*) firstFree);
    ASSERT_TRUE(firstFreeHeader->sizeAndFlags & TlsfAllocator::FREE);
    ASSERT_FALSE(firstFreeHeader->sizeAndFlags & TlsfAllocator::PREV_IS_FREE);

    ASSERT_FALSE(p);
    ASSERT_EQ(a.FlBitmask(), 5);
    ASSERT_EQ(256, a.SlBitmask(0));

    a.Deallocate(p2);
    ASSERT_FALSE(p2);
    ASSERT_EQ(112, a.BytesRemaining());
    ASSERT_EQ(a.FlBitmask(), 5);
    ASSERT_EQ(256, a.SlBitmask(0));
    ASSERT_EQ(64, a.SlBitmask(2));

    a.Deallocate(str);
    ASSERT_FALSE(str);
    ASSERT_EQ(128, a.BytesRemaining());
    ASSERT_EQ(8, a.FlBitmask());
    ASSERT_EQ(2, a.SlBitmask(3));

    // Edge Cases

    void* badPointer = nullptr;
    a.Deallocate(badPointer);
    ASSERT_FALSE(badPointer);

    // try to deallocate pointer not in allocator;

    uint64_t* val = new uint64_t;
    a.Deallocate(val); // Should do nothing and be ignored.
    free(val);
}

UTEST(test_TlsfAllocator, TestAllocationWhenNoAppropriateFragmentExists)
{
    TlsfAllocator a(128);
    ASSERT_NE(a.Data(), nullptr);
    ASSERT_EQ(a.Capacity(), 128);
    ASSERT_EQ(128, a.BytesRemaining());

    void* p0 = a.Allocate(16);
    void* p1 = a.Allocate(32);
    void* p2 = a.Allocate(16);
    void* p3 = a.Allocate(32);
    ASSERT_EQ(0, a.TotalBytesRemaining());

    a.Deallocate(p0);
    a.Deallocate(p2);

    ASSERT_EQ(64, a.BytesRemaining());
    void* tooLargeVal = a.Allocate(24);
    ASSERT_FALSE(tooLargeVal);
}

UTEST(test_ResourceSystem, TestRandomAllocationsAndDeallocations)
{
    TlsfAllocator a(1024 * 1024);

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
                TlsfAllocator::BlockHeader* header = a.GetHeader((uint8_t*) ptr);
            }
        }
        else if (action <= 10 && !pointers.empty())
        {
            std::uniform_int_distribution<int> indices(0, pointers.size() - 1);
            size_t index = indices(generator);

            void*& ptrToFree = pointers[index];
            ASSERT_EQ(0xDEADC0DE, *(uint32_t*) ptrToFree);
            TlsfAllocator::BlockHeader* header = a.GetHeader((uint8_t*) ptrToFree);

            a.Deallocate(ptrToFree);
            TlsfAllocator::FreeBlockNode* newNode = a.GetFreeBlock(header);

            ASSERT_FALSE(ptrToFree);
            pointers.erase(pointers.begin() + index);
        }
    }

    for (void* ptr : pointers)
    {
        a.Deallocate(ptr);
    }

    ASSERT_EQ(a.TotalSize(), a.TotalBytesRemaining());
}