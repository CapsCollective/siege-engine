//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_TOKEN_H
#define SIEGE_ENGINE_TOKEN_H

#include <unordered_set>

#include "String.h"

// Define macros
#define REGISTER_TOKEN(name)                                                            \
    static const Siege::TokenRegisterer CONCAT_SYMBOL(_tok_reg_, name)(TOSTRING(name)); \
    static const Siege::Token CONCAT_SYMBOL(TOKEN_, name)(TOSTRING(name))

namespace Siege
{
/**
 * The Token class provides a tokenised strings type along utility functions for looking up and
 * creating new string tokens.
 *
 * The Tokens wrap a c-string pointer to a string entry in a global unordered_set, populated with
 * valid, canonical token names. Tokens use their pointer to the canonical set as a means of
 * comparison, where a Token wrapping a null pointer is considered invalid.
 */
class Token
{
public:

    // 'Structors

    /**
     * Zero-param constructor for initialising empty Strings
     */
    Token();

    /**
     * C-string constructor for initialising by search of the
     * canonical set for the requested Token
     * @param name - the c-string to search
     */
    explicit Token(const char* name);

    // Operator overloads

    /**
     * Equality operator overload for checking equality with another
     * @note tokens are compared on the basis of their wrapped pointer
     * @param rhs - the Token to check
     * @return true if the Tokens are equal, false otherwise
     */
    bool operator==(Token rhs) const;

    /**
     * Inequality operator overload for checking inequality with another
     * @param rhs - the Token to check
     * @return true if the Tokens are not equal, false otherwise
     */
    bool operator!=(Token rhs) const;

    /**
     * Less-than comparison operator overload for Token identity comparison
     * @note this is used by standard template types requiring a key comparator
     *       and is implemented via a comparison of the wrapped pointer address
     * @param rhs - the Token to compare to
     * @return true if the Token's comparison index is lower, false otherwise
     */
    bool operator<(Token rhs) const;

    // Public methods

    /**
     * Gets the wrapped pointer of the Token
     * @return the Token c-string
     */
    const char* GetId() const;

    /**
     * Returns whether the Token is valid or not
     * @note Tokens are considered invalid if they wrap a nullptr, but those
     *       of pointers outside the canonical set are considered malformed
     * @return true if the Token is valid, false otherwise
     */
    bool IsValid() const;

    // Public static functions

    /**
     * Retrieves the canonical set of valid token strings
     * @return an unordered set of Strings
     */
    static std::unordered_set<String>& GetGlobalTokenRegister();

    /**
     * Finds the Token for a given String
     * @param name - the String to search for
     * @return the requested Token if found in the canonical set, else an invalid Token
     */
    static Token FindToken(const String& name);

    /**
     * Finds or registers a new Token for a given String
     * @param name - the String to search for
     * @return the requested Token
     */
    static Token FindOrRegisterToken(const String& name);

private:

    // Private static functions

    /**
     * Finds the wrapped pointer for a given String
     * @param name - the String to search for
     * @return the Token c-string
     */
    static const char* FindTokenId(const String& name);

    // Private fields

    /**
     * Wrapped pointer to the start of the canonical String
     */
    const char* tokenId;
};

/**
 * An empty struct for static member registration of Tokens
 */
struct TokenRegisterer
{
    // 'Structors

    /**
     * Constructor for Token registration that registers provided
     * string tokens at initialisation time
     * @note This is supposedly 1B in size per-instantiation
     * @param name - the name of the requested Token
     */
    explicit TokenRegisterer(const char* name);
};
} // namespace Siege

#endif // SIEGE_ENGINE_TOKEN_H
