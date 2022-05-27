#include "Hash.h"

namespace SnekVk::Utils
{
    // Constexpr implementation and helpers
    uint32_t crc32_impl(const uint8_t* p, size_t len, uint32_t crc) {
        return len ?
               crc32_impl(p+1,len-1,(crc>>8)^crc_table[(crc&0xFF)^*p])
                   : crc;
    }

    uint32_t crc32(const uint8_t* data, size_t length) {
        return ~crc32_impl(data, length, ~0);
    }

    size_t strlen_c(const char* str) {
        return *str ? 1+strlen_c(str+1) : 0;
    }

    StringId WSID(const char* str) {
        return crc32((uint8_t*)str, strlen_c(str));
    }
}
