#ifndef A_DARK_DISCOMFORT_STRING_H
#define A_DARK_DISCOMFORT_STRING_H

#include <string>
#include <utility>
#include <vector>

#include "Macros.h"

class String
{
public:

    // 'Structors

    String();

    String(const String& string);

    String(String&& string) noexcept;

    String(const char* string);

    ~String();

    // Operator overloads

    String& operator=(const String& rhs);
    String& operator=(String&& rhs) noexcept;
    String& operator=(const char* rhs);
    bool operator==(const String& rhs) const;

    bool operator==(const char* rhs) const;

    bool operator!=(const String& rhs) const;

    bool operator!=(const char* rhs) const;

    String operator+(const String& rhs) const;

    String operator+(const char* rhs) const;


    String& operator+=(const String& rhs);

    String& operator+=(const char* rhs);

    char& operator[](int index);

    // Conversion overloads

    operator bool() const;

    operator const char*() const;

    operator std::string() const;

    // State methods

    bool IsEmpty() const;

    size_t Size() const;


    const char* Str() const;

    // Access & search methods

    char At(int index) const;
    size_t Find(const String& substring) const;
    size_t Find(const char* substring) const;
    size_t Find(const char& character) const;

    std::vector<String> Split(const char* delimiters) const;

    String SubString(int startPos = 0, size_t length = -1) const;

    // Modifier methods
    void Clear();
    void Append(const String& string);

    void Append(const char* string);

    void Prepend(const String& string);

    void Prepend(const char* string);


    void Erase(int startPos = 0, size_t length = -1);


    void Swap(String& string);
    void Insert(int pos, const char* string);
    bool Replace(const char* toReplace, const char* replacement);
    void ToUpper();

    void ToLower();


    bool GetLine(OUT String& line);
    template<typename... P>
    void Format(P&&... params)
    {
        size_t formatLen = std::snprintf(nullptr, 0, str, std::forward<P>(params)...);
        char newStr[formatLen];
        std::sprintf(newStr, str, std::forward<P>(params)...);
        Assign(newStr);
    }

private:

    // Private methods

    void Assign(const char* string);

    // Private fields

    char* str;
};

// Reversed operator overloads

bool operator==(const char* lhs, const String& rhs);

bool operator!=(const char* lhs, const String& rhs);
String operator+(const char* lhs, const String& rhs);

#endif // A_DARK_DISCOMFORT_STRING_H
