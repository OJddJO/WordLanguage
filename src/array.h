#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdint.h>

#define ARRAY_DEFAULT_SIZE  (1 << 7)
#define ARRAY_GROWTH_FACTOR 1.5

typedef struct _Array {
    uint64_t    size;
    uint64_t    count;
    void        **buf;
} Array;

#endif
