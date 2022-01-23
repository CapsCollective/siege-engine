#include "String.h"

#include <cstdlib>
#include <cstring>

static char* Allocate(const char* string)
{
    // Check for null pointers
    if (!string) string = "";

    // Allocate string for length and copy it over
    char* newStr = static_cast<char*>(malloc(strlen(string) + 1));
    strcpy(newStr, string);
    return newStr;
}

void String::Assign(const char* string)
{
    // Check for self-assignment
    if (str == string) return;

    // TODO add functionality for capacity and reservations (optimise alloc)
    // Free current internal string and assign the new one
    free(str);
    str = Allocate(string);
}

String::String() : String("") {}

String::String(const String& string) : String(string.str) {}

String::String(String&& string) noexcept : str(string.str)
{
    string.str = nullptr;
}

String::String(const char* string) : str(Allocate(string)) {}

String::~String()
{
    free(str);
}

String& String::operator=(const String& rhs)
{
    if (this != &rhs) Assign(rhs);
    return *this;
}

String& String::operator=(String&& rhs) noexcept
{
    str = rhs.str;
    rhs.str = nullptr;
    return *this;
}

String& String::operator=(const char* rhs)
{
    Assign(rhs);
    return *this;
}

bool String::operator==(const String& rhs) const
{
    return *this == rhs.str;
}

bool String::operator==(const char* rhs) const
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

String String::operator+(const String& rhs) const
{
    return *this + (const char*) rhs.str;
}

String String::operator+(const char* rhs) const
{
    size_t lhsLength = strlen(str);
    size_t rhsLength = strlen(rhs);

    char cstr[lhsLength + rhsLength];
    strcpy(cstr, str);
    strcpy(cstr + lhsLength, rhs);

    return cstr;
}

String& String::operator+=(const String& rhs)
{
    Append(rhs);
    return *this;
}

String& String::operator+=(const char* rhs)
{
    Append(rhs);
    return *this;
}

char& String::operator[](int index)
{
    return str[index < 0 ? strlen(str) + index : index];
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

const char* String::Str() const
{
    return str;
}

char String::At(int index) const
{
    index = index < 0 ? (int) Size() + index : index;
    if (index < 0 || index >= Size()) return '\0';
    return str[index];
}

size_t String::Find(const String& substring) const
{
    return Find(substring.str);
}

size_t String::Find(const char* substring) const
{
    char* pos = strstr(str, substring);
    return pos != nullptr ? pos - str : -1;
}

size_t String::Find(const char& character) const
{
    char* pos = strchr(str, character);
    return pos != nullptr ? pos - str : -1;
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
    do
    {
        strings.emplace_back(substr);
    } while ((substr = strtok(nullptr, delimiters)) != nullptr);
    return strings;
}

String String::SubString(int startPos, size_t length) const
{
    // Perform boundary checking and set defaults
    size_t strLen = Size();
    startPos = startPos < 0 ? (int) strLen + startPos : startPos;
    length = length == -1 ? strLen - startPos : length;
    if (startPos >= strLen || (startPos + length) > strLen) return nullptr;

    // Copy over the substring and return it
    char subString[length];
    strncpy(subString, str + startPos, length);
    subString[length] = '\0'; // Manually add the null-termination char
    return subString;
}

void String::Clear()
{
    Assign("");
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

void String::Erase(int startPos, size_t length)
{
    // Perform boundary checking and set defaults
    size_t strLen = Size();
    startPos = startPos < 0 ? (int) strLen + startPos : startPos;
    length = length == -1 ? strLen - startPos : length;
    if (startPos < 0 || (startPos + length) > strLen)
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

void String::Swap(String& string)
{
    char* tmp = str;
    str = string.str;
    string.str = tmp;
}

void String::Insert(int pos, const char* string)
{
    // Perform boundary and input checking
    size_t strLen = Size();
    pos = pos < 0 ? (int) strLen + pos + 1 : pos;
    if (!string || pos > strLen || pos < 0) return;
    size_t insLength = strlen(string);

    // Copy over the non-erased portions and assign it
    char newStr[strLen + insLength];
    strncpy(newStr, str, pos);
    strncpy(newStr + pos, string, insLength);
    strcpy(newStr + pos + insLength, str + pos);
    Assign(newStr);
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

void String::ToUpper()
{
    for (int i = 0; str[i]; ++i) str[i] = (char) toupper(str[i]);
}

void String::ToLower()
{
    for (int i = 0; str[i]; ++i) str[i] = (char) tolower(str[i]);
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

bool operator==(const char* lhs, const String& rhs)
{
    return rhs == lhs;
}

bool operator!=(const char* lhs, const String& rhs)
{
    return rhs != lhs;
}

String operator+(const char* lhs, const String& rhs)
{
    size_t lhsLength = strlen(lhs);
    size_t rhsLength = strlen(rhs.Str());

    char cstr[lhsLength + rhsLength];
    strcpy(cstr, lhs);
    strcpy(cstr + lhsLength, rhs.Str());

    return {cstr};
}