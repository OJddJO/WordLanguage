#ifndef __WVM_CONTEXT_H__
#define __WVM_CONTEXT_H__

#include <stdint.h>

#include <stack.h>
#include "WVM_Types.h"

typedef struct WVM_Context {
    Stack       stack;

    WVM_Value   acc;
    WVM_Value   *locals;
    uint16_t    *ip;
} WVM_Context;

#endif
