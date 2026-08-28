#ifndef __WOBJECT_H__
#define __WOBJECT_H__

#include <stddef.h>
#include <stdint.h>

typedef struct _WType {
    const char  *typeName;
    size_t      size;
} WType;

typedef struct _WFunction {
    const char  *functionName;
    size_t      numberOfArgs;
    enum _WFuncType {
        WFUNCTYPE_NATIVE,
        WFUNCTYPE_C
    } type;
    union {
        struct {

        } native;
        struct {

        } c;
    };
} WFunction;

typedef struct _WObject {
    WType       type;
    const char  *objName;
    void        *value;
    uint64_t    refCount;
} WObject;

#endif
