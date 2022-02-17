#include <utest.h>

#include <utils/String.h>

UTEST(test_String, DefaultConstruct)
{
    // The string should default construct correctly
    String s;
    ASSERT_FALSE(s);
    ASSERT_TRUE(s.IsEmpty());
    ASSERT_EQ(s.Size(), 0);
    ASSERT_TRUE(s == "");
    ASSERT_TRUE("" == s);
    ASSERT_EQ(std::strcmp(s.Str(), ""), 0);

    // Assignment via assignment operator should apply
    String h("hello");
    s = h;
    ASSERT_FALSE(s.IsEmpty());
    ASSERT_EQ(s.Size(), 5);
    ASSERT_STREQ(s.Str(), h.Str());
    ASSERT_STREQ(h.Str(), s.Str());
    ASSERT_EQ(std::strcmp(s.Str(), h.Str()), 0);

    // Assignment of c-strings via assignment operator should apply
    s = "hello";
    ASSERT_FALSE(s.IsEmpty());
    ASSERT_EQ(s.Size(), 5);
    ASSERT_STREQ(s.Str(), "hello");
    ASSERT_STREQ("hello", s.Str());
    ASSERT_EQ(std::strcmp(s.Str(), "hello"), 0);
}

UTEST(test_String, CopyConstruct)
{
    // The string should copy construct correctly
    String s1("hello");
    String s2(s1);
    String s3;
    String s4(s3);
    ASSERT_TRUE(s2);
    ASSERT_STREQ(s2.Str(), "hello");
    ASSERT_FALSE(s4);

    // Assignment via assignment operator should apply
    s2 = "goodbye";
    ASSERT_TRUE(s2);
    ASSERT_STREQ(s2.Str(), "goodbye");
    s3 = s4;
    ASSERT_FALSE(s3);

    // Copy construction from c-strings should apply
    String s("hello");
    ASSERT_TRUE(s);
    ASSERT_STREQ(s.Str(), "hello");
    s = "goodbye";
    ASSERT_TRUE(s);
    ASSERT_STREQ(s.Str(), "goodbye");

    // Copy construction from characters should apply
    String s5('h');
    ASSERT_TRUE(s5);
    ASSERT_STREQ(s5.Str(), "h");
    s5 = 'i';
    ASSERT_TRUE(s5);
    ASSERT_STREQ(s5.Str(), "i");
}

UTEST(test_String, MoveConstruct)
{
    // The string should move construct correctly
    String s1(String("hello"));
    String s2((String()));
    ASSERT_TRUE(s1);
    ASSERT_STREQ(s1.Str(), "hello");
    ASSERT_FALSE(s2);

    // Assignment via move assignment should apply
    s1 = String("goodbye");
    ASSERT_TRUE(s1);
    ASSERT_STREQ(s1.Str(), "goodbye");
    s1 = String();
    ASSERT_FALSE(s1);
}

UTEST(test_String, Clear)
{
    // The string can be cleared
    String s1("hello");
    ASSERT_TRUE(s1);
    ASSERT_EQ(s1.Size(), 5);
    s1.Clear();
    ASSERT_FALSE(s1);
    ASSERT_EQ(s1.Size(), 0);
}

UTEST(test_String, Add)
{
    // The addition operator creates prepended strings with c-strings
    String s1;
    String s2(s1 + "hello");
    ASSERT_STREQ(s2.Str(), "hello");
    s2 = s2 + "goodbye";
    ASSERT_STREQ(s2.Str(), "hellogoodbye");
    s2 = "goodbye" + s2;
    ASSERT_STREQ(s2.Str(), "goodbyehellogoodbye");
    ASSERT_EQ(s2.Size(), 19);

    // The addition operator creates prepended strings with characters
    String s3;
    String s4(s3 + 'h');
    ASSERT_STREQ(s4.Str(), "h");
    s4 = s4 + 'i';
    ASSERT_STREQ(s4.Str(), "hi");
    s4 = 't' + s4 + 's';
    ASSERT_STREQ(s4.Str(), "this");
    ASSERT_EQ(s4.Size(), 4);

    // The addition operator creates prepended strings with other strings
    String h("hello");
    String g("goodbye");
    String s5 = String() + h;
    ASSERT_STREQ(s5.Str(), "hello");
    s5 = s5 + g;
    ASSERT_STREQ(s5.Str(), "hellogoodbye");
    s5 = g + s5;
    ASSERT_STREQ(s5.Str(), "goodbyehellogoodbye");
    ASSERT_EQ(s5.Size(), 19);
}

UTEST(test_String, Append)
{
    // The string should correctly append c-strings
    String s1;
    s1 += "hello";
    ASSERT_STREQ(s1.Str(), "hello");
    String s2 = "goodbye" + s1;
    ASSERT_STREQ(s2.Str(), "goodbyehello");
    s2.Append("goodbye");
    ASSERT_STREQ(s2.Str(), "goodbyehellogoodbye");
    ASSERT_EQ(s2.Size(), 19);

    // The string should correctly append characters
    String s5;
    s5 += 'h';
    ASSERT_STREQ(s5.Str(), "h");
    s5 += 'i';
    ASSERT_STREQ(s5.Str(), "hi");
    s5.Append('n');
    ASSERT_STREQ(s5.Str(), "hin");
    s5.Append('t');
    ASSERT_STREQ(s5.Str(), "hint");
    ASSERT_EQ(s5.Size(), 4);

    // The string should correctly append others strings
    String h("hello");
    String g("goodbye");
    String s3;
    s3 += h;
    ASSERT_STREQ(s3.Str(), h.Str());
    String s4 = g;
    s4.Append(s3);
    ASSERT_STREQ(s4.Str(), "goodbyehello");
    s4.Append("goodbye");
    ASSERT_STREQ(s4.Str(), "goodbyehellogoodbye");
    ASSERT_STREQ(s3.Str(), "hello");
    ASSERT_STRNE(s3.Str(), "goodbyehello");
    ASSERT_EQ(s4.Size(), 19);
}

UTEST(test_String, Prepend)
{
    // The string should correctly prepend c-strings
    String s1;
    s1.Prepend("hello");
    ASSERT_STREQ(s1.Str(), "hello");
    s1.Prepend("goodbye");
    ASSERT_STREQ(s1.Str(), "goodbyehello");
    s1.Prepend("goodbye");
    ASSERT_STREQ(s1.Str(), "goodbyegoodbyehello");
    ASSERT_EQ(s1.Size(), 19);

    // The string should correctly prepend characters
    String s5('t');
    ASSERT_STREQ(s5.Str(), "t");
    s5.Prepend('n');
    ASSERT_STREQ(s5.Str(), "nt");
    s5.Prepend('i');
    ASSERT_STREQ(s5.Str(), "int");
    ASSERT_EQ(s5.Size(), 3);

    // The string should correctly prepend other strings
    String s2;
    String h("hello");
    String g("goodbye");
    s2.Prepend(h);
    ASSERT_STREQ(s2.Str(), h.Str());
    String s3 = g;
    s3.Prepend(s2);
    ASSERT_STREQ(s3.Str(), "hellogoodbye");
    s3.Prepend("goodbye");
    ASSERT_STREQ(s3.Str(), "goodbyehellogoodbye");
    ASSERT_STREQ(s2.Str(), "hello");
    ASSERT_STRNE(s2.Str(), "goodbyehello");
    ASSERT_EQ(s3.Size(), 19);
}

UTEST(test_String, Index)
{
    // The string can be indexed
    String s1("abcdefg");
    ASSERT_EQ(s1.At(-1), 'g');
    ASSERT_EQ(s1.At(0), 'a');
    ASSERT_EQ(s1.At(1), 'b');
    ASSERT_EQ(s1.At(2), 'c');
    ASSERT_EQ(s1.At(6), 'g');
    ASSERT_EQ(s1.At(7), '\0');
    ASSERT_EQ(s1.At(20), '\0');
    ASSERT_EQ(s1.At(-4), 'd');
    ASSERT_EQ(s1.At(-15), '\0');
    ASSERT_EQ(s1[0], 'a');
    ASSERT_EQ(s1[3], 'd');
    ASSERT_EQ(s1[6], 'g');
    ASSERT_EQ(s1[-1], 'g');
    ASSERT_EQ(s1[-3], 'e');
    ASSERT_EQ(s1[-7], 'a');
    s1[1] = 'z';
    ASSERT_EQ(s1.At(1), 'z');
}

UTEST(test_String, Erase)
{
    // The string can be erased
    String s1("this is a string of a certain length");
    s1.Erase();
    ASSERT_FALSE(s1);
    s1 = "this is a string of a certain length";
    s1.Erase(7);
    ASSERT_STREQ(s1.Str(), "this is");
    s1.Erase(3, 3);
    ASSERT_STREQ(s1.Str(), "this");
    s1 = "this is another string";
    s1.Erase(4, 11);
    ASSERT_STREQ(s1.Str(), "this string");
    s1.Erase(4, 11);
    ASSERT_FALSE(s1);
    s1 = "four more strings! four more strings!";
    s1.Erase(-19);
    ASSERT_STREQ(s1.Str(), "four more strings!");
    s1.Erase(-13, 5);
    ASSERT_STREQ(s1.Str(), "four strings!");
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
    ASSERT_STREQ(s1.Str(), " is a  of a  length");
    s1.Insert(0, "this");
    ASSERT_STREQ(s1.Str(), "this is a  of a  length");
    s1.Insert(10, "short");
    ASSERT_STREQ(s1.Str(), "this is a short of a  length");
    s1.Insert(21, "reasonable");
    ASSERT_STREQ(s1.Str(), "this is a short of a reasonable length");
    s1.Insert(38, ", really");
    ASSERT_STREQ(s1.Str(), "this is a short of a reasonable length, really");
    s1.Insert(-1, "!");
    ASSERT_STREQ(s1.Str(), "this is a short of a reasonable length, really!");
    s1.Insert(-44, " string");
    ASSERT_STREQ(s1.Str(), "this string is a short of a reasonable length, really!");
    s1.Insert(-55, "... ");
    ASSERT_STREQ(s1.Str(), "... this string is a short of a reasonable length, really!");
    s1.Insert(-60, "this should not be inserted");
    ASSERT_STREQ(s1.Str(), "... this string is a short of a reasonable length, really!");
}

UTEST(test_String, Swap)
{
    // The string can be swapped
    String s1("stringA");
    String s2 = "stringB";
    s1.Swap(s2);
    ASSERT_STREQ(s1.Str(), "stringB");
    ASSERT_STREQ(s2.Str(), "stringA");
    s1.Swap(s2);
    ASSERT_STREQ(s1.Str(), "stringA");
    ASSERT_STREQ(s2.Str(), "stringB");
    s2.Swap(s1);
    ASSERT_STREQ(s1.Str(), "stringB");
    ASSERT_STREQ(s2.Str(), "stringA");
}

UTEST(test_String, Cast)
{
    // The string implicitly casts to c-string
    String s1("hellogoodbye");
    const char* cstr = s1;
    ASSERT_EQ(std::strcmp(cstr, "hellogoodbye"), 0);
}

UTEST(test_String, Find)
{
    // The string can be searched for substrings
    String s("ruSt Is a MuCH SafEr lANgUagE...");
    String s1("lANgUagE");
    String s2("...");
    String s3("rUBy");
    ASSERT_EQ(s.Find("ruSt"), 0);
    ASSERT_EQ(s.Find("a MuCH SafEr"), 8);
    ASSERT_EQ(s.Find(s1), 21);
    ASSERT_EQ(s.Find(s2), 29);
    ASSERT_EQ(s.Find(s3), -1);
    ASSERT_EQ(s.Find('a'), 8);
    ASSERT_EQ(s.Find("a", 8), 8);
    ASSERT_EQ(s.Find('a', 9), 16);
    ASSERT_EQ(s.Find("a", -11), 26);
}

UTEST(test_String, Replace)
{
    // The string can replace by substring
    String s("ruSt Is a MuCH SafEr lANgUagE...");
    String s3("rUBy");
    ASSERT_TRUE(s.Replace("ruSt", s3));
    ASSERT_STREQ(s.Str(), "rUBy Is a MuCH SafEr lANgUagE...");
    ASSERT_EQ(s.Find(s3), 0);
    ASSERT_FALSE(s.Replace("ruSt", "NiCEr"));
    ASSERT_TRUE(s.Replace("SafEr", "NiCEr"));
    ASSERT_STREQ(s.Str(), "rUBy Is a MuCH NiCEr lANgUagE...");
    ASSERT_EQ(s.Find("NiCEr"), 15);
    ASSERT_TRUE(s.Replace(" a MuCH NiCEr lANgUagE", ""));
    ASSERT_STREQ(s.Str(), "rUBy Is...");
    ASSERT_TRUE(s.Replace("...", " pyThoN wItH exTra sTepS"));
    ASSERT_STREQ(s.Str(), "rUBy Is pyThoN wItH exTra sTepS");
    s.Clear();
    ASSERT_FALSE(s.Replace("ruSt", s3));
    ASSERT_STREQ(s.Str(), "");
}

UTEST(test_String, Split)
{
    // The string can be split into substrings by delimiter
    String s("ruSt Is a MuCH SafEr lANgUagE...");
    std::vector<String> vec1 = s.Split(" ");
    ASSERT_EQ(vec1.size(), 6);
    ASSERT_STREQ(vec1[0].Str(), "ruSt");
    ASSERT_STREQ(vec1[1].Str(), "Is");
    ASSERT_STREQ(vec1[2].Str(), "a");
    ASSERT_STREQ(vec1[3].Str(), "MuCH");
    ASSERT_STREQ(vec1[4].Str(), "SafEr");
    ASSERT_STREQ(vec1[5].Str(), "lANgUagE...");

    String s4("hi:my,name,is");
    std::vector<String> vec2 = s4.Split(",");
    ASSERT_EQ(vec2.size(), 3);
    ASSERT_STREQ(vec2[0].Str(), "hi:my");
    ASSERT_STREQ(vec2[1].Str(), "name");
    ASSERT_STREQ(vec2[2].Str(), "is");

    std::vector<String> vec3 = s4.Split(":,");
    ASSERT_EQ(vec3.size(), 4);
    ASSERT_STREQ(vec3[0].Str(), "hi");
    ASSERT_STREQ(vec3[1].Str(), "my");
    ASSERT_STREQ(vec3[2].Str(), "name");
    ASSERT_STREQ(vec3[3].Str(), "is");

    String str("hello+goodbye+blah");
    std::vector<String> vec4 = String("+" + str).Split("+");
    ASSERT_EQ(vec4.size(), 3);

    std::vector<String> vec5 = String(str + "+").Split("+");
    ASSERT_EQ(vec5.size(), 3);

    std::vector<String> vec6 = String("").Split("+");
    ASSERT_TRUE(vec6.empty());

    std::vector<String> vec7 = String("blah&bleh").Split("\0");
    ASSERT_EQ(vec7.size(), 1);
    ASSERT_STREQ(vec7[0].Str(), "blah&bleh");
}

UTEST(test_String, ToUpper)
{
    // The string can be upper-cased
    String s("ruSt Is a MuCH SafEr lANgUagE...");
    s.ToUpper();
    ASSERT_STREQ(s.Str(), "RUST IS A MUCH SAFER LANGUAGE...");
}

UTEST(test_String, ToLower)
{
    // The string can be lower-cased
    String s("ruSt Is a MuCH SafEr lANgUagE...");
    s.ToLower();
    ASSERT_STREQ(s.Str(), "rust is a much safer language...");
    s = "34143523322421@#@#!@##%";
    s.ToLower();
    ASSERT_STREQ(s.Str(), "34143523322421@#@#!@##%");
}

UTEST(test_String, Format)
{
    // The string can be formatted
    String fs("This is a %s: ");
    String fss(fs + "%s");
    fss.Format("string", "hi");
    ASSERT_STREQ(fss.Str(), "This is a string: hi");
    String fsd(fs + "%d");
    fsd.Format("decimal", 5);
    ASSERT_STREQ(fsd.Str(), "This is a decimal: 5");
    fsd.Format(200);
    ASSERT_STREQ(fsd.Str(), "This is a decimal: 5");
    fsd.Clear();
    fsd.Format("200");
    ASSERT_TRUE(fsd.IsEmpty());
}

UTEST(test_String, SubString)
{
    // The string can generate substrings
    String s("ruSt Is a MuCH SafEr lANgUagE...");
    String sub = s.SubString();
    ASSERT_STREQ(sub.Str(), "ruSt Is a MuCH SafEr lANgUagE...");
    sub = s.SubString(5);
    ASSERT_STREQ(sub.Str(), "Is a MuCH SafEr lANgUagE...");
    sub = s.SubString(21, 8);
    ASSERT_STREQ(sub.Str(), "lANgUagE");
    sub = s.SubString(-3, 3);
    ASSERT_STREQ(sub.Str(), "...");
    sub = s.SubString(-32, 4);
    ASSERT_STREQ(sub.Str(), "ruSt");
    sub = s.SubString(-33, 5);
    ASSERT_FALSE(sub);
    sub = s.SubString(0, -2);
    ASSERT_FALSE(sub);
    sub = s.SubString(0, -1);
    ASSERT_STREQ(sub.Str(), "ruSt Is a MuCH SafEr lANgUagE...");
    sub = s.SubString(32, 4);
    ASSERT_FALSE(sub);
    sub = s.SubString(31);
    ASSERT_STREQ(sub.Str(), ".");
    sub = s.SubString(0, 200);
    ASSERT_FALSE(sub);
}

UTEST(test_String, PopBack)
{
    // The string can pop back
    String s("ruSt Is a MuCH SafEr lANgUagE...");
    ASSERT_EQ(s.PopBack(), '.');
    ASSERT_STREQ(s.Str(), "ruSt Is a MuCH SafEr lANgUagE..");
    ASSERT_EQ(s.PopBack(), '.');
    ASSERT_STREQ(s.Str(), "ruSt Is a MuCH SafEr lANgUagE.");
    ASSERT_EQ(s.PopBack(), '.');
    ASSERT_STREQ(s.Str(), "ruSt Is a MuCH SafEr lANgUagE");
    ASSERT_EQ(s.PopBack(), 'E');
    ASSERT_STREQ(s.Str(), "ruSt Is a MuCH SafEr lANgUag");
}

UTEST(test_String, GetLine)
{
    // The string can get its next line
    String s4("hi\nmy\nname\nis");
    String line;
    ASSERT_TRUE(s4.GetLine(line));
    ASSERT_STREQ(line.Str(), "hi");
    ASSERT_TRUE(s4.GetLine(line));
    ASSERT_STREQ(line.Str(), "my");
    ASSERT_TRUE(s4.GetLine(line));
    ASSERT_STREQ(line.Str(), "name");
    ASSERT_FALSE(s4.GetLine(line));
    ASSERT_STREQ(line.Str(), "is");
    ASSERT_FALSE(s4.GetLine(line));
    ASSERT_STREQ(line.Str(), "");
    s4 = "hi\nmy\nname\nis\n\nhello";
    ASSERT_TRUE(s4.GetLine(line));
    ASSERT_STREQ(line.Str(), "hi");
    ASSERT_TRUE(s4.GetLine(line));
    ASSERT_STREQ(line.Str(), "my");
    ASSERT_TRUE(s4.GetLine(line));
    ASSERT_STREQ(line.Str(), "name");
    ASSERT_TRUE(s4.GetLine(line));
    ASSERT_STREQ(line.Str(), "is");
    ASSERT_TRUE(s4.GetLine(line));
    ASSERT_STREQ(line.Str(), "");
    ASSERT_FALSE(s4.GetLine(line));
    ASSERT_STREQ(line.Str(), "hello");
}

UTEST(test_String, GetInt)
{
    // The string can be converted to int
    int value;
    String s4("2");
    ASSERT_TRUE(s4.GetInt(value));
    ASSERT_EQ(value, 2);
    s4 = "3.3";
    ASSERT_TRUE(s4.GetInt(value));
    ASSERT_EQ(value, 3);
    s4 = "5.6";
    ASSERT_TRUE(s4.GetInt(value));
    ASSERT_EQ(value, 5);
    s4 = "-10";
    ASSERT_TRUE(s4.GetInt(value));
    ASSERT_EQ(value, -10);
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
    ASSERT_EQ(value, 2.f);
    s4 = "3.3";
    ASSERT_TRUE(s4.GetFloat(value));
    ASSERT_EQ(value, 3.3f);
    s4 = "5.6";
    ASSERT_TRUE(s4.GetFloat(value));
    ASSERT_EQ(value, 5.6f);
    s4 = "-10";
    ASSERT_TRUE(s4.GetFloat(value));
    ASSERT_EQ(value, -10.f);
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
    ASSERT_STREQ(String::FromInt(2).Str(), "2");
    ASSERT_STREQ(String::FromInt(90000000).Str(), "90000000");
}

UTEST(test_String, FromFloat)
{
    // The string can be converted from float
    ASSERT_STREQ(String::FromFloat(2.f).Str(), "2.000000");
    ASSERT_STREQ(String::FromFloat(9.00143f).Str(), "9.001430");
}

UTEST(test_String, FromDouble)
{
    // The string can be converted from double
    ASSERT_STREQ(String::FromDouble(0.14159265358979323L).Str(), "0.14159265358979323");
    ASSERT_STREQ(String::FromDouble(0.04159245358979343L).Str(), "0.04159245358979343");
}

UTEST(test_String, FromLong)
{
    // The string can be converted from long
    ASSERT_STREQ(String::FromLong(2l).Str(), "2");
    ASSERT_STREQ(String::FromLong(90000000000000l).Str(), "90000000000000");
}