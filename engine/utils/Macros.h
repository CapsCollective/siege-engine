//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
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

#define EXIT_APP abort();

#define REPORT_ASSERT_FAILURE(expr, file, line, message)                       \
    CC_LOG_ERROR("ASSERTION FAILURE: {} in file: {} on line: {}\nMessage: {}", \
                 #expr,                                                        \
                 file,                                                         \
                 line,                                                         \
                 message);

// Custom assert macro
#define CC_ASSERT(expr, message)                                   \
    if (expr)                                                      \
    {}                                                             \
    else                                                           \
    {                                                              \
        REPORT_ASSERT_FAILURE(#expr, __FILE__, __LINE__, message); \
        EXIT_APP                                                   \
    }

#endif // SIEGE_ENGINE_MACROS_H
