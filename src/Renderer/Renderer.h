#pragma once

#include <volk.h>
#include <iostream>
#include <cstdint>

// Custom type declarations
typedef uint32_t u32;
typedef int32_t i32;
typedef size_t size;

#if (defined(_WIN32) || defined(_WIN64)) 
    #define EXIT_APP \
        system("pause"); \
        abort();
#else
    #define EXIT_APP abort();
#endif 

#define REPORT_ASSERT_FAILURE(expr, file, line, message) \
    std::cout << "SNEK ASSERTION FAILURE: " << #expr << " in file: " << file << " on line: "  << line << std::endl; \
    std::cout  << "                        Message: " << message << std::endl; 

// Custom assert macro
#define SNEK_ASSERT(expr, message) \
    if (expr) { } \
    else \
    { \
        REPORT_ASSERT_FAILURE(#expr, __FILE__, __LINE__, message); \
        EXIT_APP \
    } \

#define OUT

namespace SnekVk 
{

}