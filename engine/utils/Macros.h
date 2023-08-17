//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
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

// Macro for declaring a binary operator
#define DECL_BINARY_OP_NO_IMPL(op, returnType, lhsType, rhsType) \
    returnType operator op(lhsType lhs, rhsType rhs) _SEMICOLON

#endif // SIEGE_ENGINE_MACROS_H
