//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_CORE_H
#define SIEGE_ENGINE_CORE_H

#include <utils/Hash.h>
#include <volk/volk.h>

#include <cstdint>
#include <iostream>

#include "utils/Array.h"
#include "utils/StackArray.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// Custom type declarations
typedef uint32_t u32;
typedef uint64_t u64;
typedef int32_t i32;
typedef size_t size;

#define EXIT_APP abort()
#define REPORT_ASSERT_FAILURE(expr, file, line, message)                                        \
    std::cout << "ASSERTION FAILURE: " << #expr << " in file: " << file << " on line: " << line \
              << "\n                        Message: " << message << std::endl

#define CC_ASSERT(expr, msg)                                                \
    expr || (REPORT_ASSERT_FAILURE(expr, __FILE__, __LINE__, msg) && []() { \
        EXIT_APP;                                                           \
        return true;                                                        \
    }())

#define OUT

namespace Siege
{
struct CameraData
{
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
};
} // namespace Siege

#endif
