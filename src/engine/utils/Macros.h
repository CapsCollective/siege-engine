#ifndef A_DARK_DISCOMFORT_MACROS_H
#define A_DARK_DISCOMFORT_MACROS_H

// OUT parameter macro
#define OUT

// Concatenation macros
#define CONCAT(A, B) A##B
#define CONCAT_SYMBOL(A, B) CONCAT(A, B)

// Stringification macros
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#endif //A_DARK_DISCOMFORT_MACROS_H
