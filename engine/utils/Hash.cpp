//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Hash.h"

namespace Siege::Utils
{
// Constexpr implementation and helpers
uint32_t Crc32Impl(const uint8_t* p, size_t len, uint32_t crc)
{
    return len ? Crc32Impl(p + 1, len - 1, (crc >> 8) ^ crc_table[(crc & 0xFF) ^ *p]) : crc;
}

uint32_t Crc32(const uint8_t* data, size_t length)
{
    return ~Crc32Impl(data, length, ~0);
}

size_t StrlenC(const char* str)
{
    return *str ? 1 + StrlenC(str + 1) : 0;
}

StringId WSID(const char* str)
{
    return Crc32((uint8_t*) str, StrlenC(str));
}
} // namespace Siege::Utils
