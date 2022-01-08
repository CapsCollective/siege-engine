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

    bool operator ==(const String& rhs) const;

    bool operator ==(const char* rhs) const;

    bool operator !=(const String& rhs) const;

    bool operator !=(const char* rhs) const;

    String operator +(const String& rhs) const;

    String operator +(const char* rhs) const;

    String& operator =(const String& rhs);

    String& operator =(const char* rhs);

    String& operator +=(const String& rhs);

    String& operator +=(const char* rhs);

    char& operator [](int index);

    // Conversion overloads

    operator bool() const;

    operator const char*() const;

    operator std::string() const;

    // Public methods

    // TODO add string methods for:
    // - capacity and reservations (optimise mallocs)
    // - searching (front to back, match, etc.)
    // - erasing (front to back, match, etc.)
    // - getline
    // - iterators if necessary

    // TODO copy over methods from StringHelpers
    // - vector conversion
    // - split
    // - lowercase/uppercase
    // - format

    bool IsEmpty() const;

    size_t Size() const;

    size_t Find(const String& string) const;

    size_t Find(const char* string) const;

    bool Replace(const char* toReplace, const char* replacement);

    void Append(const String& string);

    void Append(const char* string);

    char At(size_t index) const;

    void Clear();

    const char* AsChar() const;

private:

    // Private fields

    char* str;
};

// Reversed operator overloads

bool operator ==(const char* lhs, const String& rhs);

String operator +(const char* lhs, const String& rhs);


#endif //A_DARK_DISCOMFORT_STRING_H
