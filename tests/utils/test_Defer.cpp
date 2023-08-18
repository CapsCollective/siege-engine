//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <utest.h>
#include <utils/Defer.h>

UTEST(test_Defer, BasicScopeDeferModify)
{
    bool a = true;
    {
        ASSERT_TRUE(a);
        defer([&a] { a = false; });
        a = true;
        ASSERT_TRUE(a);
    }
    ASSERT_FALSE(a);
}

UTEST(test_Defer, BasicScopeDeferDeletion)
{
    auto* a = new bool;
    {
        ASSERT_TRUE(a);
        defer([&a] {
            delete a;
            a = nullptr;
        });
        ASSERT_TRUE(a);
    }
    ASSERT_FALSE(a);
}

UTEST(test_Defer, LoopScopeDeferModify)
{
    bool a = true;
    for (uint8_t i = 0; i < 3; ++i)
    {
        ASSERT_TRUE(a);
        defer([&a] { a = true; });
        a = false;
        ASSERT_FALSE(a);
    }
    ASSERT_TRUE(a);
}
