#ifndef __STACK_H__
#define __STACK_H__

#include <stdint.h>

typedef struct _Stack {
    uint64_t    size;
    uint64_t    count;
    void        **buf;
} Stack;

int stackInit(Stack *stack);
int stackPush(Stack *stack, void *element);
void *stackPop(Stack *stack);

#endif
