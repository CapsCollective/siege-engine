//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Hash.h"

namespace Siege::Hash
{
// Constexpr implementation and helpers
uint32_t crc32_impl(const uint8_t* p, size_t len, uint32_t crc)
{
    return len ? crc32_impl(p + 1, len - 1, (crc >> 8) ^ crc_table[(crc & 0xFF) ^ *p]) : crc;
}

uint32_t crc32(const uint8_t* data, size_t length)
{
    return ~crc32_impl(data, length, ~0);
}

size_t strlen_c(const char* str)
{
    return *str ? 1 + strlen_c(str + 1) : 0;
}

StringId WSID(const char* str)
{
    return crc32((uint8_t*) str, strlen_c(str));
}

} // namespace Siege::Hash
