#ifndef __SIPHASH24_H__
#define __SIPHASH24_H__

#include <stdint.h>
#include <stddef.h>

#define ROTL64(x, b) (uint64_t)(((x) << (b)) | ((x) >> (64 - (b))))
#define SIPROUND \
    do { \
        v0 += v1; v1 = ROTL64(v1, 13); v1 ^= v0; v0 = ROTL64(v0, 32); \
        v2 += v3; v3 = ROTL64(v3, 16); v3 ^= v2; \
        v0 += v3; v3 = ROTL64(v3, 21); v3 ^= v0; \
        v2 += v1; v1 = ROTL64(v1, 17); v1 ^= v2; v2 = ROTL64(v2, 32); \
    } while (0)


static uint64_t siphash24(const uint8_t *in, size_t inlen, const uint64_t key[2]) {
    uint64_t v0 = 0x736f6d6570736575ULL ^ key[0];
    uint64_t v1 = 0x646f72616d617461ULL ^ key[1];
    uint64_t v2 = 0x6c7967656e657261ULL ^ key[0];
    uint64_t v3 = 0x7465646279746573ULL ^ key[1];

    uint64_t k0 = key[0];
    uint64_t k1 = key[1];

    const uint8_t *end = in + inlen - (inlen % 8);
    int left = inlen & 7;
    uint64_t b = ((uint64_t)inlen) << 56;

    // Process input in 64-bit blocks
    for (const uint8_t *ptr = in; ptr != end; ptr += 8) {
        uint64_t m;
        // Little-endian load
        m = (uint64_t)ptr[0] | ((uint64_t)ptr[1] << 8) |
            ((uint64_t)ptr[2] << 16) | ((uint64_t)ptr[3] << 24) |
            ((uint64_t)ptr[4] << 32) | ((uint64_t)ptr[5] << 40) |
            ((uint64_t)ptr[6] << 48) | ((uint64_t)ptr[7] << 56);

        v3 ^= m;
        SIPROUND;
        SIPROUND;
        v0 ^= m;
    }

    // Process remaining tail bytes
    switch (left) {
        case 7: b |= ((uint64_t)in[6]) << 48;
        case 6: b |= ((uint64_t)in[5]) << 40;
        case 5: b |= ((uint64_t)in[4]) << 32;
        case 4: b |= ((uint64_t)in[3]) << 24;
        case 3: b |= ((uint64_t)in[2]) << 16;
        case 2: b |= ((uint64_t)in[1]) << 8;
        case 1: b |= ((uint64_t)in[0]);
        case 0: break;
    }

    v3 ^= b;
    SIPROUND;
    SIPROUND;
    v0 ^= b;

    v2 ^= 0xff;

    // 4 finalization rounds
    SIPROUND;
    SIPROUND;
    SIPROUND;
    SIPROUND;

    return v0 ^ v1 ^ v2 ^ v3;
}

#undef SIPROUND
#undef ROTL64

#endif
