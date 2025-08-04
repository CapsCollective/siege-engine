//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include <utest.h>
#include <utils/Token.h>

REGISTER_TOKEN(HelloWorld);
REGISTER_TOKEN(NewOne);
REGISTER_TOKEN(GoodbyeWorld);
REGISTER_TOKEN(ReallyLongHeapAllocatedStringTokenName);

UTEST(test_Token, StaticallyRegistered)
{
    Siege::Token tk1 = TOKEN_HelloWorld;
    Siege::Token tk2 = TOKEN_NewOne;
    Siege::Token tk3 = TOKEN_GoodbyeWorld;
    Siege::Token tk4 = TOKEN_HelloWorld;
    Siege::Token tk5("Hi");
    Siege::Token tk6("HelloWorld");
    Siege::Token tk7 = TOKEN_ReallyLongHeapAllocatedStringTokenName;

    ASSERT_TRUE(tk1.IsValid());
    ASSERT_TRUE(tk2.IsValid());
    ASSERT_TRUE(tk3.IsValid());
    ASSERT_TRUE(tk4.IsValid());
    ASSERT_FALSE(tk5.IsValid());
    ASSERT_TRUE(tk6.IsValid());
    ASSERT_TRUE(tk7.IsValid());

    ASSERT_STREQ("HelloWorld", tk1.GetId());
    ASSERT_STREQ("NewOne", tk2.GetId());
    ASSERT_STREQ("GoodbyeWorld", tk3.GetId());
    ASSERT_STREQ("HelloWorld", tk4.GetId());
    ASSERT_STREQ("HelloWorld", tk6.GetId());
    ASSERT_STREQ("ReallyLongHeapAllocatedStringTokenName", tk7.GetId());

    ASSERT_TRUE(tk1 != tk2);
    ASSERT_TRUE(tk1 != tk3);
    ASSERT_TRUE(tk1 == tk4);
    ASSERT_TRUE(tk1 == tk6);
    ASSERT_TRUE(tk1 != tk7);
    ASSERT_TRUE(tk2 != tk3);
    ASSERT_TRUE(tk2 != tk4);
    ASSERT_TRUE(tk2 != tk7);
    ASSERT_TRUE(tk3 != tk4);
    ASSERT_TRUE(tk3 != tk7);
    ASSERT_TRUE(tk6 != tk7);
}

UTEST(test_Token, DynamicallyRegistered)
{
    Siege::Token tk1 = Siege::Token::FindToken("HelloWorld");
    Siege::Token tk2 = Siege::Token::FindToken("NewOne");
    Siege::Token tk3 = Siege::Token::FindToken("GoodbyeWorld");
    Siege::Token tk4 = Siege::Token::FindOrRegisterToken("HelloWorld");

    ASSERT_TRUE(tk1.IsValid());
    ASSERT_TRUE(tk2.IsValid());
    ASSERT_TRUE(tk3.IsValid());
    ASSERT_TRUE(tk4.IsValid());

    ASSERT_STREQ("HelloWorld", tk1.GetId());
    ASSERT_STREQ("NewOne", tk2.GetId());
    ASSERT_STREQ("GoodbyeWorld", tk3.GetId());
    ASSERT_STREQ("HelloWorld", tk4.GetId());

    ASSERT_TRUE(tk1 != tk2);
    ASSERT_TRUE(tk1 != tk3);
    ASSERT_TRUE(tk1 == tk4);
    ASSERT_TRUE(tk2 != tk3);
    ASSERT_TRUE(tk2 != tk4);
    ASSERT_TRUE(tk3 != tk4);

    Siege::Token tk5 = Siege::Token::FindOrRegisterToken("a");
    Siege::Token tk6 = Siege::Token::FindOrRegisterToken("c");
    Siege::Token tk7 = Siege::Token::FindOrRegisterToken("b");

    ASSERT_STREQ("a", tk5.GetId());
    ASSERT_STREQ("c", tk6.GetId());
    ASSERT_STREQ("b", tk7.GetId());

    Siege::Token tk8 = Siege::Token::FindToken("GoodbyeWorld");
    Siege::Token tk9 = Siege::Token::FindToken("FooBar");

    ASSERT_TRUE(tk8.IsValid());
    ASSERT_FALSE(tk9.IsValid());

    ASSERT_STREQ("GoodbyeWorld", tk8.GetId());

    ASSERT_TRUE(tk8 == tk3);
}
