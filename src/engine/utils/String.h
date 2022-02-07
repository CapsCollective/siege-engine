#ifndef A_DARK_DISCOMFORT_STRING_H
#define A_DARK_DISCOMFORT_STRING_H

#include <ostream>
#include <utility>
#include <vector>

#include "Macros.h"

/**
 * The String class provides a simple alternative to standard strings, that simply wraps a c-string
 * and bundles in greater manipulation functionality.
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
     * Character constructor for initialising by copy
     * @param string - the character to copy
     */
    String(const char& character);

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
     * Less-than comparison operator overload for String identity comparison
     * @note this is used by standard template types requiring a key comparator
     * @param rhs - the String to compare to
     * @return true if the String's comparison index is lower, false otherwise
     */
    bool operator<(const String& rhs) const;

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
     * Character addition operator overload for appending characters
     * @param rhs - the character to append
     * @return a new String object with the appended value
     */
    String operator+(const char& rhs) const;

    /**
     * Addition compound assignment operator overload for appending other Strings
     * @param rhs - the String to append
     * @return a reference to the original String object
     */
    String& operator+=(const String& rhs);

    /**
     * C-string addition compound assignment operator overload for appending c-strings
     * @param rhs - the c-string to append
     * @return a reference to the original String object
     */
    String& operator+=(const char* rhs);

    /**
     * Character addition compound assignment operator overload for prepending characters
     * @param rhs - the c-string to append
     * @return a reference to the original String object
     */
    String& operator+=(const char& rhs);

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

    // Conversion methods

    /**
     * Attempts to retrieve the String's value as an integer
     * @param value - the integer to populate
     * @return true if the conversion was successful, false otherwise
     */
    bool GetInt(OUT int& value) const;

    /**
     * Attempts to retrieve the String's value as a float
     * @param value - the float to populate
     * @return true if the conversion was successful, false otherwise
     */
    bool GetFloat(OUT float& value) const;

    /**
     * Conversion method for integer values
     * @param value - the integer value to convert
     */
    static String FromInt(const int& value);

    /**
     * Conversion method for float values
     * @param value - the float value to convert
     */
    static String FromFloat(const float& value);

    /**
     * Conversion method for double values
     * @param value - the double value to convert
     */
    static String FromDouble(const double& value);

    /**
     * Conversion method for long values
     * @param value - the long value to convert
     */
    static String FromLong(const long& value);

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
     * @note this method supports negative indexing for reversed lookup
     * @param substring - the substring to search for
     * @param startIdx - the starting index for the search, defaults to 0
     * @return the index of the beginning of the substring if found, else -1
     */
    size_t Find(const String& substring, int startIdx = 0) const;

    /**
     * Gets the index of the first found substring instance of the supplied c-string
     * @note this method supports negative indexing for reversed lookup
     * @param substring - the substring to search for
     * @param startIdx - the starting index for the search, defaults to 0
     * @return the index of the beginning of the substring if found, else -1
     */
    size_t Find(const char* substring, int startIdx = 0) const;

    /**
     * Gets the index of the first found instance of the supplied character
     * @note this method supports negative indexing for reversed lookup
     * @param substring - the character to search for
     * @param startIdx - the starting index for the search, defaults to 0
     * @return the index of the character if found, else -1
     */
    size_t Find(const char& character, int startIdx = 0) const;

    /**
     * Splits the String by a provided list of delimiter characters
     * @param delimiters - a c-string of delimiter characters to split by (e.g. ",:-")
     * @return a vector of Strings resulting from the split
     */
    std::vector<String> Split(const char* delimiters) const;

    /**
     * Splits the String by a delimiter character
     * @param delimiter - a character to use as the delimiter
     * @return a vector of Strings resulting from the split
     */
    std::vector<String> Split(const char& delimiter) const;

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
     * Appends the value of a character to the String
     * @param character - the character to append
     */
    void Append(const char& character);

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
     * Removes the character at
     * @return
     */
    char PopBack();

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
        size_t formatLen = snprintf(nullptr, 0, str, std::forward<P>(params)...);
        char newStr[formatLen];
        sprintf(newStr, str, std::forward<P>(params)...);
        Assign(newStr);
    }

    /**
     * Creates a formatted version of the String using standard printf formatting rules
     * @tparam P - the variadic type of the method's arguments
     * @param params - a variable number of arguments accepted by printf for formatting
     * @return a newly formatted String object
     */
    template<typename... P>
    String Formatted(P&&... params) const
    {
        String string(str);
        string.Format(std::forward<P>(params)...);
        return string;
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

// External operator overloads

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
 * Reversed c-string addition operator overload for prepending strings
 * @param lhs - the string to append to
 * @param rhs - the string to append
 * @return a new string object with the appended value
 */
String operator+(const char* lhs, const String& rhs);

/**
 * Reversed character addition operator overload for prepending characters
 * @param lhs - the character to append to
 * @param rhs - the string to append
 * @return a new string object with the appended value
 */
String operator+(const char& lhs, const String& rhs);

/**
 * std::ostream insertion operator overload for appending Strings
 * @param os - the std::ostream instance to write to
 * @param string - the String to write to the std::ostream
 * @return the formatted std::ostream instance
 */
std::ostream& operator<<(std::ostream& os, const String& string);

#endif // A_DARK_DISCOMFORT_STRING_H
