#ifndef __BYTES_H__
#define __BYTES_H__

#include <stdint.h>
#include <string.h>

static inline int16_t fetch_i16_le(const uint8_t **ip) {
    int16_t val;
    memcpy(&val, *ip, sizeof(val));
    *ip += sizeof(val);

    #if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        return __builtin_bswap16(val);
    #else
        return val;
    #endif
}

static inline int32_t fetch_i32_le(const uint8_t **ip) {
    int32_t val;
    memcpy(&val, *ip, sizeof(val));
    *ip += sizeof(val);

    #if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        return __builtin_bswap32(val);
    #else
        return val;
    #endif
}

static inline int64_t fetch_i64_le(const uint8_t **ip) {
    int64_t val;
    memcpy(&val, *ip, sizeof(val));
    *ip += sizeof(val);

    #if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        return __builtin_bswap64(val);
    #else
        return val;
    #endif
}

#endif
