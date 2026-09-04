#ifndef __WVM_CONTEXT_H__
#define __WVM_CONTEXT_H__

#include <stdint.h>

#include "WVM_Types.h"
#include "WVM_Stack.h"

typedef struct WVMContext {
    WVM_Stack   stack;

    WVM_Value   acc;
    WVM_Value   *locals;
    uint16_t    *ip;
} WVMContext;

#endif
