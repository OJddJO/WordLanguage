#ifndef __STACK_H__
#define __STACK_H__

#include <stdint.h>

typedef struct _Stack {
    uint64_t    size;
    uint64_t    count;
    size_t      elemSize;
    uint8_t     *buf;
} Stack;

int stackInit(Stack *stack, size_t elementSize);
int stackPush(Stack *stack, void *element);
int stackPop(Stack *stack, void *ret);
void stackDestroy(Stack *stack);

#endif
