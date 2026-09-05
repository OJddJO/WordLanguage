#ifndef __BYTES_H__
#define __BYTES_H__

#include <stdint.h>
#include <string.h>

#define bit_cast(T, X) \
    ((union{T a; typeof(X) b;}) {.b=(X)}.a)

static inline uint16_t fetch_u16_le(const uint8_t **ip) {
    uint16_t val;
    memcpy(&val, *ip, sizeof(val));
    *ip += sizeof(val);

    #if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        return __builtin_bswap16(val);
    #else
        return val;
    #endif
}

static inline uint32_t fetch_u32_le(const uint8_t **ip) {
    uint32_t val;
    memcpy(&val, *ip, sizeof(val));
    *ip += sizeof(val);

    #if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        return __builtin_bswap32(val);
    #else
        return val;
    #endif
}

static inline uint64_t fetch_u64_le(const uint8_t **ip) {
    uint64_t val;
    memcpy(&val, *ip, sizeof(val));
    *ip += sizeof(val);

    #if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        return __builtin_bswap64(val);
    #else
        return val;
    #endif
}

#endif
