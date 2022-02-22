#include <utest.h>
#include <utils/String.h>

UTEST(test_String, DefaultConstruct)
{
    // The string should default construct correctly
    String s;
    ASSERT_FALSE(s);
    ASSERT_TRUE(s.IsEmpty());
    ASSERT_EQ(0, s.Size());
    ASSERT_TRUE(s == "");
    ASSERT_TRUE("" == s);
    ASSERT_EQ(0, std::strcmp(s.Str(), ""));

    // Assignment via assignment operator should apply
    String h("hello");
    s = h;
    ASSERT_FALSE(s.IsEmpty());
    ASSERT_EQ(5, s.Size());
    ASSERT_STREQ(h.Str(), s.Str());
    ASSERT_EQ(0, std::strcmp(s.Str(), h.Str()));

    // Assignment of c-strings via assignment operator should apply
    s = "hello";
    ASSERT_FALSE(s.IsEmpty());
    ASSERT_EQ(5, s.Size());
    ASSERT_STREQ("hello", s.Str());
    ASSERT_EQ(0, std::strcmp(s.Str(), "hello"));
}

UTEST(test_String, CopyConstruct)
{
    // The string should copy construct correctly
    String s1("hello");
    String s2(s1);
    String s3;
    String s4(s3);
    ASSERT_TRUE(s2);
    ASSERT_STREQ("hello", s2.Str());
    ASSERT_FALSE(s4);

    // Assignment via assignment operator should apply
    s2 = "goodbye";
    ASSERT_TRUE(s2);
    ASSERT_STREQ("goodbye", s2.Str());
    s3 = s4;
    ASSERT_FALSE(s3);

    // Copy construction from c-strings should apply
    String s("hello");
    ASSERT_TRUE(s);
    ASSERT_STREQ("hello", s.Str());
    s = "goodbye";
    ASSERT_TRUE(s);
    ASSERT_STREQ("goodbye", s.Str());

    // Copy construction from characters should apply
    String s5('h');
    ASSERT_TRUE(s5);
    ASSERT_STREQ("h", s5.Str());
    s5 = 'i';
    ASSERT_TRUE(s5);
    ASSERT_STREQ("i", s5.Str());
}

UTEST(test_String, MoveConstruct)
{
    // The string should move construct correctly
    String s1(String("hello"));
    String s2((String()));
    ASSERT_TRUE(s1);
    ASSERT_STREQ("hello", s1.Str());
    ASSERT_FALSE(s2);

    // Assignment via move assignment should apply
    s1 = String("goodbye");
    ASSERT_TRUE(s1);
    ASSERT_STREQ("goodbye", s1.Str());
    s1 = String();
    ASSERT_FALSE(s1);
}

UTEST(test_String, Clear)
{
    // The string can be cleared
    String s1("hello");
    ASSERT_TRUE(s1);
    ASSERT_EQ(5, s1.Size());
    s1.Clear();
    ASSERT_FALSE(s1);
    ASSERT_EQ(0, s1.Size());
}

UTEST(test_String, Add)
{
    // The addition operator creates prepended strings with c-strings
    String s1;
    String s2(s1 + "hello");
    ASSERT_STREQ("hello", s2.Str());
    s2 = s2 + "goodbye";
    ASSERT_STREQ("hellogoodbye", s2.Str());
    s2 = "goodbye" + s2;
    ASSERT_STREQ("goodbyehellogoodbye", s2.Str());
    ASSERT_EQ(19, s2.Size());

    // The addition operator creates prepended strings with characters
    String s3;
    String s4(s3 + 'h');
    ASSERT_STREQ("h", s4.Str());
    s4 = s4 + 'i';
    ASSERT_STREQ("hi", s4.Str());
    s4 = 't' + s4 + 's';
    ASSERT_STREQ("this", s4.Str());
    ASSERT_EQ(4, s4.Size());

    // The addition operator creates prepended strings with other strings
    String h("hello");
    String g("goodbye");
    String s5 = String() + h;
    ASSERT_STREQ("hello", s5.Str());
    s5 = s5 + g;
    ASSERT_STREQ("hellogoodbye", s5.Str());
    s5 = g + s5;
    ASSERT_STREQ("goodbyehellogoodbye", s5.Str());
    ASSERT_EQ(19, s5.Size());
}

UTEST(test_String, Append)
{
    // The string should correctly append c-strings
    String s1;
    s1 += "hello";
    ASSERT_STREQ("hello", s1.Str());
    String s2 = "goodbye" + s1;
    ASSERT_STREQ("goodbyehello", s2.Str());
    s2.Append("goodbye");
    ASSERT_STREQ("goodbyehellogoodbye", s2.Str());
    ASSERT_EQ(19, s2.Size());

    // The string should correctly append characters
    String s5;
    s5 += 'h';
    ASSERT_STREQ("h", s5.Str());
    s5 += 'i';
    ASSERT_STREQ("hi", s5.Str());
    s5.Append('n');
    ASSERT_STREQ("hin", s5.Str());
    s5.Append('t');
    ASSERT_STREQ("hint", s5.Str());
    ASSERT_EQ(4, s5.Size());

    // The string should correctly append others strings
    String h("hello");
    String g("goodbye");
    String s3;
    s3 += h;
    ASSERT_STREQ(h.Str(), s3.Str());
    String s4 = g;
    s4.Append(s3);
    ASSERT_STREQ("goodbyehello", s4.Str());
    s4.Append("goodbye");
    ASSERT_STREQ("goodbyehellogoodbye", s4.Str());
    ASSERT_STREQ("hello", s3.Str());
    ASSERT_STRNE("goodbyehello", s3.Str());
    ASSERT_EQ(19, s4.Size());
}

UTEST(test_String, Prepend)
{
    // The string should correctly prepend c-strings
    String s1;
    s1.Prepend("hello");
    ASSERT_STREQ("hello", s1.Str());
    s1.Prepend("goodbye");
    ASSERT_STREQ("goodbyehello", s1.Str());
    s1.Prepend("goodbye");
    ASSERT_STREQ("goodbyegoodbyehello", s1.Str());
    ASSERT_EQ(19, s1.Size());

    // The string should correctly prepend characters
    String s5('t');
    ASSERT_STREQ("t", s5.Str());
    s5.Prepend('n');
    ASSERT_STREQ("nt", s5.Str());
    s5.Prepend('i');
    ASSERT_STREQ("int", s5.Str());
    ASSERT_EQ(3, s5.Size());

    // The string should correctly prepend other strings
    String s2;
    String h("hello");
    String g("goodbye");
    s2.Prepend(h);
    ASSERT_STREQ(h.Str(), s2.Str());
    String s3 = g;
    s3.Prepend(s2);
    ASSERT_STREQ("hellogoodbye", s3.Str());
    s3.Prepend("goodbye");
    ASSERT_STREQ("goodbyehellogoodbye", s3.Str());
    ASSERT_STREQ("hello", s2.Str());
    ASSERT_STRNE("goodbyehello", s2.Str());
    ASSERT_EQ(19, s3.Size());
}

UTEST(test_String, Index)
{
    // The string can be indexed
    String s1("abcdefg");
    ASSERT_EQ('g', s1.At(-1));
    ASSERT_EQ('a', s1.At(0));
    ASSERT_EQ('b', s1.At(1));
    ASSERT_EQ('c', s1.At(2));
    ASSERT_EQ('g', s1.At(6));
    ASSERT_EQ('\0', s1.At(7));
    ASSERT_EQ('\0', s1.At(20));
    ASSERT_EQ('d', s1.At(-4));
    ASSERT_EQ('\0', s1.At(-15));
    ASSERT_EQ('a', s1[0]);
    ASSERT_EQ('d', s1[3]);
    ASSERT_EQ('g', s1[6]);
    ASSERT_EQ('g', s1[-1]);
    ASSERT_EQ('e', s1[-3]);
    ASSERT_EQ('a', s1[-7]);
    s1[1] = 'z';
    ASSERT_EQ('z', s1.At(1));
}

UTEST(test_String, Erase)
{
    // The string can be erased
    String s1("this is a string of a certain length");
    s1.Erase();
    ASSERT_FALSE(s1);
    s1 = "this is a string of a certain length";
    s1.Erase(7);
    ASSERT_STREQ("this is", s1.Str());
    s1.Erase(3, 3);
    ASSERT_STREQ("this", s1.Str());
    s1 = "this is another string";
    s1.Erase(4, 11);
    ASSERT_STREQ("this string", s1.Str());
    s1.Erase(4, 11);
    ASSERT_FALSE(s1);
    s1 = "four more strings! four more strings!";
    s1.Erase(-19);
    ASSERT_STREQ("four more strings!", s1.Str());
    s1.Erase(-13, 5);
    ASSERT_STREQ("four strings!", s1.Str());
    s1.Erase(-14);
    ASSERT_FALSE(s1);
    s1 = "last one";
    s1.Erase(-1, 2);
    ASSERT_FALSE(s1);
}

UTEST(test_String, Insert)
{
    // The string can handle insertion
    String s1(" is a  of a  length");
    s1.Insert(5, "");
    ASSERT_STREQ(" is a  of a  length", s1.Str());
    s1.Insert(0, "this");
    ASSERT_STREQ("this is a  of a  length", s1.Str());
    s1.Insert(10, "short");
    ASSERT_STREQ("this is a short of a  length", s1.Str());
    s1.Insert(21, "reasonable");
    ASSERT_STREQ("this is a short of a reasonable length", s1.Str());
    s1.Insert(38, ", really");
    ASSERT_STREQ("this is a short of a reasonable length, really", s1.Str());
    s1.Insert(-1, "!");
    ASSERT_STREQ("this is a short of a reasonable length, really!", s1.Str());
    s1.Insert(-44, " string");
    ASSERT_STREQ("this string is a short of a reasonable length, really!", s1.Str());
    s1.Insert(-55, "... ");
    ASSERT_STREQ("... this string is a short of a reasonable length, really!", s1.Str());
    s1.Insert(-60, "this should not be inserted");
    ASSERT_STREQ("... this string is a short of a reasonable length, really!", s1.Str());
}

UTEST(test_String, Swap)
{
    // The string can be swapped
    String s1("stringA");
    String s2 = "stringB";
    s1.Swap(s2);
    ASSERT_STREQ("stringB", s1.Str());
    ASSERT_STREQ("stringA", s2.Str());
    s1.Swap(s2);
    ASSERT_STREQ("stringA", s1.Str());
    ASSERT_STREQ("stringB", s2.Str());
    s2.Swap(s1);
    ASSERT_STREQ("stringB", s1.Str());
    ASSERT_STREQ("stringA", s2.Str());
}

UTEST(test_String, Cast)
{
    // The string implicitly casts to c-string
    String s1("hellogoodbye");
    const char* cstr = s1;
    ASSERT_EQ(0, std::strcmp(cstr, "hellogoodbye"));
}

UTEST(test_String, Find)
{
    // The string can be searched for substrings
    String s("ruSt Is a MuCH SafEr lANgUagE...");
    String s1("lANgUagE");
    String s2("...");
    String s3("rUBy");
    ASSERT_EQ(0, s.Find("ruSt"));
    ASSERT_EQ(8, s.Find("a MuCH SafEr"));
    ASSERT_EQ(21, s.Find(s1));
    ASSERT_EQ(29, s.Find(s2));
    ASSERT_EQ(-1, s.Find(s3));
    ASSERT_EQ(8, s.Find('a'));
    ASSERT_EQ(8, s.Find("a", 8));
    ASSERT_EQ(16, s.Find('a', 9));
    ASSERT_EQ(26, s.Find("a", -11));
}

UTEST(test_String, Replace)
{
    // The string can replace by substring
    String s("ruSt Is a MuCH SafEr lANgUagE...");
    String s3("rUBy");
    ASSERT_TRUE(s.Replace("ruSt", s3));
    ASSERT_STREQ("rUBy Is a MuCH SafEr lANgUagE...", s.Str());
    ASSERT_EQ(0, s.Find(s3));
    ASSERT_FALSE(s.Replace("ruSt", "NiCEr"));
    ASSERT_TRUE(s.Replace("SafEr", "NiCEr"));
    ASSERT_STREQ("rUBy Is a MuCH NiCEr lANgUagE...", s.Str());
    ASSERT_EQ(15, s.Find("NiCEr"));
    ASSERT_TRUE(s.Replace(" a MuCH NiCEr lANgUagE", ""));
    ASSERT_STREQ("rUBy Is...", s.Str());
    ASSERT_TRUE(s.Replace("...", " pyThoN wItH exTra sTepS"));
    ASSERT_STREQ("rUBy Is pyThoN wItH exTra sTepS", s.Str());
    s.Clear();
    ASSERT_FALSE(s.Replace("ruSt", s3));
    ASSERT_STREQ("", s.Str());
}

UTEST(test_String, Split)
{
    // The string can be split into substrings by delimiter
    String s("ruSt Is a MuCH SafEr lANgUagE...");
    std::vector<String> vec1 = s.Split(" ");
    ASSERT_EQ(6, vec1.size());
    ASSERT_STREQ("ruSt", vec1[0].Str());
    ASSERT_STREQ("Is", vec1[1].Str());
    ASSERT_STREQ("a", vec1[2].Str());
    ASSERT_STREQ("MuCH", vec1[3].Str());
    ASSERT_STREQ("SafEr", vec1[4].Str());
    ASSERT_STREQ("lANgUagE...", vec1[5].Str());

    String s4("hi:my,name,is");
    std::vector<String> vec2 = s4.Split(",");
    ASSERT_EQ(3, vec2.size());
    ASSERT_STREQ("hi:my", vec2[0].Str());
    ASSERT_STREQ("name", vec2[1].Str());
    ASSERT_STREQ("is", vec2[2].Str());

    std::vector<String> vec3 = s4.Split(":,");
    ASSERT_EQ(4, vec3.size());
    ASSERT_STREQ("hi", vec3[0].Str());
    ASSERT_STREQ("my", vec3[1].Str());
    ASSERT_STREQ("name", vec3[2].Str());
    ASSERT_STREQ("is", vec3[3].Str());

    String str("hello+goodbye+blah");
    std::vector<String> vec4 = String("+" + str).Split("+");
    ASSERT_EQ(3, vec4.size());

    std::vector<String> vec5 = String(str + "+").Split("+");
    ASSERT_EQ(3, vec5.size());

    std::vector<String> vec6 = String("").Split("+");
    ASSERT_TRUE(vec6.empty());

    std::vector<String> vec7 = String("blah&bleh").Split("\0");
    ASSERT_EQ(1, vec7.size());
    ASSERT_STREQ("blah&bleh", vec7[0].Str());
}

UTEST(test_String, ToUpper)
{
    // The string can be upper-cased
    String s("ruSt Is a MuCH SafEr lANgUagE...");
    s.ToUpper();
    ASSERT_STREQ("RUST IS A MUCH SAFER LANGUAGE...", s.Str());
}

UTEST(test_String, ToLower)
{
    // The string can be lower-cased
    String s("ruSt Is a MuCH SafEr lANgUagE...");
    s.ToLower();
    ASSERT_STREQ("rust is a much safer language...", s.Str());
    s = "34143523322421@#@#!@##%";
    s.ToLower();
    ASSERT_STREQ("34143523322421@#@#!@##%", s.Str());
}

UTEST(test_String, Format)
{
    // The string can be formatted
    String fs("This is a %s: ");
    String fss(fs + "%s");
    fss.Format("string", "hi");
    ASSERT_STREQ("This is a string: hi", fss.Str());
    String fsd(fs + "%d");
    fsd.Format("decimal", 5);
    ASSERT_STREQ("This is a decimal: 5", fsd.Str());
    fsd.Format(200);
    ASSERT_STREQ("This is a decimal: 5", fsd.Str());
    fsd.Clear();
    fsd.Format("200");
    ASSERT_TRUE(fsd.IsEmpty());
}

UTEST(test_String, SubString)
{
    // The string can generate substrings
    String s("ruSt Is a MuCH SafEr lANgUagE...");
    String sub = s.SubString();
    ASSERT_STREQ("ruSt Is a MuCH SafEr lANgUagE...", sub.Str());
    sub = s.SubString(5);
    ASSERT_STREQ("Is a MuCH SafEr lANgUagE...", sub.Str());
    sub = s.SubString(21, 8);
    ASSERT_STREQ("lANgUagE", sub.Str());
    sub = s.SubString(-3, 3);
    ASSERT_STREQ("...", sub.Str());
    sub = s.SubString(-32, 4);
    ASSERT_STREQ("ruSt", sub.Str());
    sub = s.SubString(-33, 5);
    ASSERT_FALSE(sub);
    sub = s.SubString(0, -2);
    ASSERT_FALSE(sub);
    sub = s.SubString(0, -1);
    ASSERT_STREQ("ruSt Is a MuCH SafEr lANgUagE...", sub.Str());
    sub = s.SubString(32, 4);
    ASSERT_FALSE(sub);
    sub = s.SubString(31);
    ASSERT_STREQ(".", sub.Str());
    sub = s.SubString(0, 200);
    ASSERT_FALSE(sub);
}

UTEST(test_String, PopBack)
{
    // The string can pop back
    String s("ruSt Is a MuCH SafEr lANgUagE...");
    ASSERT_EQ('.', s.PopBack());
    ASSERT_STREQ("ruSt Is a MuCH SafEr lANgUagE..", s.Str());
    ASSERT_EQ('.', s.PopBack());
    ASSERT_STREQ("ruSt Is a MuCH SafEr lANgUagE.", s.Str());
    ASSERT_EQ('.', s.PopBack());
    ASSERT_STREQ("ruSt Is a MuCH SafEr lANgUagE", s.Str());
    ASSERT_EQ('E', s.PopBack());
    ASSERT_STREQ("ruSt Is a MuCH SafEr lANgUag", s.Str());
}

UTEST(test_String, GetLine)
{
    // The string can get its next line
    String s4("hi\nmy\nname\nis");
    String line;
    ASSERT_TRUE(s4.GetLine(line));
    ASSERT_STREQ("hi", line.Str());
    ASSERT_TRUE(s4.GetLine(line));
    ASSERT_STREQ("my", line.Str());
    ASSERT_TRUE(s4.GetLine(line));
    ASSERT_STREQ("name", line.Str());
    ASSERT_FALSE(s4.GetLine(line));
    ASSERT_STREQ("is", line.Str());
    ASSERT_FALSE(s4.GetLine(line));
    ASSERT_STREQ("", line.Str());
    s4 = "hi\nmy\nname\nis\n\nhello";
    ASSERT_TRUE(s4.GetLine(line));
    ASSERT_STREQ("hi", line.Str());
    ASSERT_TRUE(s4.GetLine(line));
    ASSERT_STREQ("my", line.Str());
    ASSERT_TRUE(s4.GetLine(line));
    ASSERT_STREQ("name", line.Str());
    ASSERT_TRUE(s4.GetLine(line));
    ASSERT_STREQ("is", line.Str());
    ASSERT_TRUE(s4.GetLine(line));
    ASSERT_STREQ("", line.Str());
    ASSERT_FALSE(s4.GetLine(line));
    ASSERT_STREQ("hello", line.Str());
}

UTEST(test_String, GetInt)
{
    // The string can be converted to int
    int value;
    String s4("2");
    ASSERT_TRUE(s4.GetInt(value));
    ASSERT_EQ(2, value);
    s4 = "3.3";
    ASSERT_TRUE(s4.GetInt(value));
    ASSERT_EQ(3, value);
    s4 = "5.6";
    ASSERT_TRUE(s4.GetInt(value));
    ASSERT_EQ(5, value);
    s4 = "-10";
    ASSERT_TRUE(s4.GetInt(value));
    ASSERT_EQ(-10, value);
    s4 = "";
    ASSERT_FALSE(s4.GetInt(value));
    s4 = "hello";
    ASSERT_FALSE(s4.GetInt(value));
    s4 = "a";
    ASSERT_FALSE(s4.GetInt(value));
}

UTEST(test_String, GetFloat)
{
    // The string can be converted to float
    float value;
    String s4("2");
    ASSERT_TRUE(s4.GetFloat(value));
    ASSERT_EQ(2.f, value);
    s4 = "3.3";
    ASSERT_TRUE(s4.GetFloat(value));
    ASSERT_EQ(3.3f, value);
    s4 = "5.6";
    ASSERT_TRUE(s4.GetFloat(value));
    ASSERT_EQ(5.6f, value);
    s4 = "-10";
    ASSERT_TRUE(s4.GetFloat(value));
    ASSERT_EQ(-10.f, value);
    s4 = "";
    ASSERT_FALSE(s4.GetFloat(value));
    s4 = "hello";
    ASSERT_FALSE(s4.GetFloat(value));
    s4 = "a";
    ASSERT_FALSE(s4.GetFloat(value));
}

UTEST(test_String, FromInt)
{
    // The string can be converted from int
    ASSERT_STREQ("2", String::FromInt(2).Str());
    ASSERT_STREQ("90000000", String::FromInt(90000000).Str());
}

UTEST(test_String, FromFloat)
{
    // The string can be converted from float
    ASSERT_STREQ("2.000000", String::FromFloat(2.f).Str());
    ASSERT_STREQ("9.001430", String::FromFloat(9.00143f).Str());
}

UTEST(test_String, FromDouble)
{
    // The string can be converted from double
    ASSERT_STREQ("0.14159265358979323", String::FromDouble(0.14159265358979323L).Str());
    ASSERT_STREQ("0.04159245358979343", String::FromDouble(0.04159245358979343L).Str());
}

UTEST(test_String, FromLong)
{
    // The string can be converted from long
    ASSERT_STREQ("2", String::FromLong(2l).Str());
    ASSERT_STREQ("90000000000000", String::FromLong(90000000000000l).Str());
}

UTEST(test_String, OnHeap)
{
    // The string should make use of small string allocation
    ASSERT_LE(16, sizeof(String));
    String s1;
    ASSERT_FALSE(s1.OnHeap());
    ASSERT_EQ(0, s1.Size());
    ASSERT_EQ(15, s1.Capacity());

    s1 += "hello";
    ASSERT_FALSE(s1.OnHeap());
    ASSERT_EQ(5, s1.Size());
    ASSERT_EQ(15, s1.Capacity());

    String s2("world");
    ASSERT_FALSE(s1.OnHeap());
    ASSERT_EQ(5, s1.Size());
    ASSERT_EQ(15, s1.Capacity());

    // It should continue to hold up to 15 characters on the stack
    ((s1 += " ") += s2) += "!!!!";
    ASSERT_FALSE(s1.OnHeap());
    ASSERT_EQ(15, s1.Size());
    ASSERT_EQ(15, s1.Capacity());

    // Hitting 16 characters should trigger a reallocation to the heap
    s1 += "!";
    ASSERT_TRUE(s1.OnHeap());
    ASSERT_EQ(16, s1.Size());
    ASSERT_EQ(16, s1.Capacity());

    s1 += " I am here!";
    ASSERT_TRUE(s1.OnHeap());
    ASSERT_EQ(27, s1.Size());
    ASSERT_EQ(27, s1.Capacity());

    // Reassigning to a smaller string should not reduce capacity, or revert to the stack
    s1 = "No longer!";
    ASSERT_TRUE(s1.OnHeap());
    ASSERT_EQ(10, s1.Size());
    ASSERT_EQ(27, s1.Capacity());

    // It can handle values of the length of its specified max size
    char bigStr[String::MAX_SIZE + 2];
    memset(bigStr, '1', String::MAX_SIZE);
    bigStr[String::MAX_SIZE] = '\0';
    s1 = bigStr;
    ASSERT_TRUE(s1.OnHeap());
    ASSERT_EQ(String::MAX_SIZE, s1.Size());
    ASSERT_EQ(String::MAX_SIZE, s1.Capacity());

    // It will fail to assign values of lengths above its specified max size
    bigStr[String::MAX_SIZE] = '1';
    bigStr[String::MAX_SIZE + 1] = '\0';
    s1 = bigStr;
    ASSERT_TRUE(s1.OnHeap());
    ASSERT_EQ(String::MAX_SIZE, s1.Size());
    ASSERT_EQ(String::MAX_SIZE, s1.Capacity());
}

UTEST(test_String, Reset)
{
    // The string should allow for resetting to default values
    String s1;
    ASSERT_FALSE(s1);
    ASSERT_EQ(0, s1.Size());
    ASSERT_EQ(15, s1.Capacity());
    ASSERT_FALSE(s1.OnHeap());

    s1.Reset();
    ASSERT_FALSE(s1);
    ASSERT_EQ(0, s1.Size());
    ASSERT_EQ(15, s1.Capacity());
    ASSERT_FALSE(s1.OnHeap());

    // Small string should respect reset calls
    s1 = "stack string!!!";
    ASSERT_TRUE(s1);
    ASSERT_EQ(15, s1.Size());
    ASSERT_EQ(15, s1.Capacity());
    ASSERT_FALSE(s1.OnHeap());

    s1.Reset();
    ASSERT_FALSE(s1);
    ASSERT_EQ(0, s1.Size());
    ASSERT_EQ(15, s1.Capacity());
    ASSERT_FALSE(s1.OnHeap());

    // Large strings should respect reset calls
    s1 = "minimum heap str";
    ASSERT_TRUE(s1);
    ASSERT_EQ(16, s1.Size());
    ASSERT_EQ(16, s1.Capacity());
    ASSERT_TRUE(s1.OnHeap());

    s1.Reset();
    ASSERT_FALSE(s1);
    ASSERT_EQ(0, s1.Size());
    ASSERT_EQ(15, s1.Capacity());
    ASSERT_FALSE(s1.OnHeap());
}
