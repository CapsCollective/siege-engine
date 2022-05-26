#pragma once

#include <functional>
#include <cstring>
#include <cstdint>

namespace SnekVk::Utils
{
    // from: https://stackoverflow.com/a/57595105
    template <typename T, typename... Rest>
    void HashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
        seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        (HashCombine(seed, rest), ...);
    };

    typedef uint32_t StringId;

    // CRC hash generation
    template <unsigned c, int k = 8>
    struct f : f<((c & 1) ? 0xedb88320 : 0) ^ (c >> 1), k - 1> {};
    template <unsigned c> struct f<c, 0>{enum {value = c};};

    #define A(x) B(x) B(x + 128)
    #define B(x) C(x) C(x +  64)
    #define C(x) D(x) D(x +  32)
    #define D(x) E(x) E(x +  16)
    #define E(x) F(x) F(x +   8)
    #define F(x) G(x) G(x +   4)
    #define G(x) H(x) H(x +   2)
    #define H(x) I(x) I(x +   1)
    #define I(x) f<x>::value ,

    constexpr unsigned crc_table[] = { A(0) };

    // Constexpr implementation and helpers
    uint32_t crc32_impl(const uint8_t* p, size_t len, uint32_t crc);

    uint32_t crc32(const uint8_t* data, size_t length);

    size_t strlen_c(const char* str);

    StringId WSID(const char* str);
}