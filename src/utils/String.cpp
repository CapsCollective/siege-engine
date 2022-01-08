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
    str(static_cast<char*>(malloc(strlen(rhs))))
{
    strcpy(str, rhs);
}

String::~String()
{
    if (IsEmpty()) free(str);
}

bool String::operator ==(const char* rhs) const
{
    return strcmp(str, rhs) == 0;
}

bool String::operator!=(const char* rhs) const
{
    return !(*this == rhs);
}

const char* String::AsChar() const
{
    return str;
}

String& String::operator =(const char* rhs)
{
    free(str);
    str = static_cast<char*>(malloc(strlen(rhs)));
    strcpy(str, rhs);
    return *this;
}

String &String::operator +=(const char* rhs)
{
    size_t lhsLength = strlen(str);
    size_t rhsLength = strlen(rhs);
    size_t fullLength = lhsLength + rhsLength;

    char cstr[fullLength];
    strcpy(cstr, str);
    strcpy(cstr + lhsLength, rhs);

    free(str);
    str = static_cast<char*>(malloc(fullLength));
    strcpy(str, cstr);

    return *this;
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