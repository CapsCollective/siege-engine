//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#pragma once

#include <volk/volk.h>

#include <cstdint>
#include <iostream>

#include "utils/Array.h"
#include "utils/Hash.h"
#include "utils/StackArray.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <utils/mat/Mat4.h>

#include <glm/glm.hpp>

// Custom type declarations
typedef uint32_t u32;
typedef uint64_t u64;
typedef int32_t i32;
typedef size_t size;

#define EXIT_APP abort();

#define REPORT_ASSERT_FAILURE(expr, file, line, message)                                        \
    std::cout << "ASSERTION FAILURE: " << #expr << " in file: " << file << " on line: " << line \
              << std::endl;                                                                     \
    std::cout << "                        Message: " << message << std::endl;

// Custom assert macro
#define CC_ASSERT(expr, message)                                   \
    if (expr)                                                      \
    {}                                                             \
    else                                                           \
    {                                                              \
        REPORT_ASSERT_FAILURE(#expr, __FILE__, __LINE__, message); \
        EXIT_APP                                                   \
    }

#define OUT

#define INTERN_STR(str) WSID(str)

namespace Siege
{
struct CameraData
{
    Siege::Mat4 projectionMatrix;
    Siege::Mat4 viewMatrix;
};
} // namespace Siege