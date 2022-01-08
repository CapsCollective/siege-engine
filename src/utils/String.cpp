#include <cstdlib>
#include <cstring>
#include <string>

#include "String.h"

String::String() :
    String("")
{}

String::String(const String &rhs) :
    String(rhs.AsChar())
{}

String::String(const char* rhs) :
    str(static_cast<char*>(malloc(strlen(rhs) + 1)))
{
    strcpy(str, rhs);
}

String::~String()
{
    // TODO can this be removed with assertions?
    if (IsEmpty()) free(str);
}

bool String::operator ==(const String& rhs) const
{
    return *this == rhs.str;
}

bool String::operator ==(const char* rhs) const
{
    return strcmp(str, rhs) == 0;
}

bool String::operator!=(const String& rhs) const
{
    return *this != rhs.str;
}

bool String::operator!=(const char* rhs) const
{
    return !(*this == rhs);
}

const char* String::AsChar() const
{
    return str;
}

String &String::operator =(const String& rhs)
{
    // Perform self-assignment check on internal pointer
    return *this = rhs.str;
}

String& String::operator =(const char* rhs)
{
    if (str == rhs) return *this;
    free(str);
    str = static_cast<char*>(malloc(strlen(rhs) + 1));
    strcpy(str, rhs);
    return *this;
}

String& String::operator +=(const String& rhs)
{
    Append(rhs);
    return *this;
}

String& String::operator +=(const char* rhs)
{
    Append(rhs);
    return *this;
}

String String::operator +(const String& rhs) const
{
    return *this + (const char*) rhs.str;
}

String String::operator +(const char* rhs) const
{
    size_t lhsLength = strlen(str);
    size_t rhsLength = strlen(rhs);

    char cstr[lhsLength + rhsLength];
    strcpy(cstr, str);
    strcpy(cstr + lhsLength, rhs);

    return cstr;
}

void String::Clear()
{
    *this = "";
}

String::operator bool() const
{
    return !IsEmpty();
}

String::operator const char*() const
{
    return str;
}

String::operator std::string() const
{
    return {str};
}

bool String::IsEmpty() const
{
    return str[0] == '\0';
}

size_t String::Size() const
{
    return strlen(str);
}

void String::Append(const String& string)
{
    Append(string.str);
}

void String::Append(const char* string)
{
    size_t lhsLength = strlen(str);
    size_t rhsLength = strlen(string);
    size_t fullLength = lhsLength + rhsLength;

    char cstr[fullLength];
    strcpy(cstr, str);
    strcpy(cstr + lhsLength, string);

    free(str);
    str = static_cast<char*>(malloc(fullLength + 1));
    strcpy(str, cstr);
}

char String::At(size_t index) const
{
    if (index < 0 || index >= Size()) return '\0';
    return str[index];
}

char& String::operator[](int index)
{
    return str[index];
}

size_t String::Find(const String& string) const
{
    return Find(string.str);
}

size_t String::Find(const char* string) const
{
    char* pos = strstr(str, string);
    return pos != nullptr ? pos - str : -1;
}

bool String::Replace(const char* toReplace, const char* replacement)
{
    if (!toReplace || *toReplace == '\0') return false;

    // Try to find the position of the supplied substring
    size_t pos = Find(toReplace);
    if (pos == -1) return false;

    // Replace the substring at position
    size_t currentLen = Size();
    size_t toReplaceLen = strlen(toReplace);
    size_t replaceLen = strlen(replacement);
    size_t finalLen = (currentLen - toReplaceLen) + replaceLen;

    char newStr[finalLen];
    strncpy(newStr, str, pos);
    strncpy(newStr + pos, replacement, replaceLen);
    strcpy(newStr + pos + replaceLen, str + pos + toReplaceLen);

    // Assign the new string
    *this = newStr;
    return true;
}

bool operator ==(const char* lhs, const String& rhs)
{
    return rhs == lhs;
}

String operator +(const char* lhs, const String& rhs)
{
    size_t lhsLength = strlen(lhs);
    size_t rhsLength = strlen(rhs.AsChar());

    char cstr[lhsLength + rhsLength];
    strcpy(cstr, lhs);
    strcpy(cstr + lhsLength, rhs.AsChar());

    return {cstr};
}

String operator +=(const char* lhs, const String& rhs)
{
    return lhs + rhs;
}