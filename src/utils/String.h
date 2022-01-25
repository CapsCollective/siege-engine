#ifndef A_DARK_DISCOMFORT_STRING_H
#define A_DARK_DISCOMFORT_STRING_H

#include <string>
#include <utility>
#include <vector>

#include "Macros.h"

/**
 * The String class provides a simple alternative to std::string, that simply wraps a c-string and
 * bundles in greater manipulation functionality.
 */
class String
{
public:

    // 'Structors

    /**
     * Zero-param constructor for initialising empty Strings
     */
    String();

    /**
     * Copy constructor for initialising Strings from others
     * @param string - the String to copy
     */
    String(const String& string);

    /**
     * Move constructor for initialising Strings from others
     * @param string - the String to move
     */
    String(String&& string) noexcept;

    /**
     * C-string constructor for initialising by copy
     * @param string - the c-string to copy
     */
    String(const char* string);

    /**
     * Destructor for cleaning up String data
     */
    ~String();

    // Operator overloads

    /**
     * Assignment operator overload for assigning from others
     * @param rhs - the String to assign
     * @return a reference to the original String object
     */
    String& operator=(const String& rhs);

    /**
     * Move assignment operator overload for assigning from others
     * @param rhs - the String to move
     * @return a reference to the original String object
     */
    String& operator=(String&& rhs) noexcept;

    /**
     * C-string assignment operator overload for assigning by copy
     * @param rhs - the c-string to assign
     * @return a reference to the original String object
     */
    String& operator=(const char* rhs);

    /**
     * Equality operator overload for checking equality with another
     * @param rhs - the String to check
     * @return true if the Strings are equal, false otherwise
     */
    bool operator==(const String& rhs) const;

    /**
     * C-string equality operator overload for checking equality
     * @param rhs - the c-string to check
     * @return true if the strings are equal, false otherwise
     */
    bool operator==(const char* rhs) const;

    /**
     * Inequality operator overload for checking inequality with another
     * @param rhs - the String to check
     * @return true if the Strings are not equal, false otherwise
     */
    bool operator!=(const String& rhs) const;

    /**
     * C-string inequality operator overload for checking inequality
     * @param rhs - the c-string to check
     * @return true if the strings are not equal, false otherwise
     */
    bool operator!=(const char* rhs) const;

    /**
     * Addition operator overload for appending other Strings
     * @param rhs - the String to append
     * @return a new String object with the appended value
     */
    String operator+(const String& rhs) const;

    /**
     * C-string addition operator overload for appending c-strings
     * @param rhs - the c-string to append
     * @return a new String object with the appended value
     */
    String operator+(const char* rhs) const;

    /**
     * Addition compound assignment operator overload for appending other Strings
     * @param rhs - the String to append
     * @return a reference to the original String object
     */
    String& operator+=(const String& rhs);

    /**
     * C-string addition compound assignment operator overload for appending other c-strings
     * @param rhs - the c-string to append
     * @return a reference to the original String object
     */
    String& operator+=(const char* rhs);

    /**
     * Indexing operator overload for obtaining reference to characters within the String
     * @note this method supports negative indexing for reversed lookup
     * @warning attempting to access a character outside the String's range has undefined results
     * @param index - the index of the desired character
     * @return a reference to the indexed character
     */
    char& operator[](int index);

    // Conversion overloads

    /**
     * Boolean conversion operator overload for checking emptiness
     * @return false if the string is empty, true otherwise
     */
    operator bool() const;

    /**
     * C-string conversion operator overload for getting the internal c-string representation
     * @return the String's c-string data
     */
    operator const char*() const;

    /**
     * std::string conversion operator overload for getting the String as a std::string
     * @return an std::string copied from the String
     */
    operator std::string() const;

    // State methods

    /**
     * Returns whether the String is empty or not
     * @return true if the String is empty, false otherwise
     */
    bool IsEmpty() const;

    /**
     * Gets the length of the String up to it's first null terminating character ('\0')
     * @return the length of the String
     */
    size_t Size() const;

    /**
     * Gets the internal c-string representation
     * @return the String's c-string data
     */
    const char* Str() const;

    // Access & search methods

    /**
     * Gets character values within the String by index with bounds checking
     * @note this method supports negative indexing for reversed lookup
     * @param index - the index of the desired character
     * @return the indexed character if found, otherwise the string-termination character ('\0')
     */
    char At(int index) const;

    /**
     * Gets the index of the first found substring instance of the supplied String
     * @param substring - the substring to search for
     * @return the index of the beginning of the substring if found, else -1
     */
    size_t Find(const String& substring) const;

    /**
     * Gets the index of the first found substring instance of the supplied c-string
     * @param substring - the substring to search for
     * @return the index of the beginning of the substring if found, else -1
     */
    size_t Find(const char* substring) const;

    /**
     * Gets the index of the first found instance of the supplied character
     * @param substring - the character to search for
     * @return the index of the character if found, else -1
     */
    size_t Find(const char& character) const;

    /**
     * Splits the String by a provided list of delimiter characters
     * @param delimiters - a c-string of delimiter characters to split by (e.g. ",:-")
     * @return a vector of Strings resulting from the split
     */
    std::vector<String> Split(const char* delimiters) const;

    /**
     * Gets a substring of the String based on position and length
     * @note this method supports negative indexing for reversed lookup
     * @warning if either parameter is found to be invalid (falling out of bounds), the returned
     *          String will be empty
     * @param startPos - the starting index of the desired substring, defaults to 0
     * @param length - the length of the desired substring, defaults to length of String
     * @return the generated substring as a new String object
     */
    String SubString(int startPos = 0, size_t length = -1) const;

    // Modifier methods

    /**
     * Clears the content of the String leaving it empty
     */
    void Clear();

    /**
     * Appends the value of another String
     * @param string - the String to append
     */
    void Append(const String& string);

    /**
     * Appends the value of a c-string to the String
     * @param string - the c-string to append
     */
    void Append(const char* string);

    /**
     * Prepends the value of another String
     * @param string - the String to prepend
     */
    void Prepend(const String& string);

    /**
     * Prepends the value of a c-string to the String
     * @param string - the c-string to prepend
     */
    void Prepend(const char* string);

    /**
     * Erases portions of the String based on position and length
     * @note this method supports negative indexing for reversed lookup
     * @warning if either parameter is found to be invalid (falling out of bounds), the String
     *          will be assigned as entirely empty
     * @param startPos - the starting index of the desired erasure, defaults to 0
     * @param length - the length of the desired erasure, defaults to length of String
     */
    void Erase(int startPos = 0, size_t length = -1);

    /**
     * Swaps the value of the String with another
     * @param string - the string to swap with
     */
    void Swap(String& string);

    /**
     * Inserts a give c-string into the String at a given position
     * @note this method supports negative indexing for reversed lookup
     * @warning if either parameter is found to be invalid (falling out of bounds, null pointer),
     *          the String will not be modified
     * @param pos - the index to insert the provided string
     * @param string - the string to insert
     */
    void Insert(int pos, const char* string);

    /**
     * Replaces a specified substring with another
     * @param toReplace - the substring to replace
     * @param replacement - the string to replace the substring with
     * @return true if the replacement was found, false otherwise
     */
    bool Replace(const char* toReplace, const char* replacement);

    /**
     * Uppercases all characters in the String
     */
    void ToUpper();

    /**
     * Lowercases all characters in the String
     */
    void ToLower();

    /**
     * Extracts up to the next new line character ('\n') of the String
     * @param line - a String to populate with the line's content
     * @return true if there is still string content to be extracted, false otherwise
     */
    bool GetLine(OUT String& line);

    /**
     * Formats the String using standard printf formatting rules
     * @tparam P - the variadic type of the method's arguments
     * @param params - a variable number of arguments accepted by printf for formatting
     */
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

    /**
     * Private helper method wrapping basic assignment operations
     * @param string - the c-string to assign
     */
    void Assign(const char* string);

    // Private fields

    /**
     * Internal c-string representation
     */
    char* str;
};

// Reversed operator overloads

/**
 * Reversed c-string equality operator overload for checking equality
 * @param lhs - the first string to compare
 * @param rhs - the second string to compare
 * @return true if the strings are equal, false otherwise
 */
bool operator==(const char* lhs, const String& rhs);

/**
 * Reversed c-string inequality operator overload for checking inequality
 * @param lhs - the first string to compare
 * @param rhs - the second string to compare
 * @return true if the strings are not equal, false otherwise
 */
bool operator!=(const char* lhs, const String& rhs);

/**
 * Reversed c-string addition operator overload for appending strings
 * @param lhs - the string to append to
 * @param rhs - the string to append
 * @return a new string object with the appended value
 */
String operator+(const char* lhs, const String& rhs);

#endif // A_DARK_DISCOMFORT_STRING_H
