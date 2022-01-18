#ifndef A_DARK_DISCOMFORT_STRING_H
#define A_DARK_DISCOMFORT_STRING_H

#include <utility>
#include <string>
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

    bool operator ==(const String& rhs) const;

    bool operator ==(const char* rhs) const;

    bool operator !=(const String& rhs) const;

    bool operator !=(const char* rhs) const;

    String operator +(const String& rhs) const;

    String operator +(const char* rhs) const;

    String& operator =(const String& rhs);

    String& operator =(String&& rhs) noexcept;

    String& operator =(const char* rhs);

    String& operator +=(const String& rhs);

    String& operator +=(const char* rhs);

    char& operator [](int index);

    // Conversion overloads

    operator bool() const;

    operator const char*() const;

    operator std::string() const;

    // Public methods

    bool IsEmpty() const;

    size_t Size() const;

    const char* AsChar() const;

    char At(size_t index) const;

    size_t Find(const String& string) const;

    size_t Find(const char* string) const;

    size_t Find(const char& character) const;

    std::vector<String> Split(const char* delimiters) const;

    String SubString(size_t startPos = 0, size_t length = -1) const;

    void Append(const String& string);

    void Append(const char* string);

    void Prepend(const String& string);

    void Prepend(const char* string);

    void Insert(size_t pos, const char* string);

    bool Replace(const char* toReplace, const char* replacement);

    void Erase(size_t startPos = 0, size_t length = -1);

    void Swap(String& string);

    bool GetLine(OUT String& line);

    void ToUpper();

    void ToLower();

    void Clear();

    template <typename ...P>
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

bool operator ==(const char* lhs, const String& rhs);

String operator +(const char* lhs, const String& rhs);


#endif //A_DARK_DISCOMFORT_STRING_H
