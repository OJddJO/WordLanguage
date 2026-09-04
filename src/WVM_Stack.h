#ifndef __WVM_STACK_H__
#define __WVM_STACK_H__

#include <stdint.h>

#include "WVM_Types.h"

typedef struct _Stack {
    uint64_t    size;
    uint64_t    count;
    WVM_Value      *buf;
} WVM_Stack;

int WStackInit(WVM_Stack *stack);
int WStackPush(WVM_Stack *stack, WVM_Value *element);
int WStackPop(WVM_Stack *stack, WVM_Value *ret);

#endif
