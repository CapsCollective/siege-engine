//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <utest.h>
#include <utils/BinarySerialisation.h>

#include <map>

struct TestBinarySerialisable1 : public Siege::BinarySerialisation::BinarySerialisable
{
    bool b = false;
    int i = 0;
    float f = 0;
    double d = 0;

    void serialise(Siege::BinarySerialisation::Buffer& buffer, bool deserialise) override
    {
        Siege::BinarySerialisation::serialise(buffer, b, deserialise);
        Siege::BinarySerialisation::serialise(buffer, i, deserialise);
        Siege::BinarySerialisation::serialise(buffer, f, deserialise);
        Siege::BinarySerialisation::serialise(buffer, d, deserialise);
    }
};

struct TestBinarySerialisable2 : public Siege::BinarySerialisation::BinarySerialisable
{
    TestBinarySerialisable1 test;

    void serialise(Siege::BinarySerialisation::Buffer& buffer, bool deserialise) override
    {
        Siege::BinarySerialisation::serialise(buffer, test, deserialise);
    }
};

UTEST(test_BinarySerialisation, Native)
{
    Siege::BinarySerialisation::Buffer buffer;
    {
        bool b = true;
        Siege::BinarySerialisation::serialise(buffer, b);
    }
    bool b;
    Siege::BinarySerialisation::serialise(buffer, b, true);
    ASSERT_EQ(true, b);
    buffer.Reset();

    {
        int32_t i = 36;
        Siege::BinarySerialisation::serialise(buffer, i);
    }
    int32_t i;
    Siege::BinarySerialisation::serialise(buffer, i, true);
    ASSERT_EQ(36, i);
    buffer.Reset();

    {
        float f = 3.14f;
        Siege::BinarySerialisation::serialise(buffer, f);
    }
    float f;
    Siege::BinarySerialisation::serialise(buffer, f, true);
    ASSERT_EQ(3.14f, f);
    buffer.Reset();

    {
        double d = 1.21;
        Siege::BinarySerialisation::serialise(buffer, d);
    }
    double d;
    Siege::BinarySerialisation::serialise(buffer, d, true);
    ASSERT_EQ(1.21, d);
    buffer.Reset();
}

UTEST(test_BinarySerialisation, Struct)
{
    Siege::BinarySerialisation::Buffer buffer;
    {
        TestBinarySerialisable1 test1;
        test1.b = true;
        test1.i = 36;
        test1.f = 3.14f;
        test1.d = 1.21;
        Siege::BinarySerialisation::serialise(buffer, test1);
    }
    TestBinarySerialisable1 test1;
    Siege::BinarySerialisation::serialise(buffer, test1, true);
    ASSERT_EQ(true, test1.b);
    ASSERT_EQ(36, test1.i);
    ASSERT_EQ(3.14f, test1.f);
    ASSERT_EQ(1.21, test1.d);
    buffer.Reset();

    {
        TestBinarySerialisable2 test2;
        test2.test.b = true;
        test2.test.i = 36;
        test2.test.f = 3.14f;
        test2.test.d = 1.21;
        Siege::BinarySerialisation::serialise(buffer, test1);
    }
    TestBinarySerialisable2 test2;
    Siege::BinarySerialisation::serialise(buffer, test2, true);
    ASSERT_EQ(true, test2.test.b);
    ASSERT_EQ(36, test2.test.i);
    ASSERT_EQ(3.14f, test2.test.f);
    ASSERT_EQ(1.21, test2.test.d);
    buffer.Reset();
}

UTEST(test_BinarySerialisation, Vector)
{
    Siege::BinarySerialisation::Buffer buffer;
    {
        std::vector<int32_t> vec1 = {2, 5, 41};
        Siege::BinarySerialisation::serialise(buffer, vec1);
    }
    std::vector<int32_t> vec1;
    Siege::BinarySerialisation::serialise(buffer, vec1, true);
    ASSERT_EQ(3, vec1.size());
    ASSERT_EQ(2, vec1[0]);
    ASSERT_EQ(5, vec1[1]);
    ASSERT_EQ(41, vec1[2]);
    buffer.Reset();

    {
        TestBinarySerialisable1 test1;
        test1.b = true;
        test1.i = 36;
        test1.f = 3.14f;
        test1.d = 1.21;
        TestBinarySerialisable1 test2;
        test2.b = false;
        test2.i = 35;
        test2.f = 3.13f;
        test2.d = 1.20;
        std::vector<TestBinarySerialisable1> vec2 = { test1, test2};
        Siege::BinarySerialisation::serialise(buffer, vec2);
    }
    std::vector<TestBinarySerialisable1> vec2;
    Siege::BinarySerialisation::serialise(buffer, vec2, true);
    ASSERT_EQ(2, vec2.size());
    ASSERT_EQ(true, vec2[0].b);
    ASSERT_EQ(36, vec2[0].i);
    ASSERT_EQ(3.14f, vec2[0].f);
    ASSERT_EQ(1.21, vec2[0].d);
    ASSERT_EQ(false, vec2[1].b);
    ASSERT_EQ(35, vec2[1].i);
    ASSERT_EQ(3.13f, vec2[1].f);
    ASSERT_EQ(1.20, vec2[1].d);
    buffer.Reset();
}

UTEST(test_BinarySerialisation, Set)
{
    Siege::BinarySerialisation::Buffer buffer;
    {
        std::set<int32_t> set1 = {2, 5, 41};
        Siege::BinarySerialisation::serialise(buffer, set1);
    }
    std::set<int32_t> set1;
    Siege::BinarySerialisation::serialise(buffer, set1, true);
    ASSERT_EQ(3, set1.size());
    auto it1 = set1.find(2);
    ASSERT_TRUE(it1 != set1.end());
    it1 = set1.find(5);
    ASSERT_TRUE(it1 != set1.end());
    it1 = set1.find(41);
    ASSERT_TRUE(it1 != set1.end());
    buffer.Reset();
}

UTEST(test_BinarySerialisation, Map)
{
    Siege::BinarySerialisation::Buffer buffer;
    {
        std::map<int32_t, bool> map1 = {
            {2, true},
            {5, false},
            {41, true},
        };
        Siege::BinarySerialisation::serialise(buffer, map1);
    }
    std::map<int32_t, bool> map1;
    Siege::BinarySerialisation::serialise(buffer, map1, true);
    ASSERT_EQ(3, map1.size());
    auto it1 = map1.find(2);
    ASSERT_TRUE(it1 != map1.end());
    ASSERT_EQ(true, it1->second);
    it1 = map1.find(5);
    ASSERT_TRUE(it1 != map1.end());
    ASSERT_EQ(false, it1->second);
    it1 = map1.find(41);
    ASSERT_TRUE(it1 != map1.end());
    ASSERT_EQ(true, it1->second);
    buffer.Reset();

    {
        TestBinarySerialisable1 test1;
        test1.b = true;
        test1.i = 36;
        test1.f = 3.14f;
        test1.d = 1.21;
        TestBinarySerialisable1 test2;
        test2.b = false;
        test2.i = 35;
        test2.f = 3.13f;
        test2.d = 1.20;
        std::map<int32_t, TestBinarySerialisable1> map2 = {
            {2, test1},
            {5, test2},
        };
        Siege::BinarySerialisation::serialise(buffer, map2);
    }
    std::map<int32_t, TestBinarySerialisable1> map2;
    Siege::BinarySerialisation::serialise(buffer, map2, true);
    ASSERT_EQ(2, map2.size());
    auto it2 = map2.find(2);
    ASSERT_TRUE(it2 != map2.end());
    ASSERT_EQ(true, it2->second.b);
    ASSERT_EQ(36, it2->second.i);
    ASSERT_EQ(3.14f, it2->second.f);
    ASSERT_EQ(1.21, it2->second.d);
    it2 = map2.find(5);
    ASSERT_TRUE(it2 != map2.end());
    ASSERT_EQ(false, it2->second.b);
    ASSERT_EQ(35, it2->second.i);
    ASSERT_EQ(3.13f, it2->second.f);
    ASSERT_EQ(1.20, it2->second.d);
    buffer.Reset();
}
