#ifndef __WVMCONTEXT_H__
#define __WVMCONTEXT_H__

#include <stdint.h>

#include "stack.h"
#include "WTypes.h"

typedef struct WVMContext {
    Stack   stack;

    WValue  acc;
    WValue  *locals;
    uint8_t *ip;
} WVMContext;

#endif
