#include <stdlib.h>
#include <stddef.h>

#include "WVM_Stack.h"
#include "WVM_Types.h"

#define DEFAULT_SIZE  (1 << 5)
#define GROWTH_FACTOR 1.5

int WStackInit(WVM_Stack *stack) {
    WVM_Stack ret = {
        .size = DEFAULT_SIZE,
        .count = 0,
        .buf = malloc(sizeof(WVM_Value) * DEFAULT_SIZE),
    };
    if (!ret.buf) return 0;

    *stack = ret;
    return 1;
}

int WStackPush(WVM_Stack *stack, WVM_Value *element) {
    if (stack->size == stack->count) {
        size_t targetSize = stack->size * GROWTH_FACTOR;
        WVM_Value *new = realloc(stack->buf, sizeof(WVM_Value) * targetSize);
        if (!new) return 0;

        stack->buf = new;
        stack->size = targetSize;
    }

    stack->buf[stack->count++] = *element;
    return 1;
}

int WStackPop(WVM_Stack *stack, WVM_Value *ret) {
    if (stack->count == 0) return 0;

    *ret = stack->buf[--stack->count];
    if (stack->size > DEFAULT_SIZE && stack->count == stack->size / GROWTH_FACTOR / GROWTH_FACTOR) {
        size_t targetSize = stack->size / GROWTH_FACTOR;
        WVM_Value *new = realloc(stack->buf, sizeof(WVM_Value) * targetSize);
        if (new) {
            stack->buf = new;
            stack->size = targetSize;
        }
    }
    return 1;
}
