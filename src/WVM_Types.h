#ifndef __WVM_TYPES_H__
#define __WVM_TYPES_H__

#include <stdint.h>

#include "WVM_Arena.h"

typedef enum _WVM_Type : uint8_t {
    WVM_TYPE_NULL = 0,
    WVM_TYPE_INT,
    WVM_TYPE_FLOAT,
    WVM_TYPE_BOOL,
    WVM_TYPE_FN,
    WVM_TYPE_PTR,
} WVM_Type;

typedef struct _WVM_Value {
    WVM_Type type;
    uint8_t pad[7];
    union {
        bool                    b;      // Boolean
        int64_t                 i;      // Integer
        double                  f;      // Float
        struct _WVM_Function    *fn;    // Function
        void                    *ptr;   // Raw native C pointer, used for C modules
    } as;
} WVM_Value;

typedef WVM_Value (*CNativeFn)();

typedef struct _HeapHeader {
    WVM_Arena   *arena;
} HeapHeader;

typedef struct _WVM_Function {
    HeapHeader  hdr;

    enum _WVM_FuncType : uint8_t {
        FUNCTYPE_C,
        FUNCTYPE_W,
    } type;
    union {
        CNativeFn   cFunc;
        uint64_t    bytecodeOffset;
    };
} WFunction;

#endif
