#ifndef A_DARK_DISCOMFORT_STRING_H
#define A_DARK_DISCOMFORT_STRING_H

#include "utility"

class String
{
public:

    // 'Structors

    String();

    String(const String& rhs);

    String(const char* rhs);

    ~String();

    // Operator overloads

    bool operator ==(const char* rhs) const;

    bool operator !=(const char* rhs) const;

    String operator +(const char* rhs) const;

    String& operator =(const char* rhs);

    String& operator +=(const char* rhs);

    // Conversion overloads

    operator bool() const;

    operator const char*() const;

    operator std::string() const;

    // Public methods

    bool IsEmpty() const;

    void Clear();

    const char* AsChar() const;

private:

    // Private fields

    char* str;
};

// Reversed operator overloads

bool operator ==(const char* lhs, const String& rhs);

String operator +(const char* lhs, const String& rhs);

String operator +=(const char* lhs, const String& rhs);


#endif //A_DARK_DISCOMFORT_STRING_H
