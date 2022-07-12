//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_STRING_H
#define SIEGE_ENGINE_STRING_H

#include <ostream>
#include <utility>
#include <vector>

#include "Hash.h"
#include "Macros.h"

/**
 * The String class provides an alternative to standard strings, bundling in greater manipulation
 * functionality and providing a relatively compact implementation of small String optimisation.
 *
 * Strings should take up to 16 bytes of stack memory, 15 bytes of which can be used to store small
 * Strings in a buffer NOT including the null termination character, meaning that it can effectively
 * store up to 15 characters on the stack before allocating memory.
 *                                                                                  inverse capacity
 *               string buffer (15B)                                                & heap flag (1B)
 * Small String [--------------------------------------------------------------------------|----]
 *
 *               string pointer (8B)                size (4B)       capacity (15b)    heap flag (1b)
 * Large String [---------------------------------------|-------------------|-----------------|-]
 *
 * @note due to its compact form factor, the longest safely representable String is 65,534
 *       characters in length, and will always be an even number as the capacity value does not make
 *       use of its least significant bit
 */
class String
{
public:

    // Public static members

    static constexpr unsigned int MEMORY_SIZE = 16u;
    static constexpr unsigned int MAX_STACK_CAPACITY = 15u;
    static constexpr unsigned int MAX_SIZE = 0b1111111111111110;

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
    String(char character);

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
    String operator+(char rhs) const;

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
    String& operator+=(char rhs);

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
     * @return the newly created String
     */
    static String FromInt(int value);

    /**
     * Conversion method for float values
     * @param value - the float value to convert
     * @return the newly created String
     */
    static String FromFloat(float value);

    /**
     * Conversion method for double values
     * @param value - the double value to convert
     * @return the newly created String
     */
    static String FromDouble(double value);

    /**
     * Conversion method for long values
     * @param value - the long value to convert
     * @return the newly created String
     */
    static String FromLong(long value);

    /**
     * Conversion method for 32-bit unsigned integer values
     * @param value - the uint32_t value to convert
     * @return the newly created String
     */
    static String FromU32(uint32_t value);

    /**
     * Conversion method for 64-bit unsigned integer values
     * @param value - the uint64_t value to convert
     * @return the newly created String
     */
    static String FromU64(uint64_t value);

    // State methods

    /**
     * Returns whether the String is allocated on the heap or not
     * @return true if the String's content is being stored on the heap, false otherwise
     */
    bool OnHeap() const;

    /**
     * Gets the capacity of the String based on its allocated size
     * @return the capacity reserved for the string
     */
    size_t Capacity() const;

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
    size_t Find(char character, int startIdx = 0) const;

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
    std::vector<String> Split(char delimiter) const;

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
     * Reserves String capacity greater than its current allocation
     * @note this method has no effect on small Strings
     * @param capacity - the capacity to reserve for
     * @return true if reservation was successful, false otherwise
     */
    bool Reserve(size_t capacity);

    /**
     * Shrinks the String capacity down to fit its current size, deallocating
     * large Strings if it can fit a small String on the stack
     * @return true if the capacity was able to shrink, false otherwise
     */
    bool Shrink();

    /**
     * Resets the String content to empty, freeing any allocated memory it holds
     * @return true if the capacity was able to shrink, false otherwise
     */
    void Reset();

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
    void Append(char character);

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
        const char* data = Data();
        size_t formatLen = snprintf(nullptr, 0, data, std::forward<P>(params)...);
        char newStr[formatLen];
        sprintf(newStr, data, std::forward<P>(params)...);
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
        String string(Data());
        string.Format(std::forward<P>(params)...);
        return string;
    }

private:

    // Private methods

    /**
     * Private helper method wrapping assignment operations; handles the bulk of small
     * string optimisation logic
     * @note that most internal modification logic should be routed through this method
     *       to avoid unexpected behaviour
     * @param string - the c-string to assign
     */
    void Assign(const char* string);

    /**
     * Private helper method to access the String's data pointer
     * @return a pointer to the String's data
     */
    char* Data();

    /**
     * Private helper method to access the String's const data pointer
     * @return a const pointer to the String's data
     */
    const char* Data() const;

    // Private fields

    /**
     * Internal representation union for the Strings
     */
    union
    {
        /**
         * Heap representation members
         */
        struct
        {
            /**
             * Internal c-string pointer and access point for
             * both stack and heap representation
             */
            char* str;

            /**
             * String size value
             */
            unsigned int size;

            /**
             * Storage capacity value where least significant bit
             * is ignored in capacity calculations as it is used
             * as the on-heap flag, meaning that the capacity will
             * always be an even number.
             *
             * @note this limits the longest safely representable
             *       String to 65,534 characters in length
             */
            unsigned int capacity;
        };

        /**
         * Stack representation members
         */
        struct
        {
            /**
             * Character data buffer for stack representation
             */
            char buffer[MAX_STACK_CAPACITY];

            /**
             * Dual purpose stack storage space available value
             * and heap storage flag bit
             *
             * @note when the available stack space reaches zero,
             *       the field implicitly functions as the
             *       c-string's null termination character
             */
            unsigned char space:7, onHeap:1;
        };

        /**
         * Total memory representation for the union
         */
        char memory[MEMORY_SIZE];
    };
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
String operator+(char lhs, const String& rhs);

/**
 * std::ostream insertion operator overload for appending Strings
 * @param os - the std::ostream instance to write to
 * @param string - the String to write to the std::ostream
 * @return the formatted std::ostream instance
 */
std::ostream& operator<<(std::ostream& os, const String& string);

// Define a hash function for the string so that it can be stored in a map or ordered set
// TODO(Aryeh): Maybe replace this with a custom implementation when the time comes.
template<>
struct std::hash<String>
{
    size_t operator()(const String& string) const
    {
        size_t seed = 0;
        Siege::Hash::HashCombine(seed,
                           string.At(0),
                           string.At(string.Size()),
                           string.Size(),
                           string.Capacity());
        return seed;
    };
};

#endif // SIEGE_ENGINE_STRING_H
