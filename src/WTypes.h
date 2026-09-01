#ifndef __WTYPES_H__
#define __WTYPES_H__

#include <stdint.h>

#include "hashmap.h"

typedef enum _WType : uint8_t {
    WTYPE_NULL = 0,
    WTYPE_INT,
    WTYPE_FLOAT,
    WTYPE_BOOL,
    WTYPE_OBJ,
    WTYPE_FN,
    WTYPE_STR,
    WTYPE_PTR,
} WType;

typedef struct _WValue {
    WType type;
    uint8_t pad[7];
    union {
        bool                b;      // Boolean
        int64_t             i;      // Integer
        double              f;      // Float
        struct _WFunction   *fn;    // Function
        struct _WString     *str;   // String
        struct _WObject     *o;     // Object
        void                *ptr;   // Raw native C pointer, used for C modules
    } as;
} WValue;

typedef WValue (*CNativeFn)();
typedef struct _WFunction {
    uint64_t    refCnt;
    enum _WFuncType : uint8_t {
        FUNCTYPE_C,
        FUNCTYPE_W,
    } type;
    union {
        CNativeFn   cfunc;
        uint64_t    bytecodeOffset;
    };
} WFunction;

enum _FastOpSlot {
    FASTOP_INIT,
    FASTOP_DEL,

    FASTOP_ADD,
    FASTOP_SUB,
    FASTOP_MUL,
    FASTOP_DIV,
    FASTOP_EQ,
    FASTOP_GT,
    FASTOP_LT,
    FASTOP_GET,
    FASTOP_SET,

    FASTOP_COUNT
};

typedef struct _WClass {
    const char      *name;
    size_t          instanceSize; // Memory required for raw instance data or fields
    size_t          datasize;
    WValue          fastops[FASTOP_COUNT];
    WValue          *fieldMap;
} WClass;

typedef struct _WObject {
    uint64_t    refCnt;
    WClass      *cls;
    union {
        WValue  *fields;
        void    *data;
    };
} WObject;

typedef struct _WString {
    uint64_t    refCnt;
    size_t      len;
    uint64_t    hash;
    char        *buf;
} WString;

#endif
