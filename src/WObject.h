#ifndef __WOBJECT_H__
#define __WOBJECT_H__

#include <stddef.h>
#include <stdint.h>

typedef struct _WFunction {
    size_t      numberOfArgs;
} WFunction;

typedef struct _WObject {
    uint64_t    refCount;
    union {
        WFunction   function;
    } as;
} WObject;

#endif
