#include <cstdlib>
#include <cstring>
#include <vector>

#include "String.h"

static char* Allocate(const char* string)
{
    // Check for null pointers
    if (!string) string = "";

    // Allocate string for length and copy it over
    char* newStr = static_cast<char*>(malloc(strlen(string) + 1));
    strcpy(newStr, string);
    return newStr;
}

String::String() :
    String("")
{}

String::String(const String& string) :
    String(string.AsChar())
{}

String::String(const char* string) :
    str(Allocate(string))
{}

String::~String()
{
    // TODO can this be removed with assertions?
    if (IsEmpty()) free(str);
}

void String::Assign(const char* string)
{
    // Check for self-assignment
    if (str == string) return;

    // TODO optimise string malloc and free
    // Free current internal string and assign the new one
    free(str);
    str = Allocate(string);
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

String& String::operator =(const String& rhs)
{
    if (this != &rhs) Assign(rhs);
    return *this;
}

String& String::operator =(const char* rhs)
{
    Assign(rhs);
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
    Assign("");
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

    Assign(cstr);
}

void String::Prepend(const String& string)
{
    Prepend(string.str);
}

void String::Prepend(const char* string)
{
    size_t lhsLength = strlen(string);
    size_t rhsLength = strlen(str);
    size_t fullLength = lhsLength + rhsLength;

    char cstr[fullLength];
    strcpy(cstr, string);
    strcpy(cstr + lhsLength, str);

    Assign(cstr);
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

size_t String::Find(const char& character) const
{
    char* pos = strchr(str, character);
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
    Assign(newStr);
    return true;
}

std::vector<String> String::Split(const char* delimiters) const
{
    if (IsEmpty()) return {};

    // Copy over the internal string
    char string[strlen(str)];
    strcpy(string, str);

    // Iterate over the string while there is still a delimiter
    std::vector<String> strings;
    char* substr = strtok(string, delimiters);
    do {
        strings.emplace_back(substr);
    } while ((substr = strtok(nullptr, delimiters)) != nullptr);
    return strings;
}

void String::ToUpper()
{
    for (int i = 0; str[i]; ++i) str[i] = (char) toupper(str[i]);
}

void String::ToLower()
{
    for (int i = 0; str[i]; ++i) str[i] = (char) tolower(str[i]);
}

String String::SubString(size_t startPos, size_t length) const
{
    // Perform boundary checking and set defaults
    size_t strLen = Size();
    if (length == -1) length = strLen - startPos;
    if (startPos >= strLen || (startPos + length) > strLen) return nullptr;

    // Copy over the substring and return it
    char subString[length];
    strncpy(subString, str + startPos, length);
    subString[length] = '\0'; // Manually add the null-termination char
    return subString;
}

bool String::GetLine(String& line)
{
    char* pos = strchr(str, '\n');
    if (pos)
    {
        size_t newLine = pos - str;
        line = SubString(0, newLine);
        Erase(0, newLine + 1);
        return true;
    }
    else
    {
        line = str;
        Clear();
        return false;
    }
}

void String::Erase(size_t startPos, size_t length)
{
    // Perform boundary checking and set defaults
    size_t strLen = Size();
    if (length == -1) length = strLen - startPos;
    if (startPos >= strLen || (startPos + length) > strLen)
    {
        Assign(nullptr);
        return;
    }

    // Copy over the non-erased portions and assign it
    char newStr[strLen - length];
    strncpy(newStr, str, startPos);
    strcpy(newStr + startPos, str + startPos + length);
    Assign(newStr);
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