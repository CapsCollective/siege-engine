//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "String.h"

#include <cstdlib>
#include <cstring>
#include <ostream>
#include <stdexcept>

namespace Siege
{
static inline char* Allocate(size_t len)
{
    return static_cast<char*>(malloc(len + 1));
}

static inline char* Reallocate(char* str, size_t len)
{
    void* ptr = realloc(str, len + 1);
    return static_cast<char*>(ptr ? ptr : str);
}

static inline unsigned int GetCapacity(size_t len)
{
    // Add the least significant bit to round up to the nearest even number,
    // remember to reapply the on-heap flag bit after setting to the field
    return len + (len & 1);
}

void String::Assign(const char* string)
{
    // Check for self-assignment and null pointers
    const char* data = Data();
    if (string == data) return;
    if (!string) string = "";

    // Get the incoming string length,
    // bail if it's longer than max length
    size_t len = strlen(string);
    if (len > MAX_SIZE) return;

    // Check whether the string is already on the heap
    if (onHeap)
    {
        // Free previous heap allocation and reallocate
        if (len > Capacity())
        {
            capacity = GetCapacity(len);
            str = Reallocate(str, capacity);
        }

        // Allocate the new data on the heap
        strcpy(str, string);
        size = len;
        onHeap = true;
    }
    else
    {
        if (len <= MAX_STACK_CAPACITY)
        {
            // Copy the new data to the stack buffer
            strcpy(buffer, string);
            buffer[len] = '\0';
            space = MAX_STACK_CAPACITY - len;
            onHeap = false;
        }
        else
        {
            // Allocate the new data on the heap
            capacity = GetCapacity(len);
            str = Allocate(capacity);
            strcpy(str, string);
            size = len;
            onHeap = true;
        }
    }
}

char* String::Data()
{
    return onHeap ? str : buffer;
}

const char* String::Data() const
{
    return onHeap ? str : buffer;
}

template<typename T>
static char* GetFromFormat(const char* format, T value)
{
    size_t len = snprintf(nullptr, 0, format, value);
    char* cstr = Allocate(len);
    sprintf(cstr, format, value);
    cstr[len] = '\0';
    return cstr;
}

String::String() : String("") {}

String::String(const String& string) : String(string.Data()) {}

String::String(String&& string) noexcept : memory()
{
    memcpy(memory, string.memory, MEMORY_SIZE);
    memset(string.memory, '\0', MEMORY_SIZE);
}

String::String(const char* string) : memory()
{
    Assign(string);
}

String::String(const char& character) : memory()
{
    char cstr[2] = {character, '\0'};
    Assign(cstr);
}

String::~String()
{
    if (OnHeap()) free(str);
}

String& String::operator=(const String& rhs)
{
    if (this != &rhs) Assign(rhs);
    return *this;
}

String& String::operator=(String&& rhs) noexcept
{
    memcpy(memory, rhs.memory, MEMORY_SIZE);
    memset(rhs.memory, '\0', MEMORY_SIZE);
    return *this;
}

String& String::operator=(const char* rhs)
{
    Assign(rhs);
    return *this;
}

bool String::operator==(const String& rhs) const
{
    return *this == rhs.Data();
}

bool String::operator==(const char* rhs) const
{
    return strcmp(Data(), rhs) == 0;
}

bool String::operator!=(const String& rhs) const
{
    return *this != rhs.Data();
}

bool String::operator!=(const char* rhs) const
{
    return !(*this == rhs);
}

bool String::operator<(const String& rhs) const
{
    return strcmp(Data(), rhs.Data()) > 0;
}

String String::operator+(const String& rhs) const
{
    return *this + (const char*) rhs.Data();
}

String String::operator+(const char* rhs) const
{
    const char* data = Data();
    size_t lhsLength = Size();
    size_t rhsLength = strlen(rhs);

    char cstr[lhsLength + rhsLength];
    strcpy(cstr, data);
    strcpy(cstr + lhsLength, rhs);

    return cstr;
}

String String::operator+(const char& rhs) const
{
    const char* data = Data();
    size_t lhsLength = Size();

    char cstr[lhsLength + 1];
    strcpy(cstr, data);
    cstr[lhsLength] = rhs;
    cstr[lhsLength + 1] = '\0';

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

String& String::operator+=(const char& rhs)
{
    Append(rhs);
    return *this;
}

char& String::operator[](int index)
{
    char* data = Data();
    return data[index < 0 ? Size() + index : index];
}

String::operator bool() const
{
    return !IsEmpty();
}

String::operator const char*() const
{
    return Data();
}

bool String::GetInt(int& value) const
{
    try
    {
        value = std::stoi(Data());
        return true;
    }
    catch (const std::invalid_argument& err)
    {}
    catch (const std::out_of_range& err)
    {}
    return false;
}

bool String::GetFloat(float& value) const
{
    try
    {
        value = std::stof(Data());
        return true;
    }
    catch (const std::invalid_argument& err)
    {}
    catch (const std::out_of_range& err)
    {}
    return false;
}

String String::FromInt(const int& value)
{
    return GetFromFormat("%d", value);
}

String String::FromFloat(const float& value)
{
    return GetFromFormat("%f", value);
}

String String::FromDouble(const double& value)
{
    return GetFromFormat("%0.17f", value);
}

String String::FromLong(const long& value)
{
    return GetFromFormat("%ld", value);
}

String String::FromSizeT(const size_t& value)
{
    return GetFromFormat("%lu", value);
}

bool String::OnHeap() const
{
    return onHeap;
}

size_t String::Capacity() const
{
    return onHeap ? capacity & 0b1111111111111110 : MAX_STACK_CAPACITY;
}

bool String::IsEmpty() const
{
    return Size() == 0;
}

size_t String::Size() const
{
    return onHeap ? size : MAX_STACK_CAPACITY - space;
}

const char* String::Str() const
{
    return Data();
}

char String::At(int index) const
{
    const char* data = Data();
    size_t len = Size();
    index = index < 0 ? (int) len + index : index;
    if (index < 0 || index >= len) return '\0';
    return data[index];
}

size_t String::Find(const String& substring, int startIdx) const
{
    return Find(substring.Data(), startIdx);
}

size_t String::Find(const char* substring, int startIdx) const
{
    const char* data = Data();
    int len = (int) Size();
    startIdx = startIdx < 0 ? len + startIdx : startIdx;
    const char* sstr = data + startIdx;
    const char* pos = strstr(sstr, substring);
    return pos != nullptr ? pos - sstr + startIdx : -1;
}

size_t String::Find(const char& character, int startIdx) const
{
    const char* data = Data();
    int len = (int) Size();
    startIdx = startIdx < 0 ? len + startIdx : startIdx;
    const char* sstr = data + startIdx;
    const char* pos = strchr(sstr, character);
    return pos != nullptr ? pos - sstr + startIdx : -1;
}

std::vector<String> String::Split(const char* delimiters) const
{
    const char* data = Data();
    size_t len = Size();
    if (len == 0) return {};

    // Copy over the internal string
    char string[len + 1];
    strcpy(string, data);

    // Iterate over the string while there is still a delimiter
    std::vector<String> strings;
    char* substr = strtok(string, delimiters);

    do strings.emplace_back(substr);
    while ((substr = strtok(nullptr, delimiters)) != nullptr);

    return strings;
}

std::vector<String> String::Split(const char& delimiter) const
{
    // Provide the method with c-string
    char delimiters[2] = {delimiter, '\0'};
    return Split(delimiters);
}

String String::SubString(int startPos, size_t length) const
{
    // Perform boundary checking and set defaults
    const char* data = Data();
    size_t len = Size();
    startPos = startPos < 0 ? (int) len + startPos : startPos;
    length = length == -1 ? len - startPos : length;
    if (startPos >= len || (startPos + length) > len) return nullptr;

    // Copy over the substring and return it
    char subString[length];
    strncpy(subString, data + startPos, length);
    subString[length] = '\0'; // Manually add the null-termination char
    return subString;
}

bool String::Reserve(size_t cap)
{
    // Ignore reservations below max stack capacity or less than current
    if (cap < Capacity() || cap > MAX_SIZE || cap <= MAX_STACK_CAPACITY) return false;
    cap = GetCapacity(cap);

    // Store previous values
    char* data = Data();
    size_t len = Size();
    char* newStr;
    if (onHeap) newStr = Reallocate(str, cap);
    else
    {
        // Move the data to the heap
        newStr = Allocate(cap);
        strcpy(newStr, data);
    }

    // Apply previous values
    str = newStr;
    capacity = cap;
    size = len;
    onHeap = true;
    return true;
}

bool String::Shrink()
{
    size_t cap = Capacity();
    size_t len = Size();
    if (cap == len || !onHeap) return false;

    char* data = Data();
    if (len <= MAX_STACK_CAPACITY)
    {
        char newStr[len + 1];
        strcpy(newStr, data);
        free(str);
        strcpy(buffer, newStr);
        space = MAX_STACK_CAPACITY - len;
        onHeap = false;
    }
    else
    {
        cap = GetCapacity(len);
        str = Reallocate(str, cap);
        capacity = cap;
        size = len;
        onHeap = true;
    }
    return true;
}

void String::Reset()
{
    if (OnHeap()) free(str);
    memset(memory, '\0', MEMORY_SIZE);
    space = MAX_STACK_CAPACITY;
}

void String::Clear()
{
    Assign("");
}

void String::Append(const String& string)
{
    Append(string.Data());
}

void String::Append(const char* string)
{
    const char* data = Data();
    size_t lhsLength = Size();
    size_t rhsLength = strlen(string);
    size_t fullLength = lhsLength + rhsLength;

    char cstr[fullLength];
    strcpy(cstr, data);
    strcpy(cstr + lhsLength, string);

    Assign(cstr);
}

void String::Append(const char& character)
{
    const char* data = Data();
    size_t lhsLength = Size();

    char cstr[lhsLength + 1];
    strcpy(cstr, data);
    cstr[lhsLength] = character;
    cstr[lhsLength + 1] = '\0';

    Assign(cstr);
}

void String::Prepend(const String& string)
{
    // Mirrors c-string implementation
    const char* data = Data();
    size_t lhsLength = string.Size();
    size_t rhsLength = Size();
    size_t fullLength = lhsLength + rhsLength;

    char cstr[fullLength];
    strcpy(cstr, string.Data());
    strcpy(cstr + lhsLength, data);

    Assign(cstr);
}

void String::Prepend(const char* string)
{
    const char* data = Data();
    size_t lhsLength = strlen(string);
    size_t rhsLength = Size();
    size_t fullLength = lhsLength + rhsLength;

    char cstr[fullLength];
    strcpy(cstr, string);
    strcpy(cstr + lhsLength, data);

    Assign(cstr);
}

char String::PopBack()
{
    char* data = Data();
    size_t len = Size();
    char character = data[len - 1];
    data[len - 1] = '\0';
    if (onHeap) size -= 1;
    return character;
}

void String::Erase(int startPos, size_t length)
{
    // Perform boundary checking and set defaults
    const char* data = Data();
    size_t len = Size();
    startPos = startPos < 0 ? (int) len + startPos : startPos;
    length = length == -1 ? len - startPos : length;
    if (startPos < 0 || (startPos + length) > len)
    {
        Assign(nullptr);
        return;
    }

    // Copy over the non-erased portions and assign it
    char newStr[len - length];
    strncpy(newStr, data, startPos);
    strcpy(newStr + startPos, data + startPos + length);
    Assign(newStr);
}

void String::Swap(String& string)
{
    char tmp[MEMORY_SIZE];
    memcpy(tmp, memory, MEMORY_SIZE);
    memcpy(memory, string.memory, MEMORY_SIZE);
    memcpy(string.memory, tmp, MEMORY_SIZE);
}

void String::Insert(int pos, const char* string)
{
    // Perform boundary and input checking
    const char* data = Data();
    size_t len = Size();
    pos = pos < 0 ? (int) len + pos + 1 : pos;
    if (!string || pos > len || pos < 0) return;
    size_t insLength = strlen(string);

    // Copy over the non-erased portions and assign it
    char newStr[len + insLength];
    strncpy(newStr, data, pos);
    strncpy(newStr + pos, string, insLength);
    strcpy(newStr + pos + insLength, data + pos);
    Assign(newStr);
}

bool String::Replace(const char* toReplace, const char* replacement)
{
    if (!toReplace || *toReplace == '\0') return false;

    // Try to find the position of the supplied substring
    const char* data = Data();
    size_t pos = Find(toReplace);
    if (pos == -1) return false;

    // Replace the substring at position
    size_t currentLen = Size();
    size_t toReplaceLen = strlen(toReplace);
    size_t replaceLen = strlen(replacement);
    size_t finalLen = (currentLen - toReplaceLen) + replaceLen;

    char newStr[finalLen];
    strncpy(newStr, data, pos);
    strncpy(newStr + pos, replacement, replaceLen);
    strcpy(newStr + pos + replaceLen, data + pos + toReplaceLen);

    // Assign the new string
    Assign(newStr);
    return true;
}

void String::ToUpper()
{
    char* data = Data();
    for (int i = 0; data[i]; ++i) data[i] = (char) toupper(data[i]);
}

void String::ToLower()
{
    char* data = Data();
    for (int i = 0; data[i]; ++i) data[i] = (char) tolower(data[i]);
}

bool String::GetLine(String& line)
{
    char* data = Data();
    char* pos = strchr(data, '\n');
    if (pos)
    {
        size_t newLine = pos - data;
        line = SubString(0, newLine);
        Erase(0, newLine + 1);
        return true;
    }
    else
    {
        line = data;
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
    size_t rhsLength = rhs.Size();

    char cstr[lhsLength + rhsLength];
    strcpy(cstr, lhs);
    strcpy(cstr + lhsLength, rhs.Str());

    return {cstr};
}

String operator+(const char& lhs, const String& rhs)
{
    size_t rhsLength = rhs.Size();

    char cstr[1 + rhsLength];
    cstr[0] = lhs;
    strcpy(cstr + 1, rhs.Str());

    return {cstr};
}

std::ostream& operator<<(std::ostream& os, const String& string)
{
    return os.write(string.Str(), (long) string.Size());
}
} // namespace Siege