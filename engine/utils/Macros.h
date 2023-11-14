//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MACROS_H
#define SIEGE_ENGINE_MACROS_H

// Reserved character macros
#define _COMMA ,
#define _SEMICOLON ;

// OUT parameter macro
#define OUT

// Concatenation macros
#define CONCAT(A, B) A##B
#define CONCAT_SYMBOL(A, B) CONCAT(A, B)

// Stringification macros
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#endif // SIEGE_ENGINE_MACROS_H
