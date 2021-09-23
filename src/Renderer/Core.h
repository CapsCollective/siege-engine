#pragma once

#include "Utils/Array.h"
#include "Utils/Hash.h"

#include <volk/volk.h>
#include <iostream>
#include <cstdint>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// Custom type declarations
typedef uint32_t u32;
typedef uint64_t u64;
typedef int32_t i32;
typedef size_t size;

#define EXIT_APP abort();

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

#define INTERN_STR(str) Utils::WSID(str)

namespace SnekVk 
{

}